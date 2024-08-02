#include "game.h"
#include "../vga/vga_color.h"
char default_board[8][8] =  {
								{'r','n','b','q','k','b','n','r'},
								{'p','p','p','p','p','p','p','p'},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{'P','P','P','P','P','P','P','P'},
								{'R','N','B','Q','K','B','N','R'}
							};
char current_board[8][8] = {
								{'r','n','b','q','k','b','n','r'},
								{'p','p','p','p','p','p','p','p'},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{'P','P','P','P','P','P','P','P'},
								{'R','N','B','Q','K','B','N','R'}
							};
char temp_board[8][8] = {
								{'r','n','b','q','k','b','n','r'},
								{'p','p','p','p','p','p','p','p'},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{' ',' ',' ',' ',' ',' ',' ',' '},
								{'P','P','P','P','P','P','P','P'},
								{'R','N','B','Q','K','B','N','R'}
							};

int hand = 0;
int turn = 0; //its whites turn

int gameOver = 0;

int promote = 0;
char promotePiece = ' ';

static int hand_x = -1;
static int hand_y = -1;

int whiteCheck = 0;
int blackCheck = 0;
int checkPiece[2] = {0,0};

int whiteCastle = 1;
int whiteLeftRook = 0;
int whiteRightRook = 0;
int whiteO_O_O = 0;
int whiteO_O = 0;


int blackCastle = 1;
int blackLeftRook = 0;
int blackRightRook = 0;
int blackO_O_O = 0;
int blackO_O = 0;

int promoteX = 0;
int promoteY = 0;


void game_setup(){

	gameOver = 0;

	turn = 0; // make it whites turn
	hand = 0;
	whiteCheck = 0;
	blackCheck = 0;
	checkPiece[0] = 0;
	checkPiece[1] = 0;

	promoteX = 0;
	promoteY = 0;




	whiteCastle = 1;
	whiteLeftRook = 0;
	whiteRightRook = 0;
	whiteO_O_O = 0;
	whiteO_O = 0;

	blackCastle = 1;
	blackLeftRook = 0;
	blackRightRook = 0;
	blackO_O_O = 0;
	blackO_O = 0;


	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			current_board[x][y] = default_board[y][x]; //must be mirrored from y,x to x,y to make more sense
			temp_board[x][y] = default_board[y][x];
		}
	}
}

void pick_up(int x, int y){
	if(hand == 1 || temp_board[x][y] == ' '){
		return;
	}
	if(islower(temp_board[x][y]) != turn){
		return;
	}
	hand_x = x;
	hand_y = y;
	temp_board[x][y] = ' ';
	hand = 1;
	////printf("Hand Piece x: %x, y: %x \n", hand_x, hand_y);
	//printf("Piece Label: %c \n", current_board[x][y]);

}


void put_down(int x, int y){
	if(hand == 0 && promote == 0){ //if nothing in hand
		return;
	}
	if(promote || legalMove(x, y, turn) == 1){

		if(promote == 0){
			temp_board[x][y] = current_board[hand_x][hand_y]; // move the piece
		}

		//check for pawn promotions
		if(promote || ((y == 7 || y == 0) && (current_board[hand_x][hand_y] == 'p' || current_board[hand_x][hand_y] == 'P'))){
			if(promote == 0){
				promoteX = x;
				promoteY = y;
			}
			promote = 1;
			hand = 0;

			if(promote && promotePiece == ' '){
				////printf("promote piece empty keep waiting \n");
				return;
			}

			temp_board[x][y] = promotePiece;
			promote = 0;
			promotePiece = ' ';
		}




		turn = 1 - turn; //change the turn

		//check for castle
		if(whiteO_O_O){
			temp_board[0][7] = ' ';
			temp_board[3][7] = 'R';
			whiteO_O_O = 0;
		}
		if(whiteO_O){
			temp_board[7][7] = ' ';
			temp_board[5][7] = 'R';
			whiteO_O = 0;
		}
		if(blackO_O_O){
			temp_board[0][0] = ' ';
			temp_board[3][0] = 'r';
			blackO_O_O = 0;
		}
		if(blackO_O){
			temp_board[7][0] = ' ';
			temp_board[5][0] = 'r';
			blackO_O = 0;
		}

		for(int i = 0; i < 8; i++){ //copy the board
			for(int j = 0; j < 8; j++){
				current_board[i][j] = temp_board[i][j];
			}
		}
		//figure out of the player is in check this move
		int whiteCheckPiece[2] = {0,0};
		whiteCheck = (inCheck(0) != 0);
		whiteCheckPiece[0] = checkPiece[0];
		whiteCheckPiece[1] = checkPiece[1];

		blackCheck = (inCheck(1) != 0);

		if(whiteCheck){
			checkPiece[0] = whiteCheckPiece[0];
			checkPiece[1] = whiteCheckPiece[1];;
			if(inCheckmate(0)){
				gameOver = 1;
				//printf("gameover");
			}
		}
		if(blackCheck){
			if(inCheckmate(1)){
				gameOver = 1;
				//printf("gameover");
			}
		}
	}
	else{
		temp_board[hand_x][hand_y] = current_board[hand_x][hand_y];
	}
	hand = 0;
	hand_x = 0;
	hand_y = 0;
}

