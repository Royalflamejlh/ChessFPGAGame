/************************************************************************
Avalon-MM Interface VGA Text mode display

Register Map:
0x000-0x0257 : VRAM, 80x30 (2400 byte, 600 word) raster order (first column then row)
0x258        : control register

VRAM Format:
X->
[ 31  30-24][ 23  22-16][ 15  14-8 ][ 7    6-0 ]
[IV3][CODE3][IV2][CODE2][IV1][CODE1][IV0][CODE0]

IVn = Draw inverse glyph
CODEn = Glyph code from IBM codepage 437

Control Register Format:
[[31-25][24-21][20-17][16-13][ 12-9][ 8-5 ][ 4-1 ][   0    ] 
[[RSVD ][FGD_R][FGD_G][FGD_B][BKG_R][BKG_G][BKG_B][RESERVED]

VSYNC signal = bit which flips on every Vsync (time for new frame), used to synchronize software
BKG_R/G/B = Background color, flipped with foreground when IVn bit is set
FGD_R/G/B = Foreground color, flipped with background when Inv bit is set

************************************************************************/
`define NUM_PALETTES 128


module vga_text_avl_interface (
	// Avalon Clock Input, note this clock is also used for VGA, so this must be 50Mhz
	// We can put a clock divider here in the future to make this IP more generalizable
	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,					// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,			// Avalon-MM Byte Enable
	input  logic [17:0] AVL_ADDR,			// Avalon-MM Address         17 is high:ADDR is a draw signal, 17 is low 16 is high : access palette, 17 and 16 low : access vram
	input  logic [31:0] AVL_WRITEDATA,		// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,		// Avalon-MM Read Data
	
	// Exported Conduit (mapped to VGA port - make sure you export in Platform Designer)
	output logic [3:0]  red, green, blue,	// VGA color channels (mapped to output pins in top-level)
	output logic hs, vs						// VGA HS/VS
);
//vram 
logic [31:0] q_a_vram, q_b_vram;
logic wren_a_vram;

//control signals
logic FRAME, DRAW_BOARD;


//vram mux signals
logic [14:0] address_a_0,address_a_1, address_b;
logic wren_a_0, wren_a_1, wren_b_0, wren_b_1, wren_b;
logic [31:0] q_a_0, q_a_1, q_b_0, q_b_1, VRAM_WRITEDATA, BOARD_DATA, data_a_0;
logic [3:0] byteena_b, byteena_b_0, byteena_b_1;

//logic
logic pixel_clk, blank;
logic [9:0]  DrawX, DrawY;
logic [10:0] shape_x, shape_y;
logic [31:0] VRAM_Data;
logic [7:0]  IDX;
logic [3:0]  IDX_Red, IDX_Green, IDX_Blue;
logic [7:0]  DRAW_PAL_ADDR;

//palette register file
logic [31:0] PALETTE_REG [`NUM_PALETTES];

//individual draw commands

//extended read and write states to 3
//removed 14:0 from avl_addr
//removed checking addr 16, and 17 in wren_a
//removed the byteenable_b port

//adding back in 14:0 =>
//adding back in add 16 and 17 check for wren_a =>
//adding back byteenabl b set to 0 =>


