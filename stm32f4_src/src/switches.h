/*
 * switches.h
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include "stm32f4xx.h"
#include <stdbool.h>

void readDirectSwitches ();
void readFlipperSwitches ();
void readFliptronicSwitches ();
void readSwitchMatrix ();
uint8_t getChangedSwitch();
uint8_t getNextChangedSwitch();
uint8_t getSwitch(uint8_t Switch);
void switchMatrixInit();

#endif /* SWITCHES_H_ */
