//--------------------------------------------------------------
// File     : stm32_ub_led.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_PIN_H
#define __STM32F4_PIN_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"




//--------------------------------------------------------------
// Liste aller PINs
// (keine Nummer doppelt und von 0 beginnend)
//--------------------------------------------------------------
typedef enum 
{
  D0 = 0,
  D1 = 1,
  D2 = 2,
  D3 = 3,
  D4 = 4,
  D5 = 5,
  D6 = 6,
  D7 = 7,
  A0 = 8,
  A1 = 9,
  A2 = 10,
  A3 = 11,
  A4 = 12,
  SWCOL1 = 13,
  SWCOL2 = 14,
  SWCOL3 = 15,
  SWCOL4 = 16,
  SWCOL5 = 17,
  SWCOL6 = 18,
  SWCOL7 = 19,
  SWCOL8 = 20,
  SWROW1 = 21,
  SWROW2 = 22,
  SWROW3 = 23,
  SWROW4 = 24,
  SWROW5 = 25,
  SWROW6 = 26,
  SWROW7 = 27,
  SWROW8 = 28,
  ULEOS = 29,
  UREOS = 30,
  LLEOS = 31,
  LREOS = 32,
  ULFL = 33,
  URFL = 34,
  LLFL = 35,
  LRFL = 36,
  DIR1 = 37,
  DIR2 = 38,
  DIR3 = 39,
  DIR4 = 40,
  DIR5 = 41,
  DIR6 = 42,
  DIR7 = 43,
  DIR8 = 44,
  BLANK = 45,
  SOL1 = 46,
  SOL2 = 47,
  SOL3 = 48,
  SOL4 = 49,
  LMP_COL = 50,
  LMP_ROW = 51,
  SW_COL_1 = 52,
  SW_ROW_1 = 53,
  SW_ROW_2 = 54,
  TRIAC = 55,
  SW_JMP_1 = 56,
  SW_JMP_2 = 57,
  ZERO_CROSS = 58,
  PWREN = 59,
  DIS_STROBE = 60,
  DIS_1 = 61,
  DIS_2 = 62,
  DIS_3 = 63,
  DIS_4 = 64,
  DISEN = 65,
  WDEN = 66,
  RW = 67,
  SNDEN = 68,
  RST = 69,
  LED = 70,
}PIN_NAME_t;

#define  PIN_ANZ   71 // Anzahl von PIN_NAME_t


//--------------------------------------------------------------
// Status eines PIN
//--------------------------------------------------------------
typedef enum {
  OFF = 0,  // PIN AUS
  ON        // PIN EIN
}PIN_STATUS_t;

//--------------------------------------------------------------
// Status eines PIN
//--------------------------------------------------------------
typedef enum {
  IN = 0,
  OUT
}PIN_TYPE_t;


//--------------------------------------------------------------
// Struktur eines PIN
//--------------------------------------------------------------
typedef struct {
  PIN_NAME_t PIN_NAME;    // Name
  GPIO_TypeDef* PIN_PORT; // Port
  const uint16_t PIN_PIN; // Pin
  PIN_TYPE_t PIN_TYPE;    // Name
  PIN_STATUS_t PIN_STATUS;    // Name
}PIN_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void pinInit();
void pinOff(PIN_NAME_t pin_name);
void pinOn(PIN_NAME_t pin_name);
void pinSwitch(PIN_NAME_t pin_name, PIN_STATUS_t wert);
void pinToggle(PIN_NAME_t pin_name);
int pinGet(PIN_NAME_t pin_name);



//--------------------------------------------------------------
#endif // __STM32F4_PIN_H
