#ifndef GAME_H
#define GAME_H

#include <system.h>
#include <alt_types.h>



extern char default_board[8][8];
extern char current_board[8][8];
extern char temp_board[8][8];

static int hand_x; //where on current board is the piece in hand from
static int hand_y;

extern int gameOver;


extern int promote;
extern int promoteX;
extern int promoteY;
extern char promotePiece;

extern int hand; // if a piece is being held
extern int turn; //whos turn 0 = white, 1 = black
extern int whiteCheck;
extern int blackCheck;
extern int checkPiece[2];

extern int whiteCastle;
extern int whiteLeftRook;
extern int whiteRightRook;
extern int whiteO_O_O;
extern int whiteO_O;

extern int blackCastle;
extern int blackLeftRook;
extern int blackRightRook;
extern int blackO_O_O;
extern int blackO_O;


void game_setup();
int legalMove(int x, int y, int color); //determines if a move is legal or not
int canBlock(int x, int y, int color);
char inCheck(int color);
int inCheckmate(int color);
int moveIntoCheck(int x, int y, int color); //determines if a move puts the player into check
void pick_up(int x, int y); // picks up piece
void put_down(int x, int y); //places piece
void draw_hand(int x, int y); //draws the current picked up piece under the mouse
#endif