int legalMove(int x, int y, int color){
	if(x == hand_x && y == hand_y){ // if the piece is put back
		return 0; // its illegal move
	}
	char handPiece = current_board[hand_x][hand_y]; //piece in your hand
	//printf("handpiece = %c \n", handPiece);
	int capture = (current_board[x][y] != ' ');


	if(capture && islower(handPiece) == islower(current_board[x][y])){ // if you try to capture urself
		return 0;
	}

	//white pawn logic
	if(handPiece == 'P'){
		if(capture){
			if(!(y == hand_y - 1 && (x == hand_x + 1 || x == hand_x - 1))){
				return 0;
			}
		}
		else if(!(((y == 4 && hand_y == 6)||(y == hand_y - 1)) && x == hand_x)){
			return 0;
		}
		else if(y == 4 && hand_y == 6){ //make sure the pawn doesnt jump
			if(current_board[x][5] != ' '){
				return 0;
			}
		}
	}
	//black pawn logic
	else if(handPiece == 'p'){
		if(capture){ //make sure the captures are diagnal
			if(!(y == hand_y + 1 && (x == hand_x + 1 || x == hand_x - 1))){
				return 0;
			}
		}
		//make sure move is legal
		else if(!(((y == 3 && hand_y == 1)||(y == hand_y + 1)) && x == hand_x)){
			return 0;
		}
		else if(y == 3 && hand_y == 1){ //make sure the pawn doesnt jump
			if(current_board[x][2] != ' '){
				return 0;
			}
		}
	}


	//rook
	else if(handPiece == 'R' || handPiece == 'r'){
		if(hand_x != x && hand_y != y){ //the piece has to stay on one of the same rows
			return 0;
		}

		//check for jump
		else if(hand_y == y){
			int dir = ((x - hand_x) > 0) - ((x - hand_x) < 0); //return 1 or -1 depending on the direction
			for(int i = 1; i < abs(x - hand_x); i++){
				if(current_board[hand_x+(i*dir)][y] != ' '){
					return 0;
				}
			}
		}
		else if(hand_x == x){
			int dir = ((y - hand_y) > 0) - ((y - hand_y) < 0); //return 1 or -1 depending on the direction
			for(int i = 1; i < abs(y - hand_y); i++){
				if(current_board[x][hand_y + (i*dir)] != ' '){
					return 0;
				}
			}
		}
		if(handPiece == 'R' && hand_x == 0 && hand_y == 7){
			whiteLeftRook = 1;
		}
		if(handPiece == 'R' && hand_x == 7 && hand_y == 7){
			whiteRightRook = 1;
		}
		if(handPiece == 'r' && hand_x == 0 && hand_y == 0){
			blackLeftRook = 1;
		}
		if(handPiece == 'r' && hand_x == 7 && hand_y == 0){
			blackRightRook = 1;
		}
	}

	//knight logic
	else if(handPiece == 'N' || handPiece == 'n'){
		if(!( (( (x == hand_x + 1) || (x == hand_x - 1) ) && ( (y == hand_y + 2) || (y == hand_y - 2)) )
		   || (( (y == hand_y + 1) || (y == hand_y - 1) ) && ( (x == hand_x + 2) || (x == hand_x - 2)) )
		)){
			return 0;
		}
	}

	//king logic
	else if(handPiece == 'K' || handPiece == 'k'){

		//check for castle long as white
		if((handPiece == 'K') && (whiteCastle == 1)
		&& (hand_y == 7) && (y == 7) && (x == 2) && (hand_x == 4)
		&& (temp_board[1][7] == ' ') && (temp_board[2][7] == ' ')
		&& (temp_board[3][7] == ' ')){
			//if in check at x==4
			if(whiteCheck){
				return 0;
			}
			//if in check at x==3
			if(moveIntoCheck(3, 7, color)){
				return 0;
			}
			if(whiteLeftRook){
				return 0;
			}
			whiteCastle = 0;
			whiteLeftRook = 1;
			whiteO_O_O = 1;
		}

		//check for castle short as white
		else if((handPiece == 'K') && (whiteCastle == 1)
		&& (hand_y == 7) && (y == 7) && (x == 6) && (hand_x == 4)
		&& (temp_board[6][7] == ' ') && (temp_board[5][7] == ' ')){
			if(whiteCheck){
				return 0;
			}
			if(moveIntoCheck(5, 7, color)){
				return 0;
			}
			if(whiteRightRook){
				return 0;
			}
			whiteRightRook = 1;
			whiteO_O = 1;
		}

		//check for castle long as black
		else if((handPiece == 'k') && (blackCastle == 1)
		&& (hand_y == 0) && (y == 0) && (x == 2) && (hand_x == 4)
		&& (temp_board[1][0] == ' ') && (temp_board[2][0] == ' ')
		&& (temp_board[3][0] == ' ')){
			//if in check at x==4
			if(blackCheck){
				return 0;
			}
			//if in check at x==3
			if(moveIntoCheck(3, 0, color)){
				return 0;
			}
			if(blackLeftRook){
				return 0;
			}
			blackCastle = 0;
			blackLeftRook = 1;
			blackO_O_O = 1;
		}

		//check for castle short as black
		else if((handPiece == 'k') && (blackCastle == 1)
		&& (hand_y == 0) && (y == 0) && (x == 6) && (hand_x == 4)
		&& (temp_board[6][0] == ' ') && (temp_board[5][0] == ' ')){
			if(blackCheck){
				return 0;
			}
			if(moveIntoCheck(5, 0, color)){
				return 0;
			}
			if(blackRightRook){
				return 0;
			}
			blackRightRook = 1;
			blackO_O = 1;
		}


		else if(!( ((x == hand_x + 1) || (x == hand_x - 1) || (x == hand_x))
		  &&  ((y == hand_y + 1) || (y == hand_y - 1) || (y == hand_y))
		)){
			return 0;
		}

		if(handPiece == 'K'){
			whiteCastle = 0;
		}
		if(handPiece == 'k'){
			blackCastle = 0;
		}
		//printf("king move is legal \n");
	}

	//bishop logic
	else if(handPiece == 'B' || handPiece == 'b'){
		int addVal = hand_x + hand_y;
		int subVal = hand_x - hand_y;
		if(!(x-y == subVal || x+y == addVal)){
			return 0;
		}
		//check for jumps
		else if(x-y == subVal){
			int dir = ((y - hand_y) > 0) - ((y - hand_y) < 0);
			for(int i = 1; i < abs(y - hand_y); i++){
				if(current_board[hand_x+(i*dir)][hand_y+(i*dir)] != ' '){
					return 0;
				}
			}
		}
		else if(x+y == addVal){
			int dir = ((y - hand_y) > 0) - ((y - hand_y) < 0);
			for(int i = 1; i < abs(y - hand_y); i++){
				if(current_board[hand_x-(i*dir)][hand_y+(i*dir)] != ' '){
					return 0;
				}
			}
		}
	}

	//queen logic
	else if(handPiece == 'Q' || handPiece == 'q'){
		int addVal = hand_x + hand_y;
		int subVal = hand_x - hand_y;

		if(!( (x-y == subVal || x+y == addVal)
		   || (hand_x == x || hand_y == y)
		)){
			return 0;
		}
		//check for jumps
		else if(x-y == subVal){
			int dir = ((y - hand_y) > 0) - ((y - hand_y) < 0);
			for(int i = 1; i < abs(y - hand_y); i++){
				if(current_board[hand_x+(i*dir)][hand_y+(i*dir)] != ' '){
					return 0;
				}
			}
		}
		else if(x+y == addVal){
			int dir = ((y - hand_y) > 0) - ((y - hand_y) < 0);
			for(int i = 1; i < abs(y - hand_y); i++){
				if(current_board[hand_x-(i*dir)][hand_y+(i*dir)] != ' '){
					return 0;
				}
			}
		}
		else if(hand_y == y){
			int dir = ((x - hand_x) > 0) - ((x - hand_x) < 0); //return 1 or -1 depending on the direction
			for(int i = 1; i < abs(x - hand_x); i++){
				if(current_board[hand_x+(i*dir)][y] != ' '){
					return 0;
				}
			}
		}
		else if(hand_x == x){
			int dir = ((y - hand_y) > 0) - ((y - hand_y) < 0); //return 1 or -1 depending on the direction
			for(int i = 1; i < abs(y - hand_y); i++){
				if(current_board[x][hand_y + (i*dir)] != ' '){
					return 0;
				}
			}
		}
	}
	//printf("checking move into check at %d,%d \n", x,y);
	if(moveIntoCheck(x,y,color)){
		//printf("move into check is true \n");
		return 0;
	}
	//printf("move into check is false \n");
	return 1;
}

