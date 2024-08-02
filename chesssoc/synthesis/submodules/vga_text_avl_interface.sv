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
logic FRAME, WRITE_BOARD;


//vram mux signals
logic [14:0] address_a_0,address_a_1, address_b_1, address_b_0, draw_address, write_address;
logic wren_a_0, wren_a_1, wren_b_0, wren_b_1, wren_b;
logic [31:0] q_a_0, q_a_1, q_b_0, q_b_1, VRAM_WRITEDATA, BOARD_DATA;
logic [3:0] byteena_b, byteena_b_0, byteena_b_1;

//logic
logic pixel_clk, blank;
logic [9:0]  DrawX, DrawY;
logic [8:0]  WriteX, WriteY;
logic WriteX_S, WriteY_S;
logic [10:0] shape_x, shape_y;
logic [31:0] VRAM_Data;
logic [7:0]  IDX;
logic [3:0]  IDX_Red, IDX_Green, IDX_Blue;
logic [7:0]  WRITE_PAL_ADDR;

//sprite signals
logic [18:0] SPRITE_DATA;
logic [4:0] SPRITE_ADDR; 
logic STORE_SPRITE, WRITE_SPRITE, CLEAR_SPRITE, hold_low, store_sprite_single;
logic [7:0] SPRITE_IDX;
logic [8:0] SPRITE_X, SPRITE_Y;
logic [18:0][18:0] SPRITE_VAL;
logic test;


