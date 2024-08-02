#include "sprite.h"
#include <system.h>
#include <alt_types.h>

const int numSprites = 16;
int pieceColor[2] = {5,6};
alt_u8 mouseSprite[3][3] = {
	{ 4, 4, 0},
	{ 4, 4, 0},
	{ 0, 0, 4}
	};
/*
alt_u8 empty[19][19] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
*/
alt_u8 pawnSprite[19][19] = {

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,2,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,2,1,1,1,2,2,0,0,0,0,0,0},
	{0,0,0,0,0,2,1,1,1,1,1,1,1,2,0,0,0,0,0},
	{0,0,0,0,2,1,1,1,1,1,1,1,1,1,2,0,0,0,0},
	{0,0,0,0,2,1,1,1,1,1,1,1,1,1,2,0,0,0,0},
	{0,0,0,0,2,1,1,1,1,1,1,1,1,1,2,0,0,0,0},
	{0,0,0,0,0,2,1,1,1,1,1,1,1,2,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,2,2,1,1,1,1,1,2,2,0,0,0,0,0},
	{0,0,0,0,2,1,1,1,1,1,1,1,1,1,2,0,0,0,0},
	{0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0},
	{0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0},
	{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

alt_u8 queenSprite[19][19] = {

		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,2,0,0,2,1,2,0,0,2,0,0,0,0,0},
		{0,2,0,0,2,1,2,0,0,2,0,0,2,1,2,0,0,2,0},
		{2,1,2,0,0,2,0,0,2,1,2,0,0,2,0,0,2,1,2},
		{0,2,0,0,2,1,2,0,2,1,2,0,2,1,2,0,0,2,0},
		{0,2,2,2,1,1,1,2,1,1,1,2,1,1,1,2,2,2,0},
		{0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0},
		{0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0},
		{0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0},
		{0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0},
		{0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0},
		{0,0,0,0,2,1,1,1,1,1,1,1,1,1,2,0,0,0,0},
		{0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0},
		{0,0,0,2,2,1,1,1,1,1,1,1,1,1,2,2,0,0,0},
		{0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

alt_u8 kingSprite[19][19] = {

		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,2,1,2,2,0,0,0,0,0,0,0},
		{0,0,0,2,2,2,0,2,1,1,1,2,0,2,2,2,0,0,0},
		{0,0,2,1,1,1,2,1,1,1,1,1,2,1,1,1,2,0,0},
		{0,2,1,1,2,1,1,1,1,1,1,1,1,1,2,1,1,2,0},
		{0,2,1,2,1,2,2,2,2,1,2,2,2,2,1,2,1,2,0},
		{0,2,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,2,0},
		{0,2,1,2,1,1,1,1,1,1,1,1,1,1,1,2,1,2,0},
		{0,0,2,1,2,1,1,1,1,1,1,1,1,1,2,1,2,0,0},
		{0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0},
		{0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0},
		{0,0,0,0,2,1,1,1,1,1,1,1,1,1,2,0,0,0,0},
		{0,0,0,0,2,1,1,1,1,1,1,1,1,1,2,0,0,0,0},
		{0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

alt_u8 bishopSprite[19][19] = {

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,2,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,2,1,1,1,2,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,2,1,1,1,2,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,2,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,2,1,1,1,2,2,0,0,0,0,0,0},
	{0,0,0,0,0,2,1,1,1,2,1,1,1,2,0,0,0,0,0},
	{0,0,0,0,0,2,1,1,1,2,1,1,1,2,0,0,0,0,0},
	{0,0,0,0,0,2,1,1,2,2,2,1,1,2,0,0,0,0,0},
	{0,0,0,0,0,2,1,1,1,2,1,1,1,2,0,0,0,0,0},
	{0,0,0,0,0,2,1,1,1,2,1,1,1,2,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,2,2,1,1,1,1,1,2,2,0,0,0,0,0},
	{0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0},
	{0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0},
	{0,0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0},
	{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

alt_u8 knightSprite[19][19] = {

		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,2,2,0,2,2,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,2,1,1,2,1,2,0,0,0,0,0},
		{0,0,0,0,0,0,2,1,1,1,1,1,1,1,2,0,0,0,0},
		{0,0,0,0,0,2,1,1,1,1,1,2,1,1,2,0,0,0,0},
		{0,0,0,0,2,1,1,2,1,1,1,1,2,1,1,2,0,0,0},
		{0,0,0,0,2,1,1,1,1,1,1,1,1,2,1,2,0,0,0},
		{0,0,0,2,1,1,1,2,2,1,1,1,1,2,1,2,0,0,0},
		{0,0,2,1,1,1,2,0,2,1,1,1,1,2,1,2,0,0,0},
		{0,0,2,1,1,2,0,0,2,1,1,1,1,2,1,2,0,0,0},
		{0,0,2,2,2,0,0,2,1,1,1,1,1,2,1,2,0,0,0},
		{0,0,0,0,0,0,2,1,1,1,1,1,1,2,1,2,0,0,0},
		{0,0,0,0,0,2,1,1,1,1,1,1,1,2,1,2,0,0,0},
		{0,0,0,0,2,1,1,1,1,1,1,1,1,2,1,2,0,0,0},
		{0,0,0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

alt_u8 rookSprite[19][19] = {

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,2,2,2,0,2,2,2,0,2,2,2,0,0,0,0},
	{0,0,0,0,2,1,2,2,2,1,2,2,2,1,2,0,0,0,0},
	{0,0,0,0,2,1,1,1,1,1,1,1,1,1,2,0,0,0,0},
	{0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0,0},
	{0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0},
	{0,0,0,0,2,1,1,1,1,1,1,1,1,1,1,2,0,0,0},
	{0,0,0,0,2,1,1,1,1,1,1,1,1,1,1,2,0,0,0},
	{0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};


alt_u8 whitesmove[19][19] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2},
		{0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0},
		{0,0,2,2,0,0,0,0,0,2,0,0,0,0,0,2,2,0,0},
		{0,0,0,2,2,0,0,0,2,2,2,0,0,0,2,2,0,0,0},
		{0,0,0,0,2,2,0,2,2,0,2,2,0,2,2,0,0,0,0},
		{0,0,0,0,0,2,2,2,0,0,0,2,2,2,0,0,0,0,0},
		{0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0},
		{0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0},
		{0,0,1,0,1,0,1,1,1,0,0,1,0,0,1,1,1,0,0}
};

alt_u8 blacksmove[19][19] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,2,0,0,0,2,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,2,0,0,0,2,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,2,0,0,0,2,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,2,0,0,0,2,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0},
		{0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0},
		{0,0,1,0,1,0,1,1,1,0,0,1,0,0,1,1,1,0,0}
};

alt_u8 checkSprite[19][19] = {

		{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0}
};


alt_u8 menuSprite[19][19] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,0},
		{0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0},
		{0,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,0},
		{0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0},
		{0,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,0},
		{0,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0},
		{0,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
alt_u8 restartSprite[19][19] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,2,1,1,2,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,1,1,1,1,2,0,0,0,0,0,0},
		{0,0,0,0,0,0,2,1,1,2,2,1,1,2,0,0,0,0,0},
		{0,0,0,0,0,2,1,1,2,0,0,2,1,1,2,0,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,2,1,1,2,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,2,1,1,2,0,0,0},
		{0,0,0,2,1,1,1,1,2,0,0,0,2,1,1,2,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,2,1,1,2,0,0,0},
		{0,0,0,0,0,2,2,0,0,0,0,0,2,1,1,2,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,2,1,1,2,0,0,0},
		{0,0,0,0,2,1,1,1,2,0,0,2,1,1,1,2,0,0,0},
		{0,0,0,0,0,2,1,1,1,2,2,1,1,1,2,0,0,0,0},
		{0,0,0,0,0,0,2,1,1,1,1,1,2,2,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,2,1,1,2,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

alt_u8 squareSprite[19][19] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

alt_u8 gameoverSprite[19][19] = {
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
		{1,1,1,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,1},
		{1,2,2,2,2,1,2,2,1,2,1,1,1,1,2,1,2,2,2},
		{1,2,1,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,2},
		{1,2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,2,2},
		{1,1,1,1,2,1,2,2,1,2,1,2,2,1,2,1,1,1,1},
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
		{1,1,1,1,2,1,2,2,1,2,1,1,1,1,2,1,1,1,2},
		{1,2,2,1,2,1,2,2,1,2,1,2,2,2,2,1,2,2,1},
		{1,2,2,1,2,1,2,2,1,2,1,1,1,2,2,1,1,1,2},
		{1,2,2,1,2,1,2,2,1,2,1,2,2,2,2,1,2,1,2},
		{1,1,1,1,2,2,1,1,2,2,1,1,1,1,2,1,2,2,1},
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

alt_u8 colorSprite[19][19] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,1,1,1,1,1,2,0,0,0,0,0},
		{0,0,0,0,0,0,2,1,1,1,1,1,1,1,2,0,0,0,0},
		{0,0,0,0,0,2,1,1,1,2,2,2,2,1,2,0,0,0,0},
		{0,0,0,0,2,1,1,1,2,0,0,0,0,2,0,0,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,2,1,1,2,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,2,1,1,1,2,0,0,0,0,2,0,0,0,0,0},
		{0,0,0,0,0,2,1,1,1,2,2,2,2,1,2,0,0,0,0},
		{0,0,0,0,0,0,2,1,1,1,1,1,1,1,2,0,0,0,0},
		{0,0,0,0,0,0,0,2,2,1,1,1,1,2,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

alt_u8 enterSprite[19][19] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,1,2,0,0},
		{0,0,0,2,0,0,0,0,0,0,0,0,0,2,1,1,2,0,0},
		{0,0,2,1,2,0,0,0,0,0,0,0,0,2,1,1,2,0,0},
		{0,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,0,0},
		{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0},
		{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0},
		{0,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0},
		{0,0,2,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
alt_u8 computerSprite[19][19] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,0,2,0,0,0,0,0,1,1,1,0,0,0,0,0,2,0,0},
		{0,0,2,0,0,0,1,1,1,1,1,1,1,0,0,0,2,0,0},
		{0,0,2,0,0,1,1,1,1,1,1,1,1,1,0,0,2,0,0},
		{0,0,2,0,0,1,1,1,1,1,1,1,1,1,0,0,2,0,0},
		{0,0,2,0,1,1,1,1,1,1,1,1,1,1,1,0,2,0,0},
		{0,0,2,0,1,1,1,1,1,1,1,1,1,1,1,0,2,0,0},
		{0,0,2,0,1,1,1,1,1,1,1,1,1,1,1,0,2,0,0},
		{0,0,2,0,0,1,1,1,1,1,1,1,1,1,0,0,2,0,0},
		{0,0,2,0,0,1,1,1,1,1,1,1,1,1,0,0,2,0,0},
		{0,0,2,0,0,0,1,1,1,1,1,1,1,0,0,0,2,0,0},
		{0,0,2,0,0,0,0,0,1,1,1,0,0,0,0,0,2,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,0,0,0,0,2,0,0,1,1,1,0,0,2,0,0,0,0,0},
		{0,0,0,0,0,2,0,1,1,1,1,1,0,2,0,0,0,0,0},
		{0,0,0,0,0,2,1,1,1,1,1,1,1,2,0,0,0,0,0},
		{0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0},
		{0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0}
};