int moveIntoCheck(int x, int y, int color){
	//printf("checking move into check from %d,%d to %d,%d \n", hand_x, hand_y, x, y);
	temp_board[x][y] = current_board[hand_x][hand_y]; //move the piece
	int checkPieceTemp[2] = {checkPiece[0], checkPiece[1]}; //backup the checking piece




	char returnVal = inCheck(color); //check if there is a check going on
	//printf("move into check val: %c \n", returnVal);
	temp_board[x][y] = current_board[x][y]; //set the board back

	checkPiece[0] = checkPieceTemp[0];
	checkPiece[1] = checkPieceTemp[1]; //reset to previous checking piece values

	if(returnVal == 0){
		//printf("not moving into check \n");
		return 0;
	}
	else{
		return 1;
	}
}

//checks temp board to see if the king is in check
char inCheck(int color){
	char kingChar;
	int kingX;
	int kingY;

	if(color == 0){
		kingChar = 'K';
	}
	else{
		kingChar = 'k';
	}

	for(int x = 0; x < 8; x++){
		for(int y = 0; y < 8; y++){
			if(temp_board[x][y] == kingChar){
				kingX = x;
				kingY = y;
			}
		}
	}

	//left of king
	for(int i = kingX - 1; i >= 0; i--){
		char cur = temp_board[i][kingY];
		checkPiece[0] = i;
		checkPiece[1] = kingY;

		if(temp_board[i][kingY] != ' '){
			if(kingX - 1 == i){ //check for king on first move
				if(color == 0){ //if u are white check for black king
					if(temp_board[i][kingY] == 'k'){
						return cur;
					}
				}
				else{ //otherwise check for white king
					if(temp_board[i][kingY] == 'K'){
						return cur;
					}
				}
			}
			if(color == 0){ //if u are white check for black king
				if(temp_board[i][kingY] == 'r' || temp_board[i][kingY] == 'q'){
					return cur;
				}
				else{
					break; //if its not one of the pieces then the row is clear
				}
			}
			else{ //otherwise check for white king
				if(temp_board[i][kingY] == 'R' || temp_board[i][kingY] == 'Q'){
					return cur;
				}
				else{
					break; //if its not one of the pieces then the row is clear
				}
			}
		}
	}
	//right of king
	for(int i = kingX + 1; i < 8; i++){
		char cur = temp_board[i][kingY];
		checkPiece[0] = i;
		checkPiece[1] = kingY;

		if(temp_board[i][kingY] != ' '){
			if(kingX + 1 == i){ //check for king on first move
				if(color == 0){ //if u are white check for black king
					if(temp_board[i][kingY] == 'k'){
						return cur;
					}
				}
				else{ //otherwise check for white king
					if(temp_board[i][kingY] == 'K'){
						return cur;
					}
				}
			}
			if(color == 0){ //check for other pieces
				if(temp_board[i][kingY] == 'r' || temp_board[i][kingY] == 'q'){
					return cur;
				}
				else{
					break; //if its not one of the pieces then the row is clear
				}
			}
			else{ //check for other pieces
				if(temp_board[i][kingY] == 'R' || temp_board[i][kingY] == 'Q'){
					return cur;
				}
				else{
					break; //if its not one of the pieces then the row is clear
				}
			}
		}
	}
	//up from king
	for(int i = kingY - 1; i >= 0; i--){
		char cur = temp_board[kingX][i];
		checkPiece[0] = kingX;
		checkPiece[1] = i;

		if(temp_board[kingX][i] != ' '){
			if(kingY - 1 == i){ //check for king on first move
				if(color == 0){ //if u are white check for black king
					if(temp_board[kingX][i] == 'k'){
						return cur;
					}
				}
				else{ //otherwise check for white king
					if(temp_board[kingX][i] == 'K'){
						return cur;
					}
				}
			}
			if(color == 0){ //check for other pieces
				if(temp_board[kingX][i] == 'r' || temp_board[kingX][i] == 'q'){
					return cur;
				}
				else{
					break; //if its not one of the pieces then the row is clear
				}
			}
			else{ //check for other pieces
				if(temp_board[kingX][i] == 'R' || temp_board[kingX][i] == 'Q'){
					return cur;
				}
				else{
					break; //if its not one of the pieces then the row is clear
				}
			}
		}
	}
	//down from king
	for(int i = kingY + 1; i < 8; i++){
		char cur = temp_board[kingX][i];
		checkPiece[0] = kingX;
		checkPiece[1] = i;

		if(cur != ' '){
			if(kingY + 1 == i){ //check for king on first move
				if(color == 0){ //if u are white check for black king
					if(cur == 'k'){
						return cur;
					}
				}
				else{ //otherwise check for white king
					if(cur == 'K'){
						return cur;
					}
				}
			}
			if(color == 0){ //check for other pieces
				if(cur == 'r' || cur == 'q'){
					return cur;
				}
				else{
					break; //if its not one of the pieces then the row is clear
				}
			}
			else{ //check for other pieces
				if(cur == 'R' || cur == 'Q'){
					return cur;
				}
				else{
					break; //if its not one of the pieces then the row is clear
				}
			}
		}
	}
	//check for black knight
	if(color == 0){
		if((kingX - 1 >= 0)&&((kingY - 2 >= 0))){
			checkPiece[0] = kingX - 1;
			checkPiece[1] = kingY - 2;

			if(temp_board[kingX - 1][kingY - 2] == 'n'){return 'n';}
		}
		if((kingX - 2 >= 0)&&((kingY - 1 >= 0))){
			checkPiece[0] = kingX - 2;
			checkPiece[1] = kingY - 1;

			if(temp_board[kingX - 2][kingY - 1] == 'n'){return 'n';}
		}
		if((kingX - 1 >= 0) &&((kingY + 2 < 8))){
			checkPiece[0] = kingX - 1;
			checkPiece[1] = kingY + 2;

			if(temp_board[kingX - 1][kingY + 2] == 'n'){return 'n';}
		}
		if((kingX - 2 >= 0) &&((kingY + 1 < 8))){
			checkPiece[0] = kingX - 2;
			checkPiece[1] = kingY + 1;

			if(temp_board[kingX - 1][kingY + 1] == 'n'){return 'n';}
		}
		if((kingX + 1 < 8) && ((kingY + 2 < 8))){
			checkPiece[0] = kingX + 1;
			checkPiece[1] = kingY + 2;

			if(temp_board[kingX + 1][kingY + 2] == 'n'){return 'n';}
		}
		if((kingX + 2 < 8) && ((kingY + 1 < 8))){
			checkPiece[0] = kingX + 2;
			checkPiece[1] = kingY + 1;

			if(temp_board[kingX + 2][kingY + 1] == 'n'){return 'n';}
		}
		if((kingX + 1 < 8) &&((kingY - 2 >= 0))){
			checkPiece[0] = kingX + 1;
			checkPiece[1] = kingY - 2;

			if(temp_board[kingX + 1][kingY - 2] == 'n'){return 'n';}
		}
		if((kingX + 2 < 8) &&((kingY - 1 >= 0))){
			checkPiece[0] = kingX + 2;
			checkPiece[1] = kingY - 1;

			if(temp_board[kingX + 2][kingY - 1] == 'n'){return 'n';}
		}
	}
	else{
		if((kingX - 1 >= 0)&&((kingY - 2 >= 0))){
			checkPiece[0] = kingX - 1;
			checkPiece[1] = kingY - 2;

			if(temp_board[kingX - 1][kingY - 2] == 'N'){return 'N';}
		}
		if((kingX - 2 >= 0)&&((kingY - 1 >= 0))){
			checkPiece[0] = kingX - 2;
			checkPiece[1] = kingY - 1;

			if(temp_board[kingX - 2][kingY - 1] == 'N'){return 'N';}
		}
		if((kingX - 1 >= 0) &&((kingY + 2 < 8))){
			checkPiece[0] = kingX - 2;
			checkPiece[1] = kingY - 1;

			if(temp_board[kingX - 1][kingY + 2] == 'N'){return 'N';}
		}
		if((kingX - 2 >= 0) &&((kingY + 1 < 8))){
			checkPiece[0] = kingX - 2;
			checkPiece[1] = kingY + 1;

			if(temp_board[kingX - 1][kingY + 1] == 'N'){return 'N';}
		}
		if((kingX + 1 < 8) && ((kingY + 2 < 8))){
			checkPiece[0] = kingX + 1;
			checkPiece[1] = kingY + 2;

			if(temp_board[kingX + 1][kingY + 2] == 'N'){return 'N';}
		}
		if((kingX + 2 < 8) && ((kingY + 1 < 8))){
			checkPiece[0] = kingX + 2;
			checkPiece[1] = kingY + 1;

			if(temp_board[kingX + 2][kingY + 1] == 'N'){return 'N';}
		}
		if((kingX + 1 < 8) &&((kingY - 2 >= 0))){
			checkPiece[0] = kingX + 1;
			checkPiece[1] = kingY - 2;

			if(temp_board[kingX + 1][kingY - 2] == 'N'){return 'N';}
		}
		if((kingX + 2 < 8) &&((kingY - 1 >= 0))){
			checkPiece[0] = kingX + 2;
			checkPiece[1] = kingY - 1;

			if(temp_board[kingX + 2][kingY - 1] == 'N'){return 'N';}
		}
	}


	//check diagnals
	if(color == 0){
		int upleft = 1;
		int upright = 1;
		int downleft = 1;
		int downright = 1;

		for(int i = 1; i < 8; i++){
			//check directions
			if(((kingX + i < 8) && (kingY + i < 8)) && downright){
				char cur = temp_board[kingX + i][kingY + i];
				checkPiece[0] = kingX + i;
				checkPiece[1] = kingY + i;

				if((cur == 'q') || (cur == 'b')){return cur;}
				else if (cur != ' '){downright = 0;}
			}

			if(((kingX - i >= 0) && (kingY + i < 8)) && downleft){
				char cur = temp_board[kingX - i][kingY + i];
				checkPiece[0] = kingX - i;
				checkPiece[1] = kingY + i;

				if((cur == 'q') || (cur == 'b')){return cur;}
				else if (cur != ' '){downleft = 0;}
			}
			if(((kingX - i >= 0) && (kingY - i >= 0)) && upleft){
				char cur = temp_board[kingX - i][kingY - i];
				checkPiece[0] = kingX - i;
				checkPiece[1] = kingY - i;

				if((i == 1)&&(cur == 'p')){return cur;}
				if((cur == 'q') || (cur == 'b')){return cur;}
				else if (cur != ' '){upleft = 0;}
			}
			if(((kingX + i < 8) && (kingY - i >= 0)) && upright){
				char cur = temp_board[kingX + i][kingY - i];
				checkPiece[0] = kingX + i;
				checkPiece[1] = kingY - i;

				if((i == 1)&&(cur == 'p')){return cur;}
				if((cur == 'q') || (cur == 'b')){return cur;}
				else if (cur != ' '){upright = 0;}
			}
		}
	}
	else{
		int upleft = 1;
		int upright = 1;
		int downleft = 1;
		int downright = 1;

		for(int i = 1; i < 8; i++){
			//check directions
			if(((kingX + i < 8) && (kingY + i < 8)) && downright){
				char cur = temp_board[kingX + i][kingY + i];
				checkPiece[0] = kingX + i;
				checkPiece[1] = kingY + i;

				if((i == 1)&&(cur == 'P')){return cur;}
				if((cur == 'Q') || (cur == 'B')){return cur;}
				else if (cur != ' '){downright = 0;}
			}

			if(((kingX - i >= 0) && (kingY + i < 8)) && downleft){
				char cur = temp_board[kingX - i][kingY + i];
				checkPiece[0] = kingX - i;
				checkPiece[1] = kingY + i;

				if((i == 1)&&(cur == 'P')){return cur;}
				if((cur == 'Q') || (cur == 'B')){return cur;}
				else if (cur != ' '){downleft = 0;}
			}
			if(((kingX - i >= 0) && (kingY - i >= 0)) && upleft){
				char cur = temp_board[kingX - i][kingY - i];
				checkPiece[0] = kingX - i;
				checkPiece[1] = kingY - i;

				if((cur == 'Q') || (cur == 'B')){return cur;}
				else if (cur != ' '){upleft = 0;}
			}
			if(((kingX + i < 8) && (kingY - i >= 0)) && upright){
				char cur = temp_board[kingX + i][kingY - i];
				checkPiece[0] = kingX + i;
				checkPiece[1] = kingY - i;

				if((cur == 'Q') || (cur == 'B')){return cur;}
				else if (cur != ' '){upright = 0;}
			}
		}
	}
	checkPiece[0] = 0;
	checkPiece[1] = 0;
	return 0;
}

