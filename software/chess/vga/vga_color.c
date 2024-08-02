/*
 * text_mode_vga_color.c
 * Minimal driver for text mode VGA support
 * This is for Week 2, with color support
 *
 *  Created on: Oct 25, 2021
 *      Author: zuofu
 */

#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <alt_types.h>
#include <time.h>
#include "vga_color.h"
#include "../sprites/sprite.h"
#include "../chess_game/game.h"

int computer = 0;

int menuOpen = 0;
int colorMenuOpen;

int colorHover = 0;
int resetHover = 0;
int enterHover = 0;
int computerHover = 0;


int hoverQueen = 0;
int hoverRook = 0;
int hoverBishop = 0;
int hoverKnight = 0;


alt_u8 frame = 0x00;
int xMousePos = 30;
int yMousePos = 70;
alt_u8 randColors[16][3];
int setRandom = 1;

void textVGAColorClr()

{
	for (int i = 0; i<(ROWS*COLUMNS) * 4; i++)
	{
		vga_ctrl->VRAM[i]  = 0x00;
		vga_ctrl->VRAM2[i] = 0x00;
	}
	for (int i = 0; i<128; i++)
	{
		vga_ctrl->COLOR_PALETTE[i] = 0x00000000;
	}
}

void setPixelColor(alt_u8 color, int x, int y) // 0<x<320, 0<y<240
{
	if(frame){
		vga_ctrl->VRAM[(y*COLUMNS*4) + x] = color;
	}
	else{
		vga_ctrl->VRAM2[(y*COLUMNS*4) + x] = color;
	}
}

alt_u8 getPixelColor(int x, int y){
	if(frame){
		return vga_ctrl->VRAM[(y*COLUMNS*4) + x];
	}
	return vga_ctrl->VRAM2[(y*COLUMNS*4) + x];


}

void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue)
{
	if(color % 2 == 0){
		vga_ctrl->COLOR_PALETTE[color/2] &= 0xFFFFE000;
		vga_ctrl->COLOR_PALETTE[color/2] |= (((alt_u32)(red))   << 9)
										 |  (((alt_u32)(green)) << 5)
										 |  (((alt_u32)(blue))  << 1);
	}
	else if(color % 2 == 1){
			vga_ctrl->COLOR_PALETTE[color/2] &= 0x00001FFF;
			vga_ctrl->COLOR_PALETTE[color/2] |= (((alt_u32)(red))   << 21)
					                         |  (((alt_u32)(green)) << 17)
											 |  (((alt_u32)(blue))  << 13);
	}
}


void setColors(){
	setColorPalette (1, 0x1, 0x1, 0x1); // 1 is background
	setColorPalette (2, 0xc, 0xc, 0xc); // 2 is white squares
	setColorPalette (3, 0xf, 0xf, 0xf); // 3 is black squares

	setColorPalette (4, 0x0, 0xc, 0x0); // 4 is mouse color green

	setColorPalette (5, 0xf, 0xf, 0x0); // 5 is white's primary
	setColorPalette (6, 0x0, 0xf, 0xf); // 6 is white's secondary

	setColorPalette (7, 0x0, 0x0, 0xf); // 5 is black's primary
	setColorPalette (8, 0xf, 0x0, 0x0); // 6 is blacks's secondary

	setColorPalette (9, 0xa, 0x1, 0x1); // 9 is the check color

	setColorPalette (10, 0xd, 0xd, 0xd); // menu button color 1
	setColorPalette (11, 0xc, 0xc, 0xc); // menu button color 2

	setColorPalette (12, 0xa, 0x1, 0x1); // restart button color 1
	setColorPalette (13, 0xa, 0x1, 0x1); // restart button color 2

	setColorPalette (14, 0x2, 0x2, 0x2); // menu background color

	setColorPalette (15, 0x5, 0xc, 0x9); // color button color 1
	setColorPalette (16, 0x9, 0x5, 0x5); // color button color 2

	setColorPalette (17, 0x4, 0xf, 0x3); // game over  color 1
	setColorPalette (18, 0x0, 0x0, 0x0); // game over  color 2

	setColorPalette (19, 0x4, 0x4, 0x4); // promote background 1
	setColorPalette (20, 0x1, 0x1, 0x1); // promote background 2

	setColorPalette (20, 0x1, 0xd, 0x1); // enter color 1
	setColorPalette (21, 0x1, 0x8, 0x1); // enter color 2

	setColorPalette (22, 0x0, 0x3, 0xf); // human color
	setColorPalette (23, 0x6, 0x6, 0x6); // computer color
}



