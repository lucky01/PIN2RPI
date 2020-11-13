//--------------------------------------------------------------
// File     : stm32_ub_i2c3_slave.c
// Datum    : 05.11.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : GPIO, I2C, MISC 
// Funktion : I2C-Slave-Funktionen (I2C-3)
//            Erstellt ein virtuelles I2C-RAM
//
// Hinweis  : mögliche Pinbelegungen
//            I2C3 : SCL :[PA8] 
//                   SDA :[PC9]
//            externe PullUp-Widerstände an SCL+SDA notwendig
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_i2c_slave.h"
#include "lisy.h"


//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_I2C3_InitI2C(uint8_t slave_adr);
void P_I2C3_InitNVIC(void);
void P_I2C3_ClearFlag(void);


//--------------------------------------------------------------
// Globale Variabeln
//--------------------------------------------------------------
I2C3_SLAVE_MODE_t i2c3_mode=I2C3_MODE_WAITING;
uint8_t i2c_tx_ptr=0;
uint8_t i2c_tx_buffer[I2C_BUF_SIZE+1];
uint8_t i2c_rx_ptr=0;
uint8_t i2c_rx_buffer[I2C_BUF_SIZE+1];


//--------------------------------------------------------------
// Definition von I2C3
//--------------------------------------------------------------
I2C3_DEV_t I2C3DEV = {
// PORT , PIN      , Clock              , Source 
  {GPIOA,GPIO_Pin_8,RCC_AHB1Periph_GPIOA,GPIO_PinSource8}, // SCL an PA8
  {GPIOC,GPIO_Pin_9,RCC_AHB1Periph_GPIOC,GPIO_PinSource9}, // SDA an PC9
};



//--------------------------------------------------------------
// Init von I2C3 (als Slave)
// slave_adr => I2C-Basis-Adresse vom Slave
//-------------------------------------------------------------- 
void I2C_Slave_Init(uint8_t slave_adr)
{
  static uint8_t init_ok=0;
  GPIO_InitTypeDef  GPIO_InitStructure;
  uint32_t n;

  // initialisierung darf nur einmal gemacht werden
  if(init_ok!=0) {
    return;
  } 

  // RAM löschen
  for(n=0;n<I2C_BUF_SIZE;n++) {
    i2c_tx_buffer[n]=0x00;
  }

  // I2C-Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

  // Clock Enable der Pins
  RCC_AHB1PeriphClockCmd(I2C3DEV.SCL.CLK, ENABLE); 
  RCC_AHB1PeriphClockCmd(I2C3DEV.SDA.CLK, ENABLE);

  // I2C reset
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C3, DISABLE);

  // I2C Alternative-Funktions mit den IO-Pins verbinden  
  GPIO_PinAFConfig(I2C3DEV.SCL.PORT, I2C3DEV.SCL.SOURCE, GPIO_AF_I2C3); 
  GPIO_PinAFConfig(I2C3DEV.SDA.PORT, I2C3DEV.SDA.SOURCE, GPIO_AF_I2C3);

  // I2C als Alternative-Funktion als OpenDrain  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  // SCL-Pin
  GPIO_InitStructure.GPIO_Pin = I2C3DEV.SCL.PIN;
  GPIO_Init(I2C3DEV.SCL.PORT, &GPIO_InitStructure);
  // SDA-Pin
  GPIO_InitStructure.GPIO_Pin = I2C3DEV.SDA.PIN;
  GPIO_Init(I2C3DEV.SDA.PORT, &GPIO_InitStructure);

  // I2C init
  P_I2C3_InitI2C(slave_adr<<1); // address is 7bit

  // NVIC init
  P_I2C3_InitNVIC();

  // interrupt enable
  I2C_ITConfig(I2C3, I2C_IT_EVT, ENABLE);
  I2C_ITConfig(I2C3, I2C_IT_BUF, ENABLE);
  I2C_ITConfig(I2C3, I2C_IT_ERR, ENABLE);

  // init Mode speichern
  init_ok=1;
}


//--------------------------------------------------------------
// auslesen von einem Byte vom internen RAM
// adr       => RAM-Adresse die gelesen wird
//
// Return_wert :
//  0...255 , Bytewert der gelesen wurde
//--------------------------------------------------------------
uint8_t I2C_Slave_ReadByte(uint8_t adr)
{
  uint8_t ret_wert=0;

  if(adr>I2C_BUF_SIZE) return(0);

  ret_wert=i2c_tx_buffer[adr];

  return(ret_wert);
}


//--------------------------------------------------------------
// schreiben von einem Byte ins interne RAM
// adr       => RAM-Adresse die beschrieben wird
// wert      => Datenwert [0...255]
//--------------------------------------------------------------
void I2C_Slave_WriteByte(uint8_t adr, uint8_t wert)
{
  if(adr>I2C_BUF_SIZE) return;

  i2c_tx_buffer[adr]=wert;
}


void I2C_SendString(char *ptr)
{
	int i = 0;
	while (*ptr != 0) {
		i2c_tx_buffer[i]=*ptr;
	    ptr++;
	    i++;
	}

	i2c_tx_buffer[i]=0x00;
}

//--------------------------------------------------------------
// interne Funktion
// Init der I2C-Schnittstelle (als Slave)
//--------------------------------------------------------------
void P_I2C3_InitI2C(uint8_t slave_adr)
{
  I2C_InitTypeDef  I2C_InitStructure;

  // I2C-Konfiguration
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = slave_adr;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C3_CLOCK_FRQ;

  // ACK enable
  I2C_AcknowledgeConfig(I2C3, ENABLE);

  // I2C enable
  I2C_Cmd(I2C3, ENABLE);

  // Init Struktur
  I2C_Init(I2C3, &I2C_InitStructure);
}


