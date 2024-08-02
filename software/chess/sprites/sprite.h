#ifndef SPRITE_H
#define SPRITE_H

#include <system.h>
#include <alt_types.h>

extern alt_u8 mouseSprite[3][3];
extern alt_u8 pawnSprite[19][19];
extern alt_u8 queenSprite[19][19];
extern alt_u8 kingSprite[19][19];
extern alt_u8 bishopSprite[19][19];
extern alt_u8 knightSprite[19][19];
extern alt_u8 rookSprite[19][19];
extern alt_u8 whitesmove[19][19];
extern alt_u8 blacksmove[19][19];
extern alt_u8 checkSprite[19][19];

extern alt_u8 menuSprite[19][19];
extern alt_u8 restartSprite[19][19];
extern alt_u8 colorSprite[19][19];
extern alt_u8 squareSprite[19][19];
extern alt_u8 gameoverSprite[19][19];
extern alt_u8 enterSprite[19][19];
extern alt_u8 computerSprite[19][19];



extern const int numSprites;
extern int pieceColor[2];

#endif

