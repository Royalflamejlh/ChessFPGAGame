//-------------------------------------------------------------------------
//    Color_Mapper.sv                                                    --
//    Stephen Kempf                                                      --
//    3-1-06                                                             --
//                                                                       --
//    Modified by David Kesler  07-16-2008                               --
//    Translated by Joe Meng    07-07-2013                               --
//                                                                       --
//    Fall 2014 Distribution                                             --
//                                                                       --
//    For use with ECE 385 Lab 7                                         --
//    University of Illinois ECE Department                              --
//-------------------------------------------------------------------------


module  color_mapper ( input               blank, pixel_clk,
							  input        [3:0]  IDX_Red, IDX_Green, IDX_Blue,
                       output logic [3:0]  red, green, blue );
	 
    always_ff @(posedge pixel_clk)
    begin:RGB_Display
		  if (~blank) begin
				red   <= 4'h0;
				green <= 4'h0;
				blue  <= 4'h0;
		  end
        else begin
			  red   <= IDX_Red;
			  green <= IDX_Green;
			  blue  <= IDX_Blue;
		  end 
    end 

endmodule