vram	vram0 (
	.address_a ( AVL_ADDR[14:0] ), //removed [14:0]
	.address_b ( address_b   ),
	.byteena_a ( AVL_BYTE_EN ), //got rid of byteenable b
	.byteena_b (  ),
	.clock ( CLK ),
	.data_a ( AVL_WRITEDATA ),
	.data_b ( VRAM_WRITEDATA),
	.wren_a ( wren_a_vram ),
	.wren_b ( 1'b0 ),
	.q_a ( AVL_READDATA ),
	.q_b ( q_b_0 )
	);

/*
vram	vram1 (
	.address_a ( address_a_1 ),
	.address_b ( address_b   ),
	.byteena_a ( AVL_BYTE_EN ),
	.byteena_b ( byteena_b_1 ),
	.clock ( CLK ),
	.data_a ( AVL_WRITEDATA  ),
	.data_b ( VRAM_WRITEDATA ),
	.wren_a ( wren_a_1 ),
	.wren_b ( wren_b_1 ),
	.q_a ( q_a_1 ),
	.q_b ( q_b_1 )
	);
*/
//mux the two vrams a port
/*
always_comb begin
	if(AVL_ADDR[15:0] < 15'h4B00) begin//first VRAM block
		address_a_0 = AVL_ADDR[14:0];
		address_a_1 = AVL_ADDR[14:0];
		wren_a_0    = wren_a_vram;
		wren_a_1    = 1'b0;
		q_a_vram    = q_a_0;
	end
	else begin
		address_a_0 = AVL_ADDR[14:0];
		address_a_1 = AVL_ADDR[15:0] - 15'h4B00;
		wren_a_0    = 1'b0;
		wren_a_1    = wren_a_vram;
		q_a_vram    = q_a_1;
	end
end
*/

//mux vram two b ports
always_comb begin
	if(~FRAME) begin // frame = 0 so 0 is being drawn and 1 is being changed
		wren_b_0    = 1'b0;
		wren_b_1    = 1'b0;
		q_b_vram    = q_b_0;
		byteena_b_0 = 4'b1111;
		byteena_b_1 = byteena_b;
	end
	else begin // frame = 1 so 1 is being drawn and 0 is being changed
		wren_b_0    = 1'b0;
		wren_b_1    = 1'b0;
		q_b_vram    = q_b_1;
		byteena_b_0 = byteena_b;
		byteena_b_1 = 4'b1111;
	end
end


vga_controller vga0(.Clk(CLK),             // 50 MHz clock
                .Reset(RESET),           // reset signal
                .hs(hs),                 // Horizontal sync pulse.  Active low
					 .vs(vs),                 // Vertical sync pulse.  Active low
					 .pixel_clk(pixel_clk),   // 25 MHz pixel clock output
					 .blank(blank),           // Blanking interval indicator.  Active low.
					 .sync(),                 // Composite Sync signal.  Active low.  We don't use it in this lab,		            
					 .DrawX(DrawX),           // horizontal coordinate
				  	 .DrawY(DrawY) );
					 
					 
color_mapper cmap0(.*);


   //avl[17:16] =
   //00 -> vram
	//01 -> palette
	//10 -> sprite
	//11 -> draw control signal  
	
// Read and write from AVL interface to register block, note that READ waitstate = 1, so this should be in always_ff
always_comb begin

	if(AVL_WRITE && ~AVL_ADDR[16] && AVL_CS && ~AVL_ADDR[17]) begin 
		//wren_a_vram  = 1'b1;
	end	
	
	else begin
		//wren_a_vram  = 1'b0;
	end
	
	
	if(~AVL_ADDR[17]  && AVL_ADDR[16]) begin
		//AVL_READDATA = PALETTE_REG[AVL_ADDR[7:0]];
	end
	
	else if(AVL_ADDR[17]) begin
		//AVL_READDATA = FRAME;
	end
	
	else begin
//		//AVL_READDATA = q_a_vram;
	end
end

always_comb begin
	if(AVL_WRITE && AVL_CS) begin //removed ~AVL_ADDR[16] && ~AVL_ADDR[17]
		wren_a_vram  = 1'b1;
	end	
	else begin
		wren_a_vram  = 1'b0;
	end
end


always_ff @(posedge CLK) begin

	if(AVL_CS && AVL_ADDR[16] && AVL_WRITE && ~AVL_ADDR[17]) begin //uncommented && ~AVL_ADDR[17]
		case(AVL_BYTE_EN)
			4'b1111 : PALETTE_REG[AVL_ADDR[6:0]]        <= AVL_WRITEDATA;
			4'b1100 : PALETTE_REG[AVL_ADDR[6:0]][31:16] <= AVL_WRITEDATA[31:16];
			4'b0011 : PALETTE_REG[AVL_ADDR[6:0]][15:0]  <= AVL_WRITEDATA[15:0];
			4'b1000 : PALETTE_REG[AVL_ADDR[6:0]][31:24] <= AVL_WRITEDATA[31:24];
			4'b0100 : PALETTE_REG[AVL_ADDR[6:0]][23:16] <= AVL_WRITEDATA[23:16];
			4'b0010 : PALETTE_REG[AVL_ADDR[6:0]][15:8]  <= AVL_WRITEDATA[15:8];
			4'b0001 : PALETTE_REG[AVL_ADDR[6:0]][7:0]   <= AVL_WRITEDATA[7:0];
			default : PALETTE_REG[AVL_ADDR[6:0]]        <= AVL_WRITEDATA;
		endcase		
	end
end






//store which frame is being drawn
always_ff @(posedge CLK) begin
	if(AVL_ADDR[17] && AVL_WRITE && AVL_CS) begin 
		FRAME <= AVL_WRITEDATA[0];
		
		if(AVL_ADDR[0]) begin
			DRAW_BOARD <= AVL_WRITEDATA[31];
			BOARD_DATA <= AVL_WRITEDATA;
		end
		else begin
			DRAW_BOARD <= 1'b0;
		end
	end
	
end



always_comb begin

	if(DRAW_BOARD) begin
		wren_b = 1'b0;
		if((DrawX < 120 || DrawX >= 520) || (DrawY < 40 || DrawY >= 440)) begin
			DRAW_PAL_ADDR = BOARD_DATA[8:1]; //set background color
		end
		else if(((DrawX - 120) % 100 < 50) ~^ ((DrawY - 40) % 100 < 50)) begin
			DRAW_PAL_ADDR = BOARD_DATA[16:9]; //set white square color
		end
		else begin
			DRAW_PAL_ADDR = BOARD_DATA[24:17]; //set black square color
		end
	end
	else begin
		wren_b = 1'b0;
		DRAW_PAL_ADDR = 8'h00;
	end
	
end

always_comb begin
	address_b  = (DrawX >> 3) + ((DrawY >> 1)*80); //calculates the current address in vram of 2x2 pixel
	
	//VRAM_WRITEDATA[7:0]   = DRAW_PAL_ADDR;
	//VRAM_WRITEDATA[15:8]  = DRAW_PAL_ADDR;
	//VRAM_WRITEDATA[23:16] = DRAW_PAL_ADDR;
	//VRAM_WRITEDATA[31:24] = DRAW_PAL_ADDR;
end


always_comb begin

	case(DrawX[2:1]) //determine the byte select
		  2'b00 : begin 
				byteena_b = 4'b0001;
					 end
		  2'b01 : begin 
				byteena_b = 4'b0010;
					 end
		  2'b10 : begin 
				byteena_b = 4'b0100;
					 end
		  2'b11 : begin 
				byteena_b = 4'b1000;
					 end
		default : begin
				byteena_b = 4'b0000;
					 end
	endcase
end



//set colors
always_comb begin
	VRAM_Data  = q_b_vram;
	
	case(DrawX[2:1]) //these two bytes determine which spot in vram the data is
		  2'b00 : begin 
						IDX     = VRAM_Data[7:0];
					 end
					 
		  2'b01 : begin 
						IDX     = VRAM_Data[15:8];
					 end
					 
		  2'b10 : begin 
						IDX     = VRAM_Data[23:16];
					 end
					 
		  2'b11 : begin 
						IDX     = VRAM_Data[31:24];
					 end
					 
		default : begin
						IDX = 7'h0;
					 end
	endcase
end




//calculate color from palette idx
always_comb begin
	case(IDX[0])
			1'b0 : begin 
					IDX_Red   = PALETTE_REG[IDX[7:1]][12:9];
					IDX_Green = PALETTE_REG[IDX[7:1]][8:5 ];
					IDX_Blue  = PALETTE_REG[IDX[7:1]][4:1 ];
					end
			1'b1 : begin
					IDX_Red   = PALETTE_REG[IDX[7:1]][24:21]; 
					IDX_Green = PALETTE_REG[IDX[7:1]][20:17];
					IDX_Blue  = PALETTE_REG[IDX[7:1]][16:13];
					end
			default : begin
					IDX_Red   = 4'hF; 
					IDX_Green = 4'h0;
					IDX_Blue  = 4'hF;
					end
	endcase
end


endmodule


