void storeSprites(){
	alt_u32 enable  = 2147483648;
	alt_u8  addr = 0;
	alt_u8  one = 1;
	alt_u8 sprites[numSprites][19][19];
	for(int k = 0; k < numSprites; k++){
		for(int i = 0; i < 19; i++){ //copy the board
			for(int j = 0; j < 19; j++){
				switch(k){
					case 0 :
						sprites[k][i][j] = pawnSprite[i][j];
						break;
					case 1 :
						sprites[k][i][j] = queenSprite[i][j];
						break;
					case 2 :
						sprites[k][i][j] = kingSprite[i][j];
						break;
					case 3 :
						sprites[k][i][j] = bishopSprite[i][j];
						break;
					case 4 :
						sprites[k][i][j] = knightSprite[i][j];
						break;
					case 5 :
						sprites[k][i][j] = rookSprite[i][j];
						break;
					case 6 :
						sprites[k][i][j] = whitesmove[i][j];
						break;
					case 7 :
						sprites[k][i][j] = blacksmove[i][j];
						break;
					case 8 :
						sprites[k][i][j] = checkSprite[i][j];
						break;
					case 9 :
						sprites[k][i][j] = menuSprite[i][j];
						break;
					case 10 :
						sprites[k][i][j] = restartSprite[i][j];
						break;
					case 11 :
						sprites[k][i][j] = colorSprite[i][j];
						break;
					case 12 :
						sprites[k][i][j] = squareSprite[i][j];
						break;
					case 13 :
						sprites[k][i][j] = gameoverSprite[i][j];
						break;
					case 14 :
						sprites[k][i][j] = enterSprite[i][j];
						break;
					case 15 :
						sprites[k][i][j] = computerSprite[i][j];
						break;
				}
			}
		}
	}

	alt_u8  numColors = 2;
	alt_u32 rowData = 0;
	alt_u32 spriteData[19];

	for(int l = 0; l < numSprites; l++){
		addr = numColors * l;
		for(int k = 1; k <= numColors; k++){
			for(int j = 0; j < 19; j++){
				for(int i = 0; i < 19; i++){
					if(sprites[l][j][i] != 0 ){ //skip data if zero
						if(sprites[l][j][i] == k){ //if the sprite is the color store it
							rowData |=  ((alt_u32)one << i);
						}
					}
				}
				spriteData[j] = rowData;
				rowData = 0;
			}
			for(int i = 0; i < 19; i++){
				alt_u32 draw_data =  (((alt_u32)(spriteData[i])) << 12)
								  |  (((alt_u32)(addr + k - 1 ))      );
				vga_ctrl->DRAW[2] = draw_data |  enable;
				vga_ctrl->DRAW[2] = draw_data;
				spriteData[i] = 0; //reset the sprite data
			}
		}
	}


}

void drawSprite(alt_u8 addr, alt_u8 color, alt_u16 x, alt_u16 y){
	alt_u32 enable   = 2147483648;
	alt_u8 numColors = 2;
	alt_u32 draw_data =  (((alt_u32)(color))     << 23)
					  |  (((alt_u32)(x))         << 14)
					  |  (((alt_u32)(y))         << 5 )
					  |  (((alt_u32)(addr))      );
	vga_ctrl->DRAW[4] = draw_data |  enable;
	usleep(300);
	vga_ctrl->DRAW[4] = draw_data; //disable draw

}


