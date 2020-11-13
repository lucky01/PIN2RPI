//--------------------------------------------------------------
// File     : stm32_ub_i2c3_slave.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_I2C3_SLAVE_H
#define __STM32F4_UB_I2C3_SLAVE_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "misc.h"


//--------------------------------------------------------------
// I2C-Clock
// Grundfrequenz (I2C3)= APB1 (APB1=45MHz)
// Mögliche Einstellungen = 100000 = 100 kHz
//--------------------------------------------------------------
#define   I2C3_CLOCK_FRQ   100000  // I2C-Frq in Hz (100 kHz)  


//--------------------------------------------------------------
// Defines
//-------------------------------------------------------------- 
#define   I2C_BUF_SIZE    0xFF    // RAM Größe in Byte (1 bis 255)



//--------------------------------------------------------------
// Struktur eines I2C-Pins
//--------------------------------------------------------------
typedef struct {
  GPIO_TypeDef* PORT;     // Port
  const uint16_t PIN;     // Pin
  const uint32_t CLK;     // Clock
  const uint8_t SOURCE;   // Source
}I2C3_PIN_t; 


//--------------------------------------------------------------
// Struktur vom I2C-Device
//--------------------------------------------------------------
typedef struct {
  I2C3_PIN_t  SCL;       // Clock-Pin
  I2C3_PIN_t  SDA;       // Data-Pin
}I2C3_DEV_t;


//--------------------------------------------------------------
typedef enum {
   I2C3_MODE_WAITING =0,    // warte auf Befehle
   I2C3_MODE_SLAVE_ADR_WR,  // Slave Adresse empfangen (zum schreiben)
   I2C3_MODE_ADR_BYTE,      // ADR-Byte empfangen
   I2C3_MODE_DATA_BYTE_WR,  // Daten-Byte (zum schreiben)
   I2C3_MODE_SLAVE_ADR_RD,  // Slave Adresse empfangen (zum lesen)
   I2C3_MODE_DATA_BYTE_RD   // Daten-Byte (zum lesen)
}I2C3_SLAVE_MODE_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void I2C_Slave_Init(uint8_t slave_adr);
uint8_t I2C_Slave_ReadByte(uint8_t adr);
void I2C_Slave_WriteByte(uint8_t adr, uint8_t wert);


//--------------------------------------------------------------
#endif // __STM32F4_UB_I2C3_SLAVE_H
