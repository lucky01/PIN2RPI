/*
 * lamps.c
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#include "main.h"
#include "lamps.h"

#define NUMBEROFLAMPBYTES (NUMBEROFLAMPS / 8) + 1

uint8_t lampMatrix[NUMBEROFLAMPBYTES] = {};


void lampOn(uint8_t Lamp) {
	Lamp--;
	lampMatrix[(Lamp - (Lamp % 8))/8] |= 1<<(Lamp % 8);
}

void lampOff(uint8_t Lamp) {
	Lamp--;
	lampMatrix[(Lamp - (Lamp % 8))/8] &= 255-(1<<(Lamp % 8));
}

bool lampGet(uint8_t Lamp) {
	Lamp--;
	return lampMatrix[(Lamp - (Lamp % 8))/8] & 1<<(Lamp % 8);
}

void clearLampMatrix (){
	pinOff(PWREN);
	setGroup(LMP_COL,0x00);
	setGroup(LMP_ROW,0x00);
	pinOn(PWREN);
}

void setLampMatrix (){
	static uint8_t lampCol = 0;
	lampCol++;                                      // prepare for next lamp column
	if (lampCol == 8){                              // max column reached?
		lampCol = 0;
	}
	pinOff(PWREN);
	setGroup(LMP_ROW,lampMatrix[lampCol]);
	setGroup(LMP_COL,(1<<lampCol));
	pinOn(PWREN);
}

void setGI(){
	pinOff(PWREN);
	setGroup(TRIAC,lampMatrix[8]);
	pinOn(PWREN);
}

void lampMatrixInit(){
	for (int i = 0; i < NUMBEROFLAMPBYTES; i++){
		lampMatrix[i] = 0;
	}

}
