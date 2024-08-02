`define NUM_SPRITES 32

module sprite_rom (input logic [18:0] data_in,
						 input logic [4:0]  addr,
						 input logic clk, reset, clear, write,
						 output logic [18:0][18:0] sprite_out
						);
						
logic [18:0][18:0] SPRITE_REG [`NUM_SPRITES];
logic [4:0]  LINE_COUNTER [`NUM_SPRITES];

logic [4:0] line;

//write logic
always_comb begin
	line = LINE_COUNTER[addr];
end


always_ff @(posedge clk) begin
	sprite_out <= SPRITE_REG[addr];

	if(write) begin
		SPRITE_REG[addr][line] <= data_in;
		LINE_COUNTER[addr]     <= (line + 1) % 19;
	end
	
	
	
	else if (clear) begin
		for (int i=0; i<19; i++) begin
			SPRITE_REG[addr][i] <= 19'h00000;
		end
		LINE_COUNTER[addr] <= 5'h00;
	end
	
	
	
	else if (reset) begin
		for (int i=0; i<`NUM_SPRITES; i++) begin
			LINE_COUNTER[i] <= 5'h00;
			for (int j=0; j<19; j++) begin
				SPRITE_REG[i][j] <= 19'h00000;
			end
		end
	end

end

endmodule
