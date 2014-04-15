/*
	Sunheng Taing	
*/

#include <stdlib.h>
#include <stdio.h>

//My headers
#include "mylib.h"
#include "text.h"
#include "pokemonLogo.h"
#include "pikachuKO.h"
#include "pokeball.h"
#include "meowth.h"
#include "pikachu.h"
#include "happyPikachu.h"

#define NUMBADOBJS 2

enum {SPLASH, GAME, WIN, LOSE};
int splash();
int game();
void delay(int);
void lose();

int main(void) {
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	
	int seed = 0;
	int state = SPLASH;
	//Loop to keep game from exiting
	while(1) {
		switch(state) {
			case SPLASH:
				seed = splash();
				state = GAME;
				break;
			case GAME:
				state = game(seed);
				break;
			case LOSE:
				lose();
				state = GAME;
				break;
			case WIN:
				win();
				state = GAME;
			default:
				break;
		}
	}	
}

/*
	Start the game after splash screen.
*/
int game(int seed) {
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	int i;
	srand(seed);
	//make background white
	fillScreen(WHITE);
	
	//Declare player - 10x10
	PLAYER pObj;
	pObj.row = 70;
	pObj.col = 120;
	pObj.image = pokeball;
	drawImage3(pObj.row, pObj.col, 10, 10, pObj.image);
	int playerPrevR = pObj.row;
	int playerPrevC = pObj.col;
	
	//Declare good object - pikachu - 15x15
	GOODOBJ gObj;
	gObj.row = rand() % 138;
	gObj.col = rand() % 230;
	gObj.image = pikachu;
	int gPrevR;
	int gPrevC;
	int score = 5;
	
	//Setting up Bad Objects in array
	BADOBJ badObjs[NUMBADOBJS];
	BADOBJ oldBadObjs[NUMBADOBJS];
	BADOBJ *cur;
	BADOBJ *curOld;
	for(i = 0; i < NUMBADOBJS; i++) {
		badObjs[i].row = rand() % 130;
		while(badObjs[i].row < pObj.row + 20 && badObjs[i].row > pObj.row - 20) {
			badObjs[i].row = rand() % 130;
		}
		badObjs[i].col = rand() % 220;
		while(badObjs[i].col < pObj.col + 20 && badObjs[i].col > pObj.col - 20) {
			badObjs[i].col = rand() % 220;
		}
		badObjs[i].image = meowth;
		oldBadObjs[i] = badObjs[i];
	}
		
	
	while(1) {
		
		//Setting up the GoodObj
		gPrevR = gObj.row;
		gPrevC = gObj.col;
		int sign = rand() % 4; //values from 0 - 3
		int moveRBy = 2;
		int moveCBy = 2;
		if(sign == 0) { //add add
			if(gObj.row + moveRBy < 130)
				gObj.row += moveRBy;
			if(gObj.col + moveCBy < 220)
				gObj.col += moveCBy;
		}else if(sign == 1) { //add subtract
			if(gObj.row + moveRBy < 130)
				gObj.row += moveRBy;
			if(gObj.col - moveRBy > 0)
				gObj.col -= moveRBy;
		}else if(sign == 2) { //subtract subtract
			if(gObj.row - moveRBy > 0)
				gObj.row -= moveRBy;
			if(gObj.col - moveRBy > 0)
				gObj.col -= moveRBy;
		}else { //subtract add
			if(gObj.row - moveRBy > 0)
				gObj.row -= moveRBy;
			if(gObj.col + moveCBy < 220)
				gObj.col += moveRBy;
		}
		drawTrailColor(gPrevR, gPrevC, 15, 15, WHITE);
		drawImage3(gObj.row, gObj.col, 15, 15, gObj.image);
		
		//Behavior of BadObjs
		for(i = 0; i < NUMBADOBJS; i++) {
			cur = badObjs + i;
			curOld = oldBadObjs + i;
			sign = rand() % 4; //values from 0 - 3
			moveRBy = 1;
			moveCBy = 1;
			if(cur->col+10 < pObj.col) {
				cur->col += moveCBy;
			}else if (cur->col-10 > pObj.col){
				cur->col -= moveCBy;
			}
			if(cur->row+10 < pObj.row) {
				cur->row += moveRBy;
			}else if(cur->row-10 > pObj.row) {
				cur->row -= moveRBy;
			}
			
			drawTrailColor(curOld->row, curOld->col, 15, 15, WHITE);
			drawImage3(cur->row, cur->col, 15, 15, cur->image);
		}
		waitForVblank();
		
		//put current badobjs into oldbadobjs
		for(i = 0; i < NUMBADOBJS; i++) {
			oldBadObjs[i] = badObjs[i];
		}		
		
		//Pokeball movement
		if(KEY_DOWN_NOW(BUTTON_DOWN)) {
			if(pObj.row < 139) {
				playerPrevR = pObj.row;
				pObj.row += 2;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_UP)) {
			if(pObj.row > 0) {
				playerPrevR = pObj.row;
				pObj.row -= 2;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			if(pObj.col > 0) {
				playerPrevC = pObj.col;
				pObj.col -= 2;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
			if(pObj.col < 230) {
				playerPrevC = pObj.col;
				pObj.col += 2;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			return GAME;
		}
		drawTrailColor(playerPrevR, playerPrevC, 10, 10, WHITE);
		drawImage3(pObj.row, pObj.col, 10, 10, pObj.image);
		waitForVblank();
		
		//check if the player touched pikachu
		if((pObj.col < gObj.col + 10) && (pObj.col > gObj.col - 10) && (pObj.row < gObj.row + 10) && (pObj.row > gObj.row - 10)) {
			score--;
			drawTrailColor(gObj.row, gObj.col, 15, 15, WHITE);
			gObj.row = rand() % 125;
			gObj.col = rand() % 225;
			
			for(i = 0; i < NUMBADOBJS; i++) {
				badObjs[i].row = rand() % 130;
				while(badObjs[i].row < pObj.row + 20 && badObjs[i].row > pObj.row - 20) {
					badObjs[i].row = rand() % 130;
				}
				badObjs[i].col = rand() % 220;
				while(badObjs[i].col < pObj.col + 20 && badObjs[i].col > pObj.col - 20) {
					badObjs[i].col = rand() % 220;
				}
				badObjs[i].image = meowth;
				oldBadObjs[i] = badObjs[i];
			}
			fillScreen(WHITE);
		}
		
		//check if the player touched meowth
		for(i = 0; i < NUMBADOBJS; i++) {
			cur = badObjs + i;
			if((pObj.col < cur->col + 11) && (pObj.col > cur->col - 11) && (pObj.row < cur->row + 11) && (pObj.row > cur->row - 11)) {
				return LOSE;
			}
		}
		
		//Scoreboard
		char buffer[41];
		sprintf(buffer, "Pika to catch: %d", score);
		drawTrailColor(150, 5, 240, 10, WHITE);
		drawString(150, 5, buffer, CYAN);
		seed++;
		
		if(score == 0) {
			return WIN;
		}
	}
}

/*
	Display splash screen with message and wait for start button.
*/
int splash() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	
	int seed = 0;
	//draw pokemon logo using DMA
	drawImage3(0, 0, 240, 100, pokemonLogo);
	drawString(120, 5, "Pikachus, I choose you!", CYAN);
	drawString(130, 5, "Press Start (Enter)", CYAN);
	
	while(!KEY_DOWN_NOW(BUTTON_START)) {
		seed++;
	}
	while(KEY_DOWN_NOW(BUTTON_START));
	return seed;	
}

/*
	Losing screen where player can restart the game.
*/
void lose() {
	
	//draw image using DMA
	drawImage3(0, 0, 240, 160, pikachuKO);
	
	drawString(50, 5, "Why Ash no win tournament?", CYAN);
	drawString(70, 5, "One more try! You can do better!", RED);
	drawString(90, 5, "Restart (Backspace)", YELLOW);
	
	while(!KEY_DOWN_NOW(BUTTON_SELECT));
	while(KEY_DOWN_NOW(BUTTON_SELECT));
}

/*
	Winning screen where player can restart the game.
*/
void win() {
	int seed = 0;
	//draw image using DMA
	drawImage3(0, 0, 240, 160, happyPikachu);
	
	drawString(50, 5, "You're better than Ash", BLUE);
	drawString(90, 5, "Restart (Backspace)", CYAN);
	
	while(!KEY_DOWN_NOW(BUTTON_SELECT)) {
		seed++;
	}
	while(KEY_DOWN_NOW(BUTTON_SELECT));
	return seed;
}
