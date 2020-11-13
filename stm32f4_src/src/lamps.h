/*
 * lamps.h
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#ifndef LAMPS_H_
#define LAMPS_H_

#include "stm32f4xx.h"
#include <stdbool.h>
#include "common.h"

extern uint8_t lampMatrix[];

void lampOn(uint8_t Lamp);
void lampOff(uint8_t Lamp);
bool lampGet(uint8_t Lamp);
void setLampMatrix ();
void lampMatrixInit ();
void setGI();


#endif /* LAMPS_H_ */
