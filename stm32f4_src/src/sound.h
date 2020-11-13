/*
 * sound.h
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */


#ifndef SOUND_H_
#define SOUND_H_

#include "stm32_pin.h"
#include "common.h"
#include <stdbool.h>

extern bool newSoundCmd;
extern uint8_t sndCmd;

void soundCmd(uint8_t cmd);
void sendSoundCmd();
void setSoundVol(uint8_t vol);
void stopSound();
void soundInit();

#endif /* SOUND_H_ */
