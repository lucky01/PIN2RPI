//--------------------------------------------------------------
// File     : stm32_pin.c
// Datum    : 09.02.2013
// Version  : 1.1
// Autor    : LUCKY1
//// CPU      : STM32F4
// Module   : GPIO
// Funktion : PIN Funktionen
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_pin.h"


//--------------------------------------------------------------
// Definition aller PINs
// Reihenfolge wie bei PIN_NAME_t
//
// Init : [PIN_OFF,PIN_ON]
//--------------------------------------------------------------
PIN_t PIN[] = {
	// Name    ,PORT , PIN
	{D0 ,GPIOE,GPIO_Pin_15,OUT,OFF},
	{D1 ,GPIOE,GPIO_Pin_13,OUT,OFF},
	{D2 ,GPIOE,GPIO_Pin_10,OUT,OFF},
	{D3 ,GPIOE,GPIO_Pin_12,OUT,OFF},
	{D4 ,GPIOE,GPIO_Pin_11,OUT,OFF},
	{D5 ,GPIOE,GPIO_Pin_14,OUT,OFF},
	{D6 ,GPIOB,GPIO_Pin_10,OUT,OFF},
	{D7 ,GPIOE,GPIO_Pin_8,OUT,OFF},
	{A0 ,GPIOD,GPIO_Pin_8,OUT,OFF},
	{A1 ,GPIOD,GPIO_Pin_9,OUT,OFF},
	{A2 ,GPIOD,GPIO_Pin_10,OUT,OFF},
	{A3 ,GPIOD,GPIO_Pin_11,OUT,OFF},
	{A4 ,GPIOD,GPIO_Pin_12,OUT,OFF},
	{SWCOL1 ,GPIOA,GPIO_Pin_0,OUT,OFF},
	{SWCOL2 ,GPIOA,GPIO_Pin_1,OUT,OFF},
	{SWCOL3 ,GPIOA,GPIO_Pin_2,OUT,OFF},
	{SWCOL4 ,GPIOA,GPIO_Pin_3,OUT,OFF},
	{SWCOL5 ,GPIOA,GPIO_Pin_4,OUT,OFF},
	{SWCOL6 ,GPIOA,GPIO_Pin_5,OUT,OFF},
	{SWCOL7 ,GPIOA,GPIO_Pin_6,OUT,OFF},
	{SWCOL8 ,GPIOA,GPIO_Pin_7,OUT,OFF},
	{SWROW1 ,GPIOE,GPIO_Pin_3,IN,OFF},
	{SWROW2 ,GPIOE,GPIO_Pin_5,IN,OFF},
	{SWROW3 ,GPIOC,GPIO_Pin_13,IN,OFF},
	{SWROW4 ,GPIOC,GPIO_Pin_1,IN,OFF},
	{SWROW5 ,GPIOC,GPIO_Pin_0,IN,OFF},
	{SWROW6 ,GPIOE,GPIO_Pin_6,IN,OFF},
	{SWROW7 ,GPIOE,GPIO_Pin_4,IN,OFF},
	{SWROW8 ,GPIOE,GPIO_Pin_2,IN,OFF},
	{ULEOS ,GPIOB,GPIO_Pin_14,IN,OFF},
	{UREOS ,GPIOB,GPIO_Pin_12,IN,OFF},
	{LLEOS ,GPIOB,GPIO_Pin_13,IN,OFF},
	{LREOS ,GPIOB,GPIO_Pin_11,IN,OFF},
	{ULFL ,GPIOD,GPIO_Pin_4,IN,OFF},
	{URFL ,GPIOD,GPIO_Pin_6,IN,OFF},
	{LLFL ,GPIOD,GPIO_Pin_5,IN,OFF},
	{LRFL ,GPIOD,GPIO_Pin_7,IN,OFF},
	{DIR1 ,GPIOB,GPIO_Pin_4,IN,OFF},
	{DIR2 ,GPIOB,GPIO_Pin_6,IN,OFF},
	{DIR3 ,GPIOB,GPIO_Pin_7,IN,OFF},
	{DIR4 ,GPIOB,GPIO_Pin_5,IN,OFF},
	{DIR5 ,GPIOD,GPIO_Pin_1,IN,OFF},
	{DIR6 ,GPIOD,GPIO_Pin_3,IN,OFF},
	{DIR7 ,GPIOD,GPIO_Pin_0,IN,OFF},
	{DIR8 ,GPIOD,GPIO_Pin_2,IN,OFF},
	{BLANK ,GPIOC,GPIO_Pin_8,OUT,OFF},
	{SOL1 ,GPIOB,GPIO_Pin_9,OUT,ON},
	{SOL2 ,GPIOB,GPIO_Pin_8,OUT,ON},
	{SOL3 ,GPIOE,GPIO_Pin_1,OUT,ON},
	{SOL4 ,GPIOB,GPIO_Pin_1,OUT,ON},
	{LMP_COL ,GPIOC,GPIO_Pin_5,OUT,ON},
	{LMP_ROW ,GPIOB,GPIO_Pin_0,OUT,ON},
	{SW_COL_1 ,GPIOC,GPIO_Pin_2,OUT,ON},
	{SW_ROW_1 ,GPIOC,GPIO_Pin_4,OUT,ON},
	{SW_ROW_2 ,GPIOC,GPIO_Pin_3,OUT,ON},
	{TRIAC ,GPIOE,GPIO_Pin_7,OUT,ON},
	{SW_JMP_1 ,GPIOC,GPIO_Pin_6,OUT,ON},
	{SW_JMP_2 ,GPIOC,GPIO_Pin_7,OUT,ON},
	{ZERO_CROSS ,GPIOB,GPIO_Pin_15,IN,OFF},
	{PWREN ,GPIOB,GPIO_Pin_2,OUT,ON},
	{DIS_STROBE ,GPIOC,GPIO_Pin_12,OUT,ON},
	{DIS_1 ,GPIOC,GPIO_Pin_10,OUT,ON},
	{DIS_2 ,GPIOA,GPIO_Pin_15,OUT,ON},
	{DIS_3 ,GPIOC,GPIO_Pin_11,OUT,ON},
	{DIS_4 ,GPIOB,GPIO_Pin_3,OUT,ON},
	{DISEN ,GPIOE,GPIO_Pin_9,OUT,ON},
	{WDEN ,GPIOD,GPIO_Pin_14,OUT,OFF},
	{RW ,GPIOD,GPIO_Pin_13,OUT,OFF},
	{SNDEN ,GPIOA,GPIO_Pin_9,OUT,ON},
	{RST ,GPIOD,GPIO_Pin_15,OUT,OFF},
	{LED ,GPIOE,GPIO_Pin_0,OUT,OFF},
};


