/*
 * switches.c
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#include "main.h"
#include "common.h"
#include "switches.h"
#include "solenoid.h"
#include <string.h>


#define SWITCHMATRIXBYTES (NUMBEROFSWITCHES / 8)
#define CHANGEDSWITCHBUFFERSIZE 32

uint8_t switchMatrix[SWITCHMATRIXBYTES] = {};
uint8_t lastSwitchMatrix[SWITCHMATRIXBYTES] = {};
uint8_t changedSwitchBuf[CHANGEDSWITCHBUFFERSIZE] = {};
int chgSwBufIdx = 0;
int chgSwIdx = 0;


uint8_t getChangedSwitch(int switchGroupStart, int switchGroupEnd) {
	for(int i = switchGroupStart; i < switchGroupEnd+1; i++){
		if (switchMatrix[i] != lastSwitchMatrix[i]){
			uint8_t compare = lastSwitchMatrix[i] ^ switchMatrix[i];
			lastSwitchMatrix[i] = switchMatrix[i];
			if (switchMatrix[i] & compare)
				return (127 + (i*8) + __builtin_ffs (compare));
			else
				return ((i*8) + __builtin_ffs (compare));
		}
	}
	return 127;
}

void updateChangedSwitchBuffer(int switchGroupStart, int switchGroupEnd){
	uint8_t changedSwitch = 127;
	do {
		changedSwitch = getChangedSwitch(switchGroupStart, switchGroupEnd);
		if (changedSwitch != 127) {
			changedSwitchBuf[chgSwBufIdx++] = changedSwitch;
			checkSolenoidHWRules(changedSwitch);
		}
		if (chgSwBufIdx == CHANGEDSWITCHBUFFERSIZE)
			chgSwBufIdx = 0;
	} while (changedSwitch!=127);
}

uint8_t getSwitch(uint8_t Switch) {
	Switch--;
		return switchMatrix[(Switch - (Switch % 8))/8] & 1<<(Switch % 8);
}

void readDirectSwitches (){
	uint8_t data = pinGet(DIR1) | (pinGet(DIR2)<<1) | (pinGet(DIR3)<<2) | (pinGet(DIR4)<<3) | (pinGet(DIR5)<<4) | (pinGet(DIR6)<<5) | (pinGet(DIR7)<<6) | (pinGet(DIR8)<<7);
	switchMatrix[8] =  data ^ 0xFF;
	updateChangedSwitchBuffer(8,8);
}

void readFlipperSwitches (){
	if(enableWPC95Flipper){
		uint8_t data = pinGet(LREOS) | (pinGet(LRFL)<<1) | (pinGet(LLEOS)<<2) | (pinGet(LLFL)<<3) | (pinGet(ULEOS)<<4) | (pinGet(ULFL)<<5) | (pinGet(UREOS)<<6) | (pinGet(URFL)<<7);
		switchMatrix[9] = data ^ 0xFF;
		updateChangedSwitchBuffer(9,9);
	}
}

void readFliptronicSwitches (){
	if(enableFliptronics){
		pinOn(RW);
		switchDataToInput();
		pinOff(SNDEN);
		setAdd(0x14); // fliptronics board
		__asm__("NOP");
		switchMatrix[10] = getData() ^ 0xff;
		setAdd(0x00); // fliptronics board
		pinOff(RW);
		switchDataToOutput();
		pinOn(SNDEN);
		updateChangedSwitchBuffer(10,10);
	}
}

void setSWCOL (uint8_t data){
	GPIOA->ODR = (GPIOA->IDR & 0xFF00) | (data & 0x00FF);
}

uint8_t readSWROW (){
	uint8_t data = pinGet(SWROW1) | (pinGet(SWROW2)<<1) | (pinGet(SWROW3)<<2) | (pinGet(SWROW4)<<3) | (pinGet(SWROW5)<<4) | (pinGet(SWROW6)<<5) | (pinGet(SWROW7)<<6) | (pinGet(SWROW8)<<7);
	return data ^ 0xFF;
}

void readSwitchMatrix (){
	static int SwColIndex = 0;
	if ((SwColIndex & 1) == 0) {
		setSWCOL(1<<(SwColIndex>>1));
	} else {
		switchMatrix[SwColIndex>>1] = readSWROW();
		setSWCOL(0);
	}
	SwColIndex++;
	if (SwColIndex == 16){
		SwColIndex = 0;
		updateChangedSwitchBuffer(0,7);
	}
}

void switchMatrixInit(){
	memcpy(lastSwitchMatrix,switchMatrix,SWITCHMATRIXBYTES);
}

uint8_t getNextChangedSwitch() {
	uint8_t ret = 127;
	if(chgSwBufIdx != chgSwIdx){
		ret = changedSwitchBuf[chgSwIdx++];
		if(chgSwIdx==CHANGEDSWITCHBUFFERSIZE)
			chgSwIdx = 0;
	}
	return ret;
}