void drawFrame()
{
	drawBoard();
	drawButton();
	drawAllPieces();
	drawTurn();
	drawPromote();
	drawMenu();
	drawColorMenu();
	drawMouse();
	frame ^= 0x01; //flip the frame
	vga_ctrl->DRAW[0] = frame; //draw the frame
	//printf("frame = 0x%01X ", frame);
}

void drawButton(){
	if(colorMenuOpen){return;}

	drawSprite(18, 10, 292, 10); //menu button
	if(menuOpen){
		drawSprite(19, 11, 292, 10); //menu button pressed
	}
}

void drawMenu(){
	if(colorMenuOpen){return;}
	if(menuOpen || gameOver){ //if menu button is pressed
		drawSprite(24, 14, 151, 102); //draw background squares
		drawSprite(24, 14, 151, 120);
		drawSprite(24, 14, 170, 102);
		drawSprite(24, 14, 170, 120);
		drawSprite(24, 14, 132, 102);
		drawSprite(24, 14, 132, 120);

		drawSprite(20, 12, 132, 111); //reset button
		if(resetHover){
			drawSprite(21, 13, 132, 111); //reset hover
		}

		drawSprite(22, 15, 151, 111); //color button
		if(colorHover){
			drawSprite(23, 16, 151, 111); //color hover
		}


		if(computer){
			drawSprite(31, 23, 170, 111); //computer button on
		}
		else{
			drawSprite(30, 22, 170, 111); //computer button off
		}

		if(gameOver){
			drawSprite(26, 17, 151, 92);
			drawSprite(27, 18, 151, 92);
		}
	}
}

void drawColorMenu(){
	if(colorMenuOpen){
		drawPiece('Q',13,16);
		drawPiece('q',287,16);

		drawSprite(24, 14, 151, 102); //draw background squares
		drawSprite(24, 14, 151, 120);
		drawSprite(24, 14, 170, 102);
		drawSprite(24, 14, 170, 120);
		drawSprite(24, 14, 132, 102);
		drawSprite(24, 14, 132, 120);

		//redo colors
		drawSprite(20, 15, 141, 111);
		if(resetHover){
			drawSprite(21, 16, 141, 111);
		}

		//enter button
		drawSprite(28, 20, 160, 111);
		if(enterHover){
			drawSprite(29, 21, 160, 111);
		}

		for(int i = 0; i < 16; i++){
			if(setRandom){
				randColors[i][0] = rand() % 256;
				randColors[i][1] = rand() % 256;
				randColors[i][2] = rand() % 256;
			}
			setColorPalette (32 + i, randColors[i][0], randColors[i][1], randColors[i][2]);
			if(i < 8){
				drawSprite(24, 32+i, 3, 41 + i + (i*19));
				drawSprite(24, 32+i, 23, 41 + i + (i*19));
			}
			else{
				int j = i - 8;
				drawSprite(24, 32+i, 297, 41 + j + (j*19));
				drawSprite(24, 32+i, 277, 41 + j + (j*19));
			}
		}
		setRandom = 0;
	}
}

void drawPromote(){
	if(promote){
		if(turn == 0){ //draw whites pieces
			if(hoverQueen) {drawSprite(24, 19, 38, 32);}
			else           {drawSprite(24, 20, 38, 32);}
			if(hoverRook)  {drawSprite(24, 19, 38, 54);}
			else           {drawSprite(24, 20, 38, 54);}
			if(hoverBishop){drawSprite(24, 19, 38, 76);}
			else           {drawSprite(24, 20, 38, 76);}
			if(hoverKnight){drawSprite(24, 19, 38, 98);}
			else           {drawSprite(24, 20, 38, 98);}

			drawPiece('Q',38,32);
			drawPiece('R',38,54);
			drawPiece('B',38,76);
			drawPiece('N',38,98);
		}
		if(turn == 1){ //draw whites pieces
			if(hoverQueen) {drawSprite(24, 19, 38, 211);}
			else           {drawSprite(24, 20, 38, 211);}
			if(hoverRook)  {drawSprite(24, 19, 38, 189);}
			else           {drawSprite(24, 20, 38, 189);}
			if(hoverBishop){drawSprite(24, 19, 38, 167);}
			else           {drawSprite(24, 20, 38, 167);}
			if(hoverKnight){drawSprite(24, 19, 38, 145);}
			else           {drawSprite(24, 20, 38, 145);}

			drawPiece('q',38,211);
			drawPiece('r',38,189);
			drawPiece('b',38,167);
			drawPiece('n',38,145);
		}
	}
}