int inCheckmate(int color){
	int tempX = hand_x;
	int tempY = hand_y;
	char kingChar;
	char checkChar = inCheck(color);

	if(color == 0){
		kingChar = 'K';
	}
	else{
		kingChar = 'k';
	}
	//printf("checking for checkmate \n");

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(temp_board[i][j] == kingChar){
				temp_board[i][j] = ' ';
				hand_x = i;
				hand_y = j;
			}
		}
	}
	//first check if king has any legal moves
	//if king can move not in checkmate
	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			if(!(hand_x + i < 0 || hand_x + i >= 8 || hand_y + j < 0 || hand_y + j >= 8)){
				if(legalMove(hand_x + i, hand_y + j, color)){
					//printf("king char = %c, king x,y = %d,%d \n", kingChar, hand_x, hand_y);
					//printf("not mated bc legal king move \n");
					//printf("legal move is %d, %d \n", hand_x + i, hand_y + j);
					temp_board[hand_x][hand_y] = kingChar;
					return 0;
				}
			}
		}
	}



	//then find piece(s) delivering check
	//first replace the piece with a pawn of same team
	//then see if the king is still in check
	int checkPieceTemp[2] = {checkPiece[0], checkPiece[1]};

	if(color == 0){
		//first see if the piece delivering check was taken if there is still checkmate
		temp_board[checkPiece[0]][checkPiece[1]] = 'P';
		temp_board[hand_x][hand_y] = 'K'; //put king back for check

		if(inCheck(color) != 0){
			//printf("mated by double check without a king move \n");
			//put the piece back
			checkPiece[0] = checkPieceTemp[0];
			checkPiece[1] = checkPieceTemp[1];
			temp_board[checkPiece[0]][checkPiece[1]] = checkChar;
			return 1; //if its still in check then its checkmate
		}

		checkPiece[0] = checkPieceTemp[0];
		checkPiece[1] = checkPieceTemp[1];

		//first remove the kings off the board then
		//replace the checking piece with a king of the opposite color and
		//see if their king is in check, meaning that the piece
		//can be captured

		int oppKing_x;
		int oppKing_y;
		//find the opposing king
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
				if(temp_board[i][j] == 'k'){
					oppKing_x = i;
					oppKing_y = j;
				}
			}
		}

		temp_board[hand_x][hand_y] = ' '; //remove your king bc it cant capture
		temp_board[oppKing_x][oppKing_y] = ' '; //hide the other king
		temp_board[checkPiece[0]][checkPiece[1]] = 'k'; //set the piece delivering check to a king
		if(inCheck(1)){ //if their new king can be capture then its not checkmate
			checkPiece[0] = checkPieceTemp[0];
			checkPiece[1] = checkPieceTemp[1];
			temp_board[checkPiece[0]][checkPiece[1]] = checkChar;
			temp_board[oppKing_x][oppKing_y] = 'k';
			temp_board[hand_x][hand_y] = 'K';
			//printf("out of check by capture \n");
			return 0;
		}

		checkPiece[0] = checkPieceTemp[0];
		checkPiece[1] = checkPieceTemp[1];


		//only way left is to block and if its a night you cant block
		if(checkChar == 'n'){
			checkPiece[0] = checkPieceTemp[0];
			checkPiece[1] = checkPieceTemp[1];
			temp_board[checkPiece[0]][checkPiece[1]] = checkChar;
			temp_board[oppKing_x][oppKing_y] = 'k';
			temp_board[hand_x][hand_y] = 'K';
			//printf("no way to block the knight :( \n");
			return 1;
		}

		//return to normal board
		checkPiece[0] = checkPieceTemp[0];
		checkPiece[1] = checkPieceTemp[1];
		temp_board[checkPiece[0]][checkPiece[1]] = checkChar;
		temp_board[oppKing_x][oppKing_y] = 'k';
		temp_board[hand_x][hand_y] = 'K';


		//go through empty squares between
		int xdir = ((checkPiece[0] - hand_x) > 0) - ((checkPiece[0] - hand_x) < 0);
		int ydir = ((checkPiece[1] - hand_y) > 0) - ((checkPiece[1] - hand_y) < 0);
		//printf("horizontal xdir,ydir = %d,%d \n", xdir, ydir);

		for(int i = 1; i < 7; i++){
			//printf("hand_x+(i*xdir) = %d, hand_y+(i*ydir) = %d \n",(hand_x+(i*xdir)),(hand_y+(i*ydir)) );
			if(hand_x+(i*xdir) == checkPiece[0] && hand_y+(i*ydir) == checkPiece[1]){
				//printf("reached piece, nothing to block with\n");
				return 1;
				break;
			}
			if((hand_x+(i*xdir) >= 0 || hand_x+(i*xdir) < 8 ||
				hand_y+(i*ydir) >= 0 || hand_y+(i*ydir) < 8)
			&& canBlock(hand_x+(i*xdir), hand_y+(i*ydir), color)){
				//printf("can block on straight line from piece \n");
				return 0;
			}
		}




	}

	else{
		temp_board[checkPiece[0]][checkPiece[1]] = 'p';
		temp_board[hand_x][hand_y] = 'k'; //put king back for check

		if(inCheck(color) != 0){
			//printf("mated by double check without a king move \n");
			temp_board[checkPiece[0]][checkPiece[1]] = checkChar;
			checkPiece[0] = checkPieceTemp[0];
			checkPiece[1] = checkPieceTemp[1];
			return 1; //if its still in check then its checkmate
		}

		checkPiece[0] = checkPieceTemp[0];
		checkPiece[1] = checkPieceTemp[1];

		//first remove the kings off the board then
		//replace the checking piece with a king of the opposite color and
		//see if their king is in check, meaning that the piece
		//can be captured

		int oppKing_x;
		int oppKing_y;
		//find the opposing king
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
				if(temp_board[i][j] == 'k'){
					oppKing_x = i;
					oppKing_y = j;
				}
			}
		}

		temp_board[hand_x][hand_y] = ' '; //remove your king bc it cant capture
		temp_board[oppKing_x][oppKing_y] = ' '; //hide the other king
		temp_board[checkPiece[0]][checkPiece[1]] = 'K'; //set the piece delivering check to a king
		if(inCheck(1 - color)){ //if their new king can be capture then its not checkmate
			checkPiece[0] = checkPieceTemp[0];
			checkPiece[1] = checkPieceTemp[1];
			temp_board[checkPiece[0]][checkPiece[1]] = checkChar;
			temp_board[oppKing_x][oppKing_y] = 'K';
			temp_board[hand_x][hand_y] = 'k';
			//printf("out of check by capture \n");
			return 0;
		}

		checkPiece[0] = checkPieceTemp[0];
		checkPiece[1] = checkPieceTemp[1];


		//only way left is to block and if its a night you cant block
		if(checkChar == 'N'){
			checkPiece[0] = checkPieceTemp[0];
			checkPiece[1] = checkPieceTemp[1];
			temp_board[checkPiece[0]][checkPiece[1]] = checkChar;
			temp_board[oppKing_x][oppKing_y] = 'K';
			temp_board[hand_x][hand_y] = 'k';
			//printf("no way to block the knight :( \n");
			return 1;
		}

		//return to normal board
		checkPiece[0] = checkPieceTemp[0];
		checkPiece[1] = checkPieceTemp[1];
		temp_board[checkPiece[0]][checkPiece[1]] = checkChar;
		temp_board[oppKing_x][oppKing_y] = 'K';
		temp_board[hand_x][hand_y] = 'k';


		//go through empty squares between
		int xdir = ((checkPiece[0] - hand_x) > 0) - ((checkPiece[0] - hand_x) < 0);
		int ydir = ((checkPiece[1] - hand_y) > 0) - ((checkPiece[1] - hand_y) < 0);
		//printf("horizontal xdir,ydir = %d,%d \n", xdir, ydir);

		for(int i = 1; i < 7; i++){
			//printf("hand_x+(i*xdir) = %d, hand_y+(i*ydir) = %d \n",(hand_x+(i*xdir)),(hand_y+(i*ydir)) );
			if(hand_x+(i*xdir) == checkPiece[0] && hand_y+(i*ydir) == checkPiece[1]){
				//printf("reached piece, nothing to block with\n");
				return 1;
				break;
			}
			if((hand_x+(i*xdir) >= 0 || hand_x+(i*xdir) < 8 ||
				hand_y+(i*ydir) >= 0 || hand_y+(i*ydir) < 8)
			&& canBlock(hand_x+(i*xdir), hand_y+(i*ydir), color)){
				//printf("can block on straight line from piece \n");
				return 0;
			}
		}
	}

	//the other piece then if not then checkmate
	//printf("error occured when trying to find way out of check \n");
	checkPiece[0] = checkPieceTemp[0];
	checkPiece[1] = checkPieceTemp[1];
	hand_x = tempX;
	hand_y = tempY;
	return 1;
}

