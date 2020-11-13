//--------------------------------------------------------------
// File     : main.c
// Datum    : 23.06.2013
// Version  : 1.4
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der USB-CDC-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32_usb_cdc.h"
#include "stm32_i2c_slave.h"
#include "stm32_pin.h"

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include "stdlib.h"
#include <misc.h>

#include "sound.h"
#include "lamps.h"
#include "solenoid.h"
#include "switches.h"

uint8_t timercycles = 0;
int T1 = 0;
int T2 = 0;

void checkTimers(){
	if (T1 != 0)
		T1--;
	if (T2 != 0)
		T2--;
	if (T2 == 0){
		T2 = 500;
		pinToggle(LED);
	}
}

void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
	    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	    timercycles++;
	    readSwitchMatrix();
	    if ((timercycles % 20) == 0){	//1ms
	    	checkWatchdog();
		    readDirectSwitches();
			readFliptronicSwitches();
			setFliptronics();
			readFlipperSwitches();
			setFlippers();
	    	checkSolenoids();
	    	sendSoundCmd();
	    	checkTimers();
	    	setGI();
	    	setSolenoids();
	    }
	    if (timercycles == 39){  // anti Ghosting
	    	clearLampMatrix();
	    }
	    if ((timercycles % 40) == 0){	//2ms
		    setLampMatrix();
	    	timercycles = 0;
	    }
	}
}

void timerInit(){

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period =  25 - 1; // 0.05 milliseconds
	TIM_TimeBaseStructure.TIM_Prescaler = 168-1; //168Mhz clock
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

}

int main(void)
{
  USB_CDC_RXSTATUS_t check=RX_EMPTY;

  SystemInit(); // Quarz Einstellungen aktivieren

  pinInit();
  timerInit();

  USB_CDC_Init();
  I2C_Slave_Init(0x70);

  bool USB_Connected = false;

  while(1)
  {
	  if(USB_CDC_GetStatus()==USB_CDC_CONNECTED && !USB_Connected) {
		  USB_CDC_SendString("PIN2RPI ", NONE); USB_CDC_SendString(VERSION, NONE); USB_CDC_SendString("\r\n", NONE);
		  USB_CDC_SendString("Waiting for Lisy commands\r\n", NONE);
		  USB_Connected = true;
	  }
  }
}
