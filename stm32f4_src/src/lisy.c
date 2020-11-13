/*
 * lisy.c
 *
 *  Created on: 16.04.2020
 *      Author: jamann
 */

#include "main.h"
#include "lisy.h"
#include "stm32_usb_cdc.h"
#include "stm32_i2c_slave.h"
#include "solenoid.h"
#include "lamps.h"
#include "switches.h"
#include "sound.h"
#include "common.h"


void responseString(uint8_t interface, char *ptr){
	switch (interface) {
	case USB:
		USB_CDC_SendString(ptr,NULLBYTE);
	break;
	case I2C:
		I2C_SendString(ptr);
	break;
	default:
	break;
	}
}

void responseByte(uint8_t interface, char byte){
	switch (interface) {
	case USB:
		USB_VCP_DataTx(byte);
	break;
	case I2C:
		I2C_Slave_WriteByte(0x00,byte);
	break;
	default:
	break;
	}
}

void parseLisy(uint8_t interface, uint8_t* buf){

	setWatchdog();
	switch (buf[0]) {								// execute command if complete
		case 0:										// get connected hardware
			responseString(interface,"PIN2RPI");
			break;
		case 1:										// get firmware version
			responseString(interface,VERSION);
			break;
		case 2:										// get API version
			responseString(interface,LISY_API_VERSION_STR);
			break;
		case 3:										// get number of lamps
			responseByte(interface,NUMBEROFLAMPS);
			break;
		case 4:										// get number of solenoids
			responseByte(interface,NUMBEROFSOLENOIDS);
			break;
		case 6:										// get number of displays
			responseByte(interface,NUMBEROFDISPLAYS);
			break;
		case 7:										// Display details
			break;
		case 9:										// get number of switches
			responseByte(interface,NUMBEROFSWITCHES);
			break;
		case 10:									// get status of lamp
			responseByte(interface,lampGet(buf[1]));
			break;
		case 11:									// turn on lamp
			lampOn(buf[1]);
			break;
		case 12:									// turn off lamp
			lampOff(buf[1]);
			break;
		case 19:									// get number of modern lights
			responseByte(interface,NUMBEROFMODERNLIGHTS);
			break;
		case 20:									// get status of solenoid
			solenoidGet(buf[1]);
			break;
		case 21:									// set solenoid # to on
			solenoidOn(buf[1]);
			break;
		case 22:									// set solenoid # to off
			solenoidOff(buf[1]);
			break;
		case 23:									// pulse solenoid
			solenoidPulse(buf[1]);
			break;
		case 24:									// set solenoid pulse time
			sol[buf[1]-1].presetPulseTime = buf[2];
			break;
		case 25:									// set solenoid recycle time
			sol[buf[1]-1].presetRecycleTime = buf[2];
			break;
		case 26:									// pulse and enable solenoid with PWM
			solenoidPulseEnable(buf[1],buf[2],buf[3],buf[4]);
			break;
		case 30:									// set display 0 to (credit display)
			break;
		case 31:									// set display 1 to
			break;
		case 32:									// set display 2 to
			break;
		case 33:									// set display 3 to
			break;
		case 34:									// set display 4 to
			break;
		case 37:									// select display protocol
			break;
		case 40:									// get status of switch #
			responseByte(interface,getSwitch(buf[1]));
			break;
		case 41:									// get changed switches
			responseByte(interface,getNextChangedSwitch());
			break;
		case 50:									// play sound
			soundCmd(buf[1]);
			break;
		case 51:									// stop sound
			stopSound();
			break;
		case 54:									// sound volume setting
			setSoundVol(buf[1]);
			break;
		case 60:									// configure hardware rule for solenoid
			setSolenoidHwRule(buf+1);
			break;
		case 100:									// init
			responseByte(interface,initBoard());
			break;
		case 101:
			responseByte(interface,setWatchdog());
			break;
		default:
			break;
		}
}