void drawBoard(){
	alt_u32 enable   = 2147483648;
	alt_u8  background    = 1;
	alt_u8  white_squares = 2;
	alt_u8  black_squares = 3;
	alt_u32 draw_data =  (((alt_u32)(background))      << 1 )
					  |  (((alt_u32)(black_squares))   << 9 )
					  |  (((alt_u32)(white_squares))   << 17);
	//printf("Draw Data: 0x%X\n\r", draw_data);
	vga_ctrl->DRAW[1] = draw_data |  enable;
	usleep(290);
	vga_ctrl->DRAW[1] = draw_data;

}

void drawTurn(){
	if(colorMenuOpen){return;}
	drawSprite(12 + (2*turn), pieceColor[0] + (2*turn), 292, 212);
	drawSprite(13 + (2*turn), pieceColor[1] + (2*turn), 292, 212);
}


void drawAllPieces(){
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			char piece = temp_board[x][y];
			if(piece != ' '){
				drawPiece(piece, 63 + (25*x), 23 + (25*y));
			}
		}
	}
}

void drawPiece(char piece, int x, int y){
	//if(menuOpen){return;}
	alt_u8 c = (isupper(piece) == 0); //if its upper case c = 0 so piece is white
	piece = toupper(piece);
	switch(piece){
		case('P'):
			drawSprite(0, pieceColor[0] + (2*c), x, y);
			drawSprite(1, pieceColor[1] + (2*c), x, y);
			break;
		case('Q'):
			drawSprite(2, pieceColor[0] + (2*c), x, y);
			drawSprite(3, pieceColor[1] + (2*c), x, y);
			break;
		case('K'):
			if(whiteCheck && (c == 0)){drawSprite(16, 9, x, y);}
			if(blackCheck && (c == 1)){drawSprite(16, 9, x, y);}
			drawSprite(4, pieceColor[0] + (2*c), x, y);
			drawSprite(5, pieceColor[1] + (2*c), x, y);
			break;
		case('B'):
			drawSprite(6, pieceColor[0] + (2*c), x, y);
			drawSprite(7, pieceColor[1] + (2*c), x, y);
			break;
		case('N'):
			drawSprite(8, pieceColor[0] + (2*c), x, y);
			drawSprite(9, pieceColor[1] + (2*c), x, y);
			break;
		case('R'):
			drawSprite(10, pieceColor[0] + (2*c), x, y);
			drawSprite(11, pieceColor[1] + (2*c), x, y);
			break;
	}
}

void drawMouse(){
	if(hand == 1){
		draw_hand((xMousePos), (yMousePos));
	}

	for(int i = 0; i < 3; i++){ //redraw mouse and save whats under the mouse
		for(int j = 0; j < 3; j++){
			if(mouseSprite[i][j] != 0){
				setPixelColor(mouseSprite[i][j], (xMousePos)+i, (yMousePos)+j);
			}
		}
	}
}