//--------------------------------------------------------------
// interne Funktion
// Init vom NVIC
//--------------------------------------------------------------
void P_I2C3_InitNVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  // EV-Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = I2C3_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // ER-Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = I2C3_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);    
}


//--------------------------------------------------------------
// interne Funktion
// Flags löschen
//--------------------------------------------------------------
void P_I2C3_ClearFlag(void)
{

  // ADDR-Flag löschen
  while((I2C3->SR1 & I2C_SR1_ADDR) == I2C_SR1_ADDR) {
    I2C3->SR1;
    I2C3->SR2;
  }

  // STOPF Flag löschen
  while((I2C3->SR1&I2C_SR1_STOPF) == I2C_SR1_STOPF) {
    I2C3->SR1;
    I2C3->CR1 |= 0x1;
  }
}


//--------------------------------------------------------------
// ISR (Event Handler vom I2C)
//--------------------------------------------------------------
/*void I2C3_EV_IRQHandler(void)
{
  uint32_t event;
  uint8_t wert;

  // Event auslesen
  event=I2C_GetLastEvent(I2C3);

  if(event==I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED) {
    // Master hat die Slave Adresse gesendet
    // um Daten an den Slave zu senden
    i2c3_mode=I2C3_MODE_SLAVE_ADR_WR;
  }
  else if(event==I2C_EVENT_SLAVE_BYTE_RECEIVED) {
    // Master hat ein Byte zum Slave gesendet
    wert=I2C_ReceiveData(I2C3);
    // check ob Adresse oder Daten
    if(i2c3_mode==I2C3_MODE_SLAVE_ADR_WR) {
      i2c3_mode=I2C3_MODE_ADR_BYTE;
      // aktuelle ram_adresse setzen
      i2c3_ram_adr=wert;
    }
    else {
      i2c3_mode=I2C3_MODE_DATA_BYTE_WR;
      // Daten in RAM speichern
      i2c3_ram[i2c3_ram_adr]=wert;
      // ram adresse autom. hochzählen
      i2c3_ram_adr++;
    }
  }
  else if(event==I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) {
    // Master hat die Slave Adresse gesendet
    // um Daten vom Slave auszulesen
    i2c3_mode=I2C3_MODE_SLAVE_ADR_RD;
    // Daten aus RAM auslesen
    wert=i2c3_ram[i2c3_ram_adr];
    // Daten zum Master senden
    I2C_SendData(I2C3, wert);
    // ram adresse autom. hochzählen
    i2c3_ram_adr++;
  }
  else if(event==I2C_EVENT_SLAVE_BYTE_TRANSMITTED) {
    // Master will noch ein Datenbyte vom Slave auslesen
    i2c3_mode=I2C3_MODE_DATA_BYTE_RD;
    // Daten aus RAM auslesen
    wert=i2c3_ram[i2c3_ram_adr];
    // Daten zum Master senden
    I2C_SendData(I2C3, wert);
    // ram adresse autom. hochzählen
    i2c3_ram_adr++;
  }
  else if(event==I2C_EVENT_SLAVE_STOP_DETECTED) {
    // Master hat die STOP-Sequenz gesendet
    P_I2C3_ClearFlag();
    i2c3_mode=I2C3_MODE_WAITING;
  }
}*/

void I2C3_EV_IRQHandler(void)
{
  uint32_t event;
  uint8_t wert;

  // Event auslesen
  event=I2C_GetLastEvent(I2C3);  

  if(event==I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED) {
    // Master hat die Slave Adresse gesendet
    // um Daten an den Slave zu senden
    i2c3_mode=I2C3_MODE_SLAVE_ADR_WR;
  }
  else if(event==I2C_EVENT_SLAVE_BYTE_RECEIVED) {
    // Master hat ein Byte zum Slave gesendet
	wert=I2C_ReceiveData(I2C3);
	i2c_rx_buffer[i2c_rx_ptr] = wert;
	i2c_rx_ptr++;
	if (wert == 0x00){
		parseLisy(I2C,i2c_rx_buffer);
		i2c_rx_ptr = 0;
	}
  }
  else if(event==I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) {
    // Master hat die Slave Adresse gesendet
    // um Daten vom Slave auszulesen
    i2c3_mode=I2C3_MODE_SLAVE_ADR_RD;
    // Daten aus RAM auslesen
    wert=i2c_tx_buffer[i2c_tx_ptr];
    // Daten zum Master senden
    I2C_SendData(I2C3, wert);
    // ram adresse autom. hochzählen
    i2c_tx_ptr++;
  }
  else if(event==I2C_EVENT_SLAVE_BYTE_TRANSMITTED) {
    // Master will noch ein Datenbyte vom Slave auslesen
    i2c3_mode=I2C3_MODE_DATA_BYTE_RD;
    // Daten aus RAM auslesen
    wert=i2c_tx_buffer[i2c_tx_ptr];
    // Daten zum Master senden
    I2C_SendData(I2C3, wert);
    // ram adresse autom. hochzählen
    i2c_tx_ptr++;
  }
  else if(event==I2C_EVENT_SLAVE_STOP_DETECTED) {
    // Master hat die STOP-Sequenz gesendet
    P_I2C3_ClearFlag();
    i2c3_mode=I2C3_MODE_WAITING;
  }
}


//--------------------------------------------------------------
// ISR (Error Handler vom I2C)
//--------------------------------------------------------------
void I2C3_ER_IRQHandler(void)
{
  if (I2C_GetITStatus(I2C3, I2C_IT_AF)) {
    I2C_ClearITPendingBit(I2C3, I2C_IT_AF);
  }
}
