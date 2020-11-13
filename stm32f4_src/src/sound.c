/*
 * sound.c
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#include "sound.h"

#define SOUNDCOMMANDBUFFERSIZE 16

uint8_t sndCmdBuf[SOUNDCOMMANDBUFFERSIZE] = {};
int sndCmdBufIdx = 0;
int sndCmdIdx = 0;

void setSoundVol(uint8_t vol){
	soundCmd(0x79);
	soundCmd(vol);
	soundCmd(vol^0xFF);
}

void stopSound(){
	soundCmd(0x00);
}

void soundCmd(uint8_t cmd){
	sndCmdBuf[sndCmdBufIdx++] = cmd;
	if( sndCmdBufIdx==SOUNDCOMMANDBUFFERSIZE) sndCmdBufIdx = 0;
}

void sendSoundCmd(){
	if(sndCmdBufIdx != sndCmdIdx){
		pinOff(SNDEN);
		setAdd(0x1c); // sound command
		setData(sndCmdBuf[sndCmdIdx++]);
		pinOn(WDEN);
		pinOff(WDEN);
		pinOn(SNDEN);
		setAdd(0x00); // fliptronics board
		clearData();
		if( sndCmdIdx==SOUNDCOMMANDBUFFERSIZE) sndCmdIdx = 0;
	}
}

void soundInit(){
	soundCmd(0x5f);
	soundCmd(0x5f);
	stopSound();
}