void computerMove(){
	if(computer){
		int xMouseTemp = xMousePos;
		int yMouseTemp = yMousePos;
		while(turn){
			//pick random piece
			if(promote){
				promotePiece = 'q';
				moveMouse(0,0,0);
				break;
			}

			while(hand == 0){
				xMousePos = 70 + ((rand() % 8)*25);
				yMousePos = 30 + ((rand() % 8)*25);
				moveMouse(0, 0, 1);
			}

			int stop = 0;
			for(int i = 0; i < 8; i++){
				for(int j = 0; j < 8; j++){
					if(legalMove(i, j, 1)){
						xMousePos = 70 + ((i*25));
						yMousePos = 30 + ((j*25));
						moveMouse(0,0,0);
						stop = 1;
						break;
					}
				}
				if(stop){
					break;
				}
			}
			moveMouse(0,0,0);
		}
		xMousePos = xMouseTemp;
		yMousePos = yMouseTemp;
	}
}

void moveMouse(int xDisp, int yDisp, int button){

	xMousePos += xDisp * 2;
	yMousePos += yDisp * 2;

	if(xMousePos > 316){
		xMousePos = 316;
	}
	else if (xMousePos < 0){
		xMousePos = 0;
	}
	if(yMousePos > 236){
		yMousePos = 236;
	}
	else if (yMousePos < 0){
		yMousePos = 0;
	}

	if((button == 1)
	&&((xMousePos >= 292 && xMousePos < 317) && (yMousePos >= 10 && yMousePos < 35) )){
		if(colorMenuOpen == 0){
			menuOpen = 1 - menuOpen;
		}
	}
	//color menu logic
	if(colorMenuOpen){
		if((xMousePos >= 141 && xMousePos < 161) && (yMousePos >= 111 && yMousePos < 131)){
			resetHover = 1;
			if(button == 1){
				setRandom = 1;
				resetHover = 0;
			}
		}
		else{
			resetHover = 0;
		}
		if((xMousePos >= 160 && xMousePos < 180) && (yMousePos >= 111 && yMousePos < 131)){
			enterHover = 1;
			if(button == 1){
				menuOpen = 0;
				enterHover = 0;
				setRandom = 1;
				colorMenuOpen = 0;
			}
		}
		else{
			enterHover = 0;
		}
		if(button == 1){
			for(int i = 0; i < 16; i++){
				if(i < 8){
					if((xMousePos >=3 && xMousePos < 3 + 19)&& (yMousePos >= 41 + i + (i*19) && yMousePos < 60 + i + (i*19))){
						setColorPalette(5, randColors[i][0], randColors[i][1], randColors[i][2]);
					};
					if((xMousePos >=23 && xMousePos < 23 + 19)&& (yMousePos >= 41 + i + (i*19) && yMousePos < 60 + i + (i*19))){
						setColorPalette(6, randColors[i][0], randColors[i][1], randColors[i][2]);
					};
				}
				else{
					int j = i - 8;
					if((xMousePos >=297 && xMousePos < 297 + 19)&& (yMousePos >= 41 + j + (j*19) && yMousePos < 60 + j + (j*19))){
						setColorPalette(7, randColors[i][0], randColors[i][1], randColors[i][2]);
					};
					if((xMousePos >=277 && xMousePos < 277 + 19)&& (yMousePos >= 41 + j + (j*19) && yMousePos < 60 + j + (j*19))){
						setColorPalette(8, randColors[i][0], randColors[i][1], randColors[i][2]);
					};
				}
			}
		}
	}
	//main menu logic
	else if(menuOpen || gameOver){
		if((xMousePos >= 132 && xMousePos < 132 + 19) && (yMousePos >= 111 && yMousePos < 131)){
			resetHover = 1;
			if(button == 1){
				game_setup();
				resetHover = 0;
				menuOpen = 0;
			}
		}
		else{
			resetHover = 0;
		}
		if((xMousePos >= 151 && xMousePos < 151 + 19) && (yMousePos >= 111 && yMousePos < 131)){
			colorHover = 1;
			if(button == 1){
				menuOpen = 0;
				colorHover = 0;
				setRandom = 1;
				colorMenuOpen = 1;
			}
		}
		else{
			colorHover = 0;
		}
		if((xMousePos >= 170 && xMousePos < 170 + 19) && (yMousePos >= 111 && yMousePos < 131)){
			if(button == 1){
				computer = 1 - computer;
			}
		}
	}
	//promote menu logic
	else if(promote){
		if(turn == 0){
			if((xMousePos >= 38 && xMousePos < 57) && (yMousePos >= 32 && yMousePos < 51)){
				hoverQueen = 1;
				if(button == 1){
					promotePiece = 'Q';
				}
			}
			else if((xMousePos >= 38 && xMousePos < 57) && (yMousePos >= 54 && yMousePos < 73)){
				hoverRook = 1;
				if(button == 1){
					promotePiece = 'R';
				}
			}
			else if((xMousePos >= 38 && xMousePos < 57) && (yMousePos >= 76 && yMousePos < 95)){
				hoverBishop = 1;
				if(button == 1){
					promotePiece = 'B';
				}
			}
			else if((xMousePos >= 38 && xMousePos < 57) && (yMousePos >= 98 && yMousePos < 117)){
				hoverKnight = 1;
				if(button == 1){
					promotePiece = 'N';
				}
			}
			else{
				hoverQueen = 0;
				hoverRook = 0;
				hoverBishop = 0;
				hoverKnight = 0;
			}
		}
		else{
			if((xMousePos >= 38 && xMousePos < 57) && (yMousePos >= 211 && yMousePos < 230)){
				hoverQueen = 1;
				if(button == 1){
					promotePiece = 'q';
				}
			}
			else if((xMousePos >= 38 && xMousePos < 57) && (yMousePos >= 189 && yMousePos < 208)){
				hoverRook = 1;
				if(button == 1){
					promotePiece = 'r';
				}
			}
			else if((xMousePos >= 38 && xMousePos < 57) && (yMousePos >= 167 && yMousePos < 186)){
				hoverBishop = 1;
				if(button == 1){
					promotePiece = 'b';
				}
			}
			if((xMousePos >= 38 && xMousePos < 57) && (yMousePos >= 145 && yMousePos < 164)){
				hoverKnight = 1;
				if(button == 1){
					promotePiece = 'n';
				}
			}
			else{
				hoverQueen = 0;
				hoverRook = 0;
				hoverBishop = 0;
				hoverKnight = 0;
			}
		}
		put_down(promoteX, promoteY);
	}

	else if( (button == 1 && hand == 0)
	  &&((xMousePos >= 60 && xMousePos < 260) && (yMousePos >= 20 && yMousePos < 220) )){ // click
		pick_up(((xMousePos)-60)/25, ((yMousePos)-20)/25);
	}
	else if((button == 0 && hand == 1)
	  &&((xMousePos >= 60 && xMousePos < 260) && (yMousePos >= 20 && yMousePos < 220) )){ //release
		put_down(((xMousePos)-60)/25, ((yMousePos)-20)/25);
	}

}



