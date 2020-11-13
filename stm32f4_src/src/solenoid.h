/*
 * solenoid.h
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#ifndef SOLENOID_H_
#define SOLENOID_H_

#include <stdbool.h>
#include "common.h"

typedef struct {
	uint8_t pulseTime;
	uint8_t recycleTime;
	uint8_t presetPulseTime;
	uint8_t presetRecycleTime;
	uint16_t pulsePWM;
	uint16_t holdPWM;
	uint16_t currentPWM;
} sol_t;


typedef struct {
	uint8_t solenoid;
	uint8_t switch1;
	uint8_t switch2;
	uint8_t switch3;
	uint8_t pulseTime;
	uint16_t pulsePWM;
	uint16_t holdPWM;
	uint8_t sw1Flag;
	uint8_t sw2Flag;
	uint8_t sw3Flag;
} sol_hw_rule_t;

extern sol_t sol[];
extern uint8_t enableWPC95Flipper;
extern uint8_t enableFliptronics;
extern uint8_t enableAux8;


void solenoidInit();
void solenoidSetPulseTime(uint8_t solenoid, uint8_t pulseTime);
void solenoidSetRecycleTime(uint8_t solenoid, uint8_t recycleTime);
void solenoidPulse (uint8_t Solenoid);
void solenoidPulseEnable (uint8_t Solenoid, uint8_t pulseTime, uint8_t pulsePWM, uint8_t holdPWM);
void solenoidOn (uint8_t Solenoid);
void solenoidOff(uint8_t Solenoid);
uint8_t solenoidGet(uint8_t Solenoid);
void checkSolenoids ();
void setSolenoids();
void setFlippers();
void setSolenoidHwRule (sol_hw_rule_t* newRule);
void checkSolenoidHWRules(uint8_t changedSwitch);


#endif /* SOLENOID_H_ */
