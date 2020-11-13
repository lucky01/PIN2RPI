/*
 * solenoid.c
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#include "main.h"
#include "solenoid.h"
#include "switches.h"
#include <string.h>

#define DEFAULTPULSETIME 20
#define DEFAULTRECYCLETIME DEFAULTPULSETIME * 2

const uint16_t pwmPattern[17]={
	0b0000000000000000,		//0
	0b0000000000000001,		//1
	0b0000000100000001,		//2
	0b0000100000100001,		//3
	0b0001000100010001,		//4
	0b0010010001001001,		//5
	0b0100100101001001,		//6
	0b0101010001010101,		//7
	0b0101010101010101,		//8
	0b0011100011100111,		//9
	0b0101110101011101,		//10
	0b0101110111011101,		//11
	0b0111011101110111,		//12
	0b0111101111101111,		//13
	0b0111111101111111,		//14
	0b1111111101111111,		//15
	0b1111111111111111,		//16
};

sol_t sol[NUMBEROFSOLENOIDS] = {};
sol_hw_rule_t solHWRule[NUMBEROFHWRULES];													// hardware rules for activated switches

uint8_t statusSol1to8 = 0;
uint8_t statusSol9to16 = 0;
uint8_t statusSol17to24 = 0;
uint8_t statusSol25to32 = 0;
uint8_t statusWPC95Flipper = 0;
uint8_t statusFliptronics = 0;
uint8_t statusAux8 = 0;
uint8_t enableWPC95Flipper = false;
uint8_t enableFliptronics = false;
uint8_t enableAux8 = false;
uint8_t pwmBitpos = 0;



void solenoidSetPulseTime(uint8_t solenoid, uint8_t pulseTime){
	solenoid--;
	if (solenoid >= 32 && solenoid < 40)
		enableWPC95Flipper = true;
	if (solenoid >= 40 && solenoid < 48)
		enableFliptronics = true;
	if (solenoid >= 48 && solenoid < 56)
		enableAux8 = true;
	sol[solenoid].presetPulseTime = pulseTime;
}

void solenoidSetRecycleTime(uint8_t solenoid, uint8_t recycleTime){
	solenoid--;
	if (solenoid >= 32 && solenoid < 40)
		enableWPC95Flipper = true;
	if (solenoid >= 40 && solenoid < 48)
		enableFliptronics = true;
	if (solenoid >= 48 && solenoid < 56)
		enableAux8 = true;
	sol[solenoid].presetRecycleTime = recycleTime;
}

void solenoidInit(){
  for (int i=0; i < NUMBEROFSOLENOIDS; i++) {
	sol[i].pulseTime = 0;
	sol[i].recycleTime = 0;
	sol[i].presetPulseTime = DEFAULTPULSETIME;
	sol[i].presetRecycleTime = DEFAULTRECYCLETIME;
	sol[i].pulsePWM = pwmPattern[0];
	sol[i].holdPWM = pwmPattern[0];
	sol[i].currentPWM = pwmPattern[0];
  }
  memset(solHWRule,0x00,(sizeof(sol_hw_rule_t)*NUMBEROFHWRULES));
}

void solenoidPulse (uint8_t Solenoid){
	Solenoid--;
	if (sol[Solenoid].pulseTime == 0)
		sol[Solenoid].pulseTime = sol[Solenoid].presetPulseTime;
	sol[Solenoid].pulsePWM = pwmPattern[16];
	sol[Solenoid].holdPWM = pwmPattern[0];
	sol[Solenoid].currentPWM = pwmPattern[16];
}

void solenoidPulseEnable (uint8_t Solenoid, uint8_t pulseTime, uint8_t pulsePWM, uint8_t holdPWM){
	Solenoid--;
	if (sol[Solenoid].pulseTime == 0) {
		sol[Solenoid].pulseTime = pulseTime;
		sol[Solenoid].pulsePWM = pwmPattern[(pulsePWM+1)/16];
		sol[Solenoid].holdPWM = pwmPattern[(holdPWM+1)/16];
		sol[Solenoid].currentPWM = pwmPattern[(pulsePWM+1)/16];
	}
}

void solenoidOn (uint8_t Solenoid){
	Solenoid--;
	if (sol[Solenoid].pulseTime == 0){
		sol[Solenoid].pulseTime = 255;
		sol[Solenoid].presetPulseTime = 255;
		sol[Solenoid].presetRecycleTime = 0;
		sol[Solenoid].pulsePWM = pwmPattern[16];
		sol[Solenoid].holdPWM = pwmPattern[16];
		sol[Solenoid].currentPWM = pwmPattern[16];
	}
}

void solenoidOff(uint8_t Solenoid) {
	Solenoid--;
	sol[Solenoid].pulseTime = 0;
	sol[Solenoid].presetPulseTime = DEFAULTPULSETIME;
	sol[Solenoid].presetRecycleTime = DEFAULTRECYCLETIME;
	sol[Solenoid].pulsePWM = pwmPattern[16];
	sol[Solenoid].holdPWM = pwmPattern[0];
	sol[Solenoid].currentPWM = pwmPattern[16];
}

uint8_t solenoidGet(uint8_t Solenoid) {
	if (Solenoid > 40)
		return 0x02;
	Solenoid--;
	if (sol[Solenoid].pulseTime != 0 && sol[Solenoid].recycleTime == 0)
		return 0x01;
	return 0x00;
}

uint8_t calcSolGroup(uint8_t offset){
	uint8_t var = 0;
	for (int i = 0; i < 8; i++){
		if (sol[i+offset].recycleTime > 0)
			sol[i+offset].recycleTime--;
		if (sol[i+offset].pulseTime > 0 && sol[i+offset].recycleTime == 0) {
			if (sol[i+offset].holdPWM != pwmPattern[16] && sol[i+offset].presetRecycleTime != 0)
				sol[i+offset].pulseTime--;
			var = var | (((sol[i+offset].currentPWM>>pwmBitpos)&0x01)<<i);
			if (sol[i+offset].pulseTime == 0){
				if (sol[i+offset].holdPWM == pwmPattern[0])
					sol[i+offset].recycleTime = sol[i+offset].presetRecycleTime;
				else{
					sol[i+offset].currentPWM = sol[i+offset].holdPWM;
					sol[i+offset].presetPulseTime = 255;
					sol[i+offset].presetRecycleTime = 0;
					sol[i+offset].pulseTime = 255;
				}
			}
		}
	}
	return var;
}

void setSolenoids()	{
	pinOff(PWREN);
	setGroup(SOL2,statusSol1to8);
	setGroup(SOL4,statusSol9to16);
	setGroup(SOL3,statusSol17to24);
	setGroup(SOL1,statusSol25to32);
	pinOn(PWREN);
}

void setFlippers()	{
	if (enableWPC95Flipper){
		statusWPC95Flipper = calcSolGroup(32);
		pinOff(PWREN);
		setGroup(SW_JMP_1,statusWPC95Flipper);
		pinOn(PWREN);
	}
}

void setFliptronics()	{
	if (enableFliptronics){
		statusFliptronics = calcSolGroup(40);
		pinOff(SNDEN);
		setAdd(0x14); // fliptronics board
		setData(statusFliptronics^0xFF);
		pinOn(WDEN);
		pinOff(WDEN);
		pinOn(SNDEN);
		setAdd(0x00);
		clearData();
	}
}

void setAux8()	{
	if (enableAux8){
		statusAux8 = calcSolGroup(48);
		pinOff(DISEN);
		setData(statusAux8);
		pinOn(DIS_STROBE);
		pinOff(DIS_STROBE);
		pinOn(DISEN);
		clearData();
	}
}

void removeSolenoidHWRule(uint8_t ruleNo){
	uint8_t i = ruleNo;
	while (solHWRule[i].solenoid) {							// move all entries up
		memcpy(&solHWRule[i], &solHWRule[i+1],sizeof(sol_hw_rule_t));
		i++;
	}
	memset (&solHWRule[i],0x00,sizeof(sol_hw_rule_t)); //delete last entry
}

void setSolenoidHwRule (sol_hw_rule_t* newRule){
	uint8_t i = 0;
	if (!newRule->switch1 && !newRule->switch2 && !newRule->switch3) { 	// all flags zero means disable rules
		while (solHWRule[i].solenoid) {									// check for HW activation rules for this switch
			if (solHWRule[i].solenoid == newRule->solenoid) { 			// rule for this solenoid found?
				removeSolenoidHWRule(i);
			}
			else {
				i++;}
		}
	}
	else {											// create new HW rule
		if (newRule->pulseTime) {						// pulse time > 0?
			while (solHWRule[i].solenoid && (i<NUMBEROFHWRULES-1)) // look for a free slot
				i++;
			memcpy (&solHWRule[i],newRule,sizeof(sol_hw_rule_t));
		}
	}
}

void checkSolenoidHWRules(uint8_t changedSwitch){
	int c = 0;
	while (solHWRule[c].solenoid && (c<NUMBEROFHWRULES)){
		if (solHWRule[c].switch1 == changedSwitch  && solHWRule[c].sw1Flag){
			solenoidPulseEnable(solHWRule[c].solenoid, solHWRule[c].pulseTime, solHWRule[c].pulsePWM, solHWRule[c].holdPWM);
			if(solHWRule[c].sw1Flag > 1)
				solHWRule[c].sw1Flag = 0;
		}
		if (solHWRule[c].switch2 == changedSwitch && solHWRule[c].sw2Flag){
			solenoidPulseEnable(solHWRule[c].solenoid, solHWRule[c].pulseTime, solHWRule[c].pulsePWM, solHWRule[c].holdPWM);
			if(solHWRule[c].sw2Flag > 1)
				solHWRule[c].sw2Flag = 0;
		}
		if (solHWRule[c].switch3 == changedSwitch && solHWRule[c].sw3Flag){
			solenoidPulseEnable(solHWRule[c].solenoid, solHWRule[c].pulseTime, solHWRule[c].pulsePWM, solHWRule[c].holdPWM);
			if(solHWRule[c].sw3Flag > 1)
				solHWRule[c].sw3Flag = 0;
		}
		c++;
	}
}

void checkSolenoids (){
	pwmBitpos++;
	if (pwmBitpos == 16)
		pwmBitpos = 0;
	statusSol1to8 = calcSolGroup(0);
	statusSol9to16 = calcSolGroup(8);
	statusSol17to24 = calcSolGroup(16);
	statusSol25to32 = calcSolGroup(24);
}