void textVGAColorTest (){
	unsigned int base_vram =  &(vga_ctrl->VRAM[0]);
	unsigned int base_vram2 = &(vga_ctrl->VRAM2[0]);
	unsigned int base_pal  = &(vga_ctrl->COLOR_PALETTE[0]);

	printf ("VRAM Base Address: 0x%01X\n\r", base_vram);
	printf ("VRAM2 Base Address: 0x%01X\n\r", base_vram2);
	printf ("Color Palette Base Address: 0x%01X\n\r", base_pal);
	printf ("Color Palette Base Address (RELATIVE): 0x%01X\n\r", (base_pal-base_vram));

	printf("testing VRAM... \n\r ");
	textVGAColorClr();
	//Register write and readback test
	time_t t;
	srand((unsigned) time(&t)); //seed random number

	//vram check
	alt_u32 checksum[ROWS], readsum[ROWS];

	for (int j = 0; j < ROWS; j++)
	{
		checksum[j] = 0;
		for (int i = 0; i < COLUMNS * 4; i++)
		{
			alt_u32 rand_num = (rand() + 1) % 256;
			vga_ctrl->VRAM[(j*COLUMNS*4) + i] = rand_num;
			if(j == 0){
				printf("vram addr: %x, ", (unsigned int)(&(vga_ctrl->VRAM[(j*COLUMNS*4) + i])));
				printf("vram num: %x, ", ((j*COLUMNS*4) + i));
				printf ("random num: %x, ", rand_num);
				printf ("vram data: %x \n\r", vga_ctrl->VRAM[(j*COLUMNS*4) + i]);
			}
			checksum[j] += rand_num;
		}
	}
	for (int j = 0; j < ROWS; j++)
	{
		readsum[j] = 0;
		for (int i = 0; i < COLUMNS*4; i++)
		{
			readsum[j] += vga_ctrl->VRAM[(j*COLUMNS*4) + i];
			//printf("vram addr: %x, ", (unsigned int)(&(vga_ctrl->VRAM[(j*COLUMNS*4) + i])));
			//printf("vram num: %x, ", ((j*COLUMNS*4) + i));
			//printf ("vram data: %x \n\r", vga_ctrl->VRAM[(j*COLUMNS*4) + i]);
		}
		printf ("Row: %d, Checksum: %ld, Read-back Checksum: %ld\n\r", j, checksum[j], readsum[j]);
		if (checksum[j] != readsum[j])
		{
			printf ("Checksum mismatch!, check your Avalon-MM code\n\r");
			while (1){};
		}
	}
	printf ("VRAM Check complete\n\r");




	printf ("Randomizing Color Palette \n\r");
	for (int i = 0; i < 256; i++){
		alt_u8 c1 = rand() % 15;
		alt_u8 c2 = rand() % 15;
		alt_u8 c3 = rand() % 15;
		setColorPalette((alt_u8)i, c1, c2, c3);
	}

	printf ("Printing Palette Values \n\r");
	alt_u32 row_val[128];
	for (int i = 0; i < 128; i++)
	{
		row_val[i] = vga_ctrl->COLOR_PALETTE[i];
	}
	for (int i = 0; i < 128; i++)
	{
		printf("row_val[%d] = 0x%08LU\n\r", i, row_val[i]);
		int B0 = (row_val[i] >> 1 ) & 0x0000000F;
		int G0 = (row_val[i] >> 5 ) & 0x0000000F;
		int R0 = (row_val[i] >> 9 ) & 0x0000000F;
		int B1 = (row_val[i] >> 13) & 0x0000000F;
		int G1 = (row_val[i] >> 17) & 0x0000000F;
		int R1 = (row_val[i] >> 21) & 0x0000000F;
		printf("RGB0 = 0x%01X 0x%01X 0x%01X \n\r", R0, G0, B0);
		printf("RGB1 = 0x%01X 0x%01X 0x%01X \n\r", R1, G1, B1);
	}

	//unsigned int base_vram =  &(vga_ctrl->VRAM[0]);
	//unsigned int base_vram2 = &(vga_ctrl->VRAM2[0]);
	//unsigned int base_pal  = &(vga_ctrl->COLOR_PALETTE[0]);

	//printf ("VRAM Base Address: 0x%01X\n\r", base_vram);
	//printf ("VRAM2 Base Address: 0x%01X\n\r", base_vram2);
	//printf ("Color Palette Base Address: 0x%01X\n\r", base_pal);
	//printf ("Color Palette Base Address (RELATIVE): 0x%01X\n\r", (base_pal-base_vram));


	printf ("Checking Clear\n\r)");
	textVGAColorClr();

	printf ("Colorful Wait");
	for (int k = 0; k < 10000; k++){
		int x = rand() % 320;
		int y = rand() % 240;
		alt_8 color = rand() % 256;
		alt_u8 c1 = rand() % 15;
		alt_u8 c2 = rand() % 15;
		alt_u8 c3 = rand() % 15;

		setColorPalette(color, c1, c2, c3);
		setPixelColor(color, x, y);
	}
}


