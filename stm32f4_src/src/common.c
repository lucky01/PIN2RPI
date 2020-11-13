/*
 * common.c
 *
 *  Created on: 15.04.2020
 *      Author: jamann
 */

#include "common.h"
#include <stdbool.h>

bool resetStatus = true;

uint16_t watchdog = 0;
extern int T1;

void setData (uint8_t data){
	//				GPIOE					bit15			   bit13			   bit10		     bit12			   bit11			   bit14			 bit8
	GPIOE->ODR = (GPIOE->IDR & 0x02FF) | ((data&1) << 15) | ((data&2) << 12) | ((data&4) << 8) | ((data&8) << 9) | ((data&16) << 7) | ((data&32) << 9) | ((data&128) << 1);
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,data&64);
}

void clearData (){
	GPIOE->ODR = GPIOE->IDR & 0x02FF;
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}

uint8_t getData (){
	uint8_t data = pinGet(D0) | (pinGet(D1)<<1) | (pinGet(D2)<<2) | (pinGet(D3)<<3) | (pinGet(D4)<<4) | (pinGet(D5)<<5) | (pinGet(D6)<<6) | (pinGet(D7)<<7);
	return data;
}

void switchDataToInput(){
	GPIOE->MODER &= ~(GPIO_MODER_MODER15);
	GPIOE->MODER &= ~(GPIO_MODER_MODER13);
	GPIOE->MODER &= ~(GPIO_MODER_MODER10);
	GPIOE->MODER &= ~(GPIO_MODER_MODER12);
	GPIOE->MODER &= ~(GPIO_MODER_MODER11);
	GPIOE->MODER &= ~(GPIO_MODER_MODER14);
	GPIOB->MODER &= ~(GPIO_MODER_MODER10);
	GPIOE->MODER &= ~(GPIO_MODER_MODER8);
}

void switchDataToOutput(){
	GPIOE->MODER |= GPIO_MODER_MODER15_0;
	GPIOE->MODER |= GPIO_MODER_MODER13_0;
	GPIOE->MODER |= GPIO_MODER_MODER10_0;
	GPIOE->MODER |= GPIO_MODER_MODER12_0;
	GPIOE->MODER |= GPIO_MODER_MODER11_0;
	GPIOE->MODER |= GPIO_MODER_MODER14_0;
	GPIOB->MODER |= GPIO_MODER_MODER10_0;
	GPIOE->MODER |= GPIO_MODER_MODER8_0;
}

void setAdd(uint8_t address){
	GPIOD->ODR = (GPIOD->IDR & 0xE0FF) | (address << 8);
}

void setGroup(PIN_NAME_t pinName, uint8_t cmd){
	pinOff(pinName);
	setData(cmd);
	__asm__("NOP");
	pinOn(pinName);
	clearData();
}

void resetBoard(){
	if (!resetStatus){
		stopSound();
		solenoidInit();
		lampMatrixInit();
		switchMatrixInit();
		resetStatus= true;
	}
}

void delay(int ms){
	T1 = ms;
	while (T1 != 0){
	}
}

uint8_t initBoard(){
	pinOff(RST);
	pinOff(BLANK);
	__asm__("NOP");
	pinOn(RST);
	pinOn(BLANK);
//	delay(3000);
//	soundInit();
	solenoidInit();
	lampMatrixInit();
	switchMatrixInit();
	resetStatus = false;
	return 0;
}



void checkWatchdog (){
	if (watchdog > 0){
		watchdog--;
	}
	if (watchdog == 0){
		resetBoard();
	}
}

uint8_t setWatchdog(){
	watchdog = 1000;
	return 0;
}
