/*
 *  text_mode_vga.h
 *	Minimal driver for text mode VGA support, ECE 385 Summer 2021 Lab 6
 *  You may wish to extend this driver for your final project/extra credit project
 * 
 *  Created on: Jul 17, 2021
 *      Author: zuofu
 */

#ifndef VGA_COLOR_H_
#define VGA_COLOR_H_

#define COLUMNS 80
#define ROWS 240

#include <system.h>
#include <alt_types.h>

extern int computer;

extern alt_u8 frame;
extern int xMousePos;
extern int yMousePos;

extern int menuOpen;
extern int colorMenuOpen;
extern alt_u8 randColors[16][3];
extern int setRandom;

extern int colorHover;
extern int resetHover;
extern int enterHover;
extern int computerHover;

extern int hoverQueen;
extern int hoverRook;
extern int hoverBishop;
extern int hoverKnight;

struct TEXT_VGA_STRUCT {
	alt_u8 VRAM [ROWS*COLUMNS*4]; //Week 2 - extended VRAM
	alt_u8 VRAM2 [ROWS*COLUMNS*4];
	alt_u32 RESERVED[27136];
	alt_u32 COLOR_PALETTE[128];
	alt_u32 RESERVED2[65408];
	alt_u32 DRAW[64];
	//modify this by adding const bytes to skip to palette, or manually compute palette
};



struct COLOR{
	char name [20];
	alt_u8 red;
	alt_u8 green;
	alt_u8 blue;
};


//you may have to change this line depending on your platform designer
static volatile struct TEXT_VGA_STRUCT* vga_ctrl = VGA_TEXT_MODE_CONTROLLER_0_BASE;

//CGA colors with names
static struct COLOR colors[]={
    {"black",          0x0, 0x0, 0x0},
	{"blue",           0x0, 0x0, 0xa},
    {"green",          0x0, 0xa, 0x0},
	{"cyan",           0x0, 0xa, 0xa},
    {"red",            0xa, 0x0, 0x0},
	{"magenta",        0xa, 0x0, 0xa},
    {"brown",          0xa, 0x5, 0x0},
	{"light gray",     0xa, 0xa, 0xa},
    {"dark gray",      0x5, 0x5, 0x5},
	{"light blue",     0x5, 0x5, 0xf},
    {"light green",    0x5, 0xf, 0x5},
	{"light cyan",     0x5, 0xf, 0xf},
    {"light red",      0xf, 0x5, 0x5},
	{"light magenta",  0xf, 0x5, 0xf},
    {"yellow",         0xf, 0xf, 0x5},
	{"white",          0xf, 0xf, 0xf}
};
//updated quick draw functions
void drawFrame();
void drawBoard();
void drawPromote();

void storeSprite();
void drawSprites(alt_u8 sprite_num, alt_u8 color, alt_u16 x, alt_u16 y);

void textVGAColorClr();
void setColorPalette (alt_u8 color, alt_u8 red, alt_u8 green, alt_u8 blue); //Fill in this code
void textVGAColorTest();

void setPixelColor(alt_u8 color, int x, int y);
alt_u8 getPixelColor(int x, int y);

void drawAllPieces(); //draws board from temp board
void drawPiece(char piece, int x, int y); //char and monitor x,y

void drawTurn();

void drawMenu();
void drawColorMenu();

void computerMove();

void drawButton();

void setColors();
void moveMouse(int xDisp, int yDisp, int button);
void drawMouse();
#endif /*VGA_COLOR_H_*/