//--------------------------------------------------------------
// PIN ausschalten
//--------------------------------------------------------------
void pinOff(PIN_NAME_t pin_name)
{
  PIN[pin_name].PIN_PORT->BSRRH = PIN[pin_name].PIN_PIN;
}

//--------------------------------------------------------------
// PIN einschalten
//--------------------------------------------------------------
void pinOn(PIN_NAME_t pin_name)
{
  PIN[pin_name].PIN_PORT->BSRRL = PIN[pin_name].PIN_PIN;
} 


//--------------------------------------------------------------
// PIN ein- oder ausschalten
//--------------------------------------------------------------
void pinSwitch(PIN_NAME_t pin_name, PIN_STATUS_t wert)
{
  if(wert==OFF) {
    pinOff(pin_name);
  }
  else {
    pinOn(pin_name);
  }
}

void pinToggle(PIN_NAME_t pin_name)
{
  PIN[pin_name].PIN_PORT->ODR ^= PIN[pin_name].PIN_PIN;
}

int pinGet(PIN_NAME_t pin_name)
{
	if ((PIN[pin_name].PIN_PORT->IDR & PIN[pin_name].PIN_PIN) != OFF)
	  {
	    return ON;
	  }

	return OFF;
}

void pinInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  PIN_NAME_t pin_name;

  // Clock Enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  for(pin_name=0;pin_name<PIN_ANZ;pin_name++) {

	  if (PIN[pin_name].PIN_TYPE == OUT){
			// Config als Digital-Ausgang
			GPIO_InitStructure.GPIO_Pin = PIN[pin_name].PIN_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(PIN[pin_name].PIN_PORT, &GPIO_InitStructure);

			// Default Wert einstellen
			if(PIN[pin_name].PIN_STATUS==OFF) {
				 pinOff(pin_name);
			}
			else {
				pinOn(pin_name);
			}
	  } else {
			// Config als Digital-Eingang
			GPIO_InitStructure.GPIO_Pin = PIN[pin_name].PIN_PIN;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(PIN[pin_name].PIN_PORT, &GPIO_InitStructure);

	  }
  }
}
