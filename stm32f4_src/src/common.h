/*
 * common.h
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "stm32f4xx_gpio.h"
#include "stm32_pin.h"

void setData (uint8_t data);
void clearData ();
void switchDataToInput();
void switchDataToOutput();
uint8_t getData ();
void setAdd(uint8_t address);
void setGroup(PIN_NAME_t pinName, uint8_t cmd);
uint8_t initBoard();
void checkWatchdog ();
uint8_t setWatchdog();

#endif /* COMMON_H_ */