//see if color can block square x,y
int canBlock(int x, int y, int color){
	//printf("checking can block function \n");
	if(color == 0){
		//check left
		for(int i = x - 1; i >= 0; i--){
			if(temp_board[i][y] == 'R' || temp_board[i][y] == 'Q'){
				//printf("can block with rook or queen to left \n");
				return 1;
			}
			if(temp_board[i][y] != ' '){
				//printf("no pieces to left to block with \n");
				break;
			}
		}
		//check right
		for(int i = x + 1; i < 8; i++){
			if(temp_board[i][y] == 'R' || temp_board[i][y] == 'Q'){
				//printf("can block with rook or queen to right \n");
				return 1;
			}
			if(temp_board[i][y] != ' '){
				//printf("no pieces to right to block with \n");
				break;
			}
		}
		//check down
		for(int i = y - 1; i >= 0; i--){
			if(i == y - 1 && temp_board[x][i] == 'P'){
				//printf("can block with pawn below 1\n");
				return 1;
			}
			if(i == y - 2 && temp_board[x][i] == 'P' && i == 6){
				//printf("can block with pawn below 2\n");
				return 1;
			}
			if(temp_board[x][i] == 'R' || temp_board[x][i] == 'Q'){
				//printf("can block with rook or queen below \n");
				return 1;
			}
			if(temp_board[x][i] != ' '){
				//printf("no pieces below to block with \n");
				break;
			}
		}
		//check up
		for(int i = y + 1; i < 8; i++){
			if(temp_board[x][i] == 'R' || temp_board[x][i] == 'Q'){
				//printf("can block with rook or queen up \n");
				return 1;
			}
			if(temp_board[x][i] != ' '){
				//printf("no pieces up to block with \n");
				break;
			}
		}
		//check up-left
		for(int i = 0; i < 8; i++){
			if(x - i < 0 || y - i < 0){
				//printf("no pieces to the upleft to block with \n");
				break;
			}
			if(temp_board[x-i][y-i] == 'B' || temp_board[x-i][y-i] == 'Q'){
				//printf("can block with rook or queen to the upleft \n");
				return 1;
			}
			if(temp_board[x-i][y-i] != ' '){
				//printf("no pieces to the upleft to block with \n");
				break;
			}
		}
		//check up right
		for(int i = 0; i < 8; i++){
			if(x + i >= 8 || y - i < 0){
				//printf("no pieces to the upright to block with \n");
				break;
			}
			if(temp_board[x+i][y-i] == 'B' || temp_board[x+i][y-i] == 'Q'){
				//printf("can block with rook or queen to the upleft \n");
				return 1;
			}
			if(temp_board[x+i][y-i] != ' '){
				//printf("no pieces to the upleft to block with \n");
				break;
			}
		}
		//check to down left
		for(int i = 0; i < 8; i++){
			if(x - i < 0 || y + i >= 8){
				//printf("no pieces to the down left to block with \n");
				break;
			}
			if(temp_board[x-i][y+i] == 'B' || temp_board[x-i][y+i] == 'Q'){
				//printf("can block with rook or queen to the down left \n");
				return 1;
			}
			if(temp_board[x-i][y+i] != ' '){
				//printf("no pieces to the down left to block with \n");
				break;
			}
		}
		//check to down right
		for(int i = 0; i < 8; i++){
			if(x + i >= 8 || y + i >= 8){
				//printf("no pieces to the down right to block with \n");
				break;
			}
			if(temp_board[x+i][y+i] == 'B' || temp_board[x+i][y+i] == 'Q'){
				//printf("can block with rook or queen to the down right \n");
				return 1;
			}
			if(temp_board[x+i][y+i] != ' '){
				//printf("no pieces to the down right to block with \n");
				break;
			}
		}

		//check for knight
		for(int i = -2; i <= 2; i++){
			for(int j = -2; j <= 2; j++){
				if((abs(i) != abs(j))
				&&(i != 0 && j != 0)
				&&(x+i >= 0 && x+i < 8 && y+j >= 0 && y+j < 8)){
					//printf("i = %d, j = %d \n", i, j);
					if(temp_board[x+i][y+j] == 'N'){
						//printf("can block with a knight");
						return 1;
					}
				}
			}
		}

	}
	else{
		//check left
		for(int i = x - 1; i >= 0; i--){
			if(temp_board[i][y] == 'r' || temp_board[i][y] == 'q'){
				//printf("can block with rook or queen to left \n");
				return 1;
			}
			if(temp_board[i][y] != ' '){
				//printf("no pieces to left to block with \n");
				break;
			}
		}
		//check right
		for(int i = x + 1; i < 8; i++){
			if(temp_board[i][y] == 'r' || temp_board[i][y] == 'q'){
				//printf("can block with rook or queen to righg \n");
				return 1;
			}
			if(temp_board[i][y] != ' '){
				//printf("no pieces to right to block with \n");
				break;
			}
		}
		//check up
		for(int i = y - 1; i >= 0; i--){
			if(i == y - 1 && temp_board[x][i] == 'p'){
				//printf("can block with pawn above 1 \n");
				return 1;
			}
			if(i == y - 2 && temp_board[x][i] == 'p' && i == 1){
				//printf("can block with pawn above 2 \n");
				return 1;
			}
			if(temp_board[x][i] == 'r' || temp_board[x][i] == 'q'){
				//printf("can block with rook or queen above\n");
				return 1;
			}
			if(temp_board[x][i] != ' '){
				//printf("no pieces to the left to block with \n");
				break;
			}
		}
		//check down
		for(int i = y + 1; i < 8; i++){
			if(temp_board[x][i] == 'r' || temp_board[x][i] == 'q'){
				//printf("can block with rook or queen below \n");
				return 1;
			}
			if(temp_board[x][i] != ' '){
				//printf("no pieces to below to block with \n");
				break;
			}
		}
		//check up-left
		for(int i = 0; i < 8; i++){
			if(x - i < 0 || y - i < 0){
				//printf("no pieces to the upleft to block with \n");
				break;
			}
			if(temp_board[x-i][y-i] == 'b' || temp_board[x-i][y-i] == 'q'){
				//printf("can block with rook or queen to the upleft \n");
				return 1;
			}
			if(temp_board[x-i][y-i] != ' '){
				//printf("no pieces to the upleft to block with \n");
				break;
			}
		}
		//check up right
		for(int i = 0; i < 8; i++){
			if(x + i >= 8 || y - i < 0){
				//printf("no pieces to the upright to block with \n");
				break;
			}
			if(temp_board[x+i][y-i] == 'b' || temp_board[x+i][y-i] == 'q'){
				//printf("can block with rook or queen to the upleft \n");
				return 1;
			}
			if(temp_board[x+i][y-i] != ' '){
				//printf("no pieces to the upleft to block with \n");
				break;
			}
		}
		//check to down left
		for(int i = 0; i < 8; i++){
			if(x - i < 0 || y + i >= 8){
				//printf("no pieces to the down left to block with \n");
				break;
			}
			if(temp_board[x-i][y+i] == 'b' || temp_board[x-i][y+i] == 'q'){
				//printf("can block with rook or queen to the down left \n");
				return 1;
			}
			if(temp_board[x-i][y+i] != ' '){
				//printf("no pieces to the down left to block with \n");
				break;
			}
		}
		//check to down right
		for(int i = 0; i < 8; i++){
			if(x + i >= 8 || y + i >= 8){
				//printf("no pieces to the down right to block with \n");
				break;
			}
			if(temp_board[x+i][y+i] == 'b' || temp_board[x+i][y+i] == 'q'){
				//printf("can block with rook or queen to the down right \n");
				return 1;
			}
			if(temp_board[x+i][y+i] != ' '){
				//printf("no pieces to the down right to block with \n");
				break;
			}
		}

		//check for knight
		for(int i = -2; i <= 2; i++){
			for(int j = -2; j <= 2; j++){
				if((abs(i) != abs(j))
				&&(i != 0 && j != 0)
				&&(x+i >= 0 && x+i < 8 && y+j >= 0 && y+j < 8)){
					//printf("i = %d, j = %d \n", i, j);
					if(temp_board[x+i][y+j] == 'n'){
						//printf("can block with a knight");
						return 1;
					}
				}
			}
		}
	}
	//printf("no pieces to block with \n");
	return 0;
}


void draw_hand(int x, int y){
	if(hand){
		drawPiece(current_board[hand_x][hand_y], x-9, y-9);
	}
}