//palette register file
logic [31:0] PALETTE_REG [`NUM_PALETTES];

//individual draw commands

vram	vram0 (
	.address_a ( address_a_0 ),
	.address_b ( address_b_0 ),
	.byteena_a ( AVL_BYTE_EN ),
	.byteena_b ( byteena_b_0 ),
	.clock ( CLK ),
	.data_a ( AVL_WRITEDATA ),
	.data_b ( VRAM_WRITEDATA),
	.wren_a ( wren_a_0 ),
	.wren_b ( wren_b_0 ),
	.q_a ( q_a_0 ),
	.q_b ( q_b_0 )
	);
	
vram	vram1 (
	.address_a ( address_a_1 ),
	.address_b ( address_b_1 ),
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

//mux the two vrams a port
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

//mux vram two b ports
always_comb begin
	if(~FRAME) begin // frame = 0 so 0 is being drawn and 1 is being changed
		address_b_0 = draw_address;
		address_b_1 = write_address;
		wren_b_0    = 1'b0;
		wren_b_1    = wren_b;
		q_b_vram    = q_b_0;
		byteena_b_0 = 4'b1111;
		byteena_b_1 = byteena_b;
	end
	else begin // frame = 1 so 1 is being drawn and 0 is being changed
		address_b_0 = write_address;
		address_b_1 = draw_address;
		wren_b_0    = wren_b;
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


//sprite logic to only store for a clock cycle
always_ff @(posedge CLK) begin
	if(STORE_SPRITE) begin
	   if(store_sprite_single || hold_low)
			store_sprite_single <= 1'b0;
		else
			store_sprite_single <= 1'b1;
			hold_low <= 1'b1;
	end
	else begin
		hold_low <= 1'b0;
		store_sprite_single <= 1'b0;
	end
end

sprite_rom srom0(.data_in(SPRITE_DATA), 
					  .addr(SPRITE_ADDR), 
					  .clk(CLK), 
					  .reset(RESET), 
					  .clear(CLEAR_SPRITE), 
					  .write(store_sprite_single), 
					  .sprite_out(SPRITE_VAL));

   //avl[17:16] =
   //00 -> vram
	//01 -> palette
	//10 -> sprite
	//11 -> draw control signal  
	
// Read and write from AVL interface to register block, note that READ waitstate = 1, so this should be in always_ff
always_comb begin

	if(AVL_WRITE && AVL_CS && (AVL_ADDR[17] !== 1'b1) && (AVL_ADDR[16] !== 1'b1)) begin 
		wren_a_vram  = 1'b1;
	end	
	
	else begin
		wren_a_vram  = 1'b0;
	end
	
	
	if((AVL_ADDR[17] !== 1'b1)  && (AVL_ADDR[16] === 1'b1)) begin
		AVL_READDATA = PALETTE_REG[AVL_ADDR[7:0]];
	end
	
	else if(AVL_ADDR[17] === 1'b1) begin
		AVL_READDATA = FRAME;
	end
	
	else begin
		AVL_READDATA = q_a_vram;
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






//figure out what is to be written and to which frame
always_ff @(posedge CLK) begin
	if((AVL_ADDR[17] === 1) && AVL_WRITE && AVL_CS) begin 
	
		if(AVL_ADDR[0] === 1) begin //draw board command
			WRITE_BOARD  <= AVL_WRITEDATA[31];
			BOARD_DATA   <= AVL_WRITEDATA;
		end
		
		else if(AVL_ADDR[1] === 1) begin //store sprite command
			STORE_SPRITE <= AVL_WRITEDATA[31];
			SPRITE_DATA  <= AVL_WRITEDATA[30:12];
			SPRITE_ADDR  <= AVL_WRITEDATA[4:0];
		end
		
		else if(AVL_ADDR[2] === 1) begin //write sprite
			WRITE_SPRITE <= AVL_WRITEDATA[31];
			SPRITE_IDX   <= AVL_WRITEDATA[30:23];
			SPRITE_X     <= AVL_WRITEDATA[22:14];  //in terms of writex and writey
			SPRITE_Y     <= AVL_WRITEDATA[13:5];
			SPRITE_ADDR  <= AVL_WRITEDATA[4:0];
		end
		
		else if(AVL_ADDR[3] === 1) begin //clear sprite
			CLEAR_SPRITE <= AVL_WRITEDATA[31];
			SPRITE_ADDR  <= AVL_WRITEDATA[4:0];
		end
		
		else begin
			FRAME <= AVL_WRITEDATA[0];
		end
	end
end

//writeX and y counters
always_ff @ (posedge CLK) begin
	if ( RESET ) begin 
		 WriteX <= 9'b000000000;
		 WriteY <= 9'b000000000;
		 
	end
		
	else if ( WriteX == 319 ) begin 
		WriteX <= 9'b000000000;
		
		if ( WriteY == 239 )  
			WriteY <= 9'b000000000;
		else 
			WriteY <= (WriteY + 1);
	end
	
	else 
		WriteX <= (WriteX + 1);
end


//write Board logic
always_comb begin
	if((WriteX >= 9'd60  && WriteX <= 9'd84 ) 
	|| (WriteX >= 9'd110 && WriteX <= 9'd134)
	|| (WriteX >= 9'd160 && WriteX <= 9'd184)
	|| (WriteX >= 9'd210 && WriteX <= 9'd234)) begin
		WriteX_S	 = 1;
	end
	else begin
		WriteX_S	 = 0;
	end
	
	if((WriteY >= 9'd20  && WriteY <= 9'd44 ) 
	|| (WriteY >= 9'd70  && WriteY <= 9'd94 )
	|| (WriteY >= 9'd120 && WriteY <= 9'd144)
	|| (WriteY >= 9'd170 && WriteY <= 9'd194)) begin
		WriteY_S	 = 1;
	end
	else begin
		WriteY_S	 = 0;
	end
	
end

//test
always_ff @ (posedge CLK) begin
	test <= SPRITE_VAL[9][9];
end


//write logic
always_comb begin
	write_address = ((WriteX >> 2) + (WriteY*80));
	
	
	//write board
	if(WRITE_BOARD) begin
		wren_b = 1'b1;
		if(WriteX < 60 || WriteX >= 260 || WriteY < 20 || WriteY >= 220) begin
			WRITE_PAL_ADDR = BOARD_DATA[8:1]; //set background color
		end
		else if(WriteX_S ~^ WriteY_S) begin
			WRITE_PAL_ADDR = BOARD_DATA[16:9]; //set white square color
		end
		else begin
			WRITE_PAL_ADDR = BOARD_DATA[24:17]; //set black square color
		end
	end
	
	
	//write sprite
	else if (WRITE_SPRITE) begin
		WRITE_PAL_ADDR = SPRITE_IDX; //the color should be the one from the input
		
		if((WriteY - SPRITE_Y >= 0) && (WriteY - SPRITE_Y <= 18) && (WriteX - SPRITE_X >= 0) && (WriteX - SPRITE_X <= 18)) begin
			if(SPRITE_VAL[WriteY - SPRITE_Y][WriteX - SPRITE_X] === 1'b1) //if there is a 1 at the position then write
				wren_b = 1'b1;
			else
				wren_b = 1'b0;
		end
		
		else
			wren_b = 1'b0;
	
	end
	
	//nothing being written
	else begin
		wren_b = 1'b0;
		WRITE_PAL_ADDR = 8'h00;
	end			
	
	VRAM_WRITEDATA[7:0]   = WRITE_PAL_ADDR;
	VRAM_WRITEDATA[15:8]  = WRITE_PAL_ADDR;
	VRAM_WRITEDATA[23:16] = WRITE_PAL_ADDR;
	VRAM_WRITEDATA[31:24] = WRITE_PAL_ADDR;
end




always_comb begin

	case(WriteX[1:0]) //determine the byte select
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
	draw_address  = ((DrawX >> 3) + ((DrawY >> 1)*80));
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


























