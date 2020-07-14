/*
 *
 *
 * Created 4/22/2020 
 * Author: ramez 
 *
 *
 ********************************************
 Miguel Rivas & Ramez Kaupak
 ESE381 L02
 Lab 8: ADC conversion
 p7-1.c A to D conversion of channel 5
 This program converts the analog input from channel 4.
 Channel 4 is configured as signle-ended input from PA04.
 External reference A is used from PA03. Clock prescaler
 is divided by 16 and sampling time is set to 5 clock cycles(20us)
 Software trigger is used. 

 ********************************************
 */
 
#include "saml21j18b.h"


extern unsigned char* ARRAY_PORT_PINCFG0;
extern unsigned char* ARRAY_PORT_PMUX0;


void init_adc(void){
	REG_GCLK_PCHCTRL30=0x40;
	//REG_PM_APBCMASK |=0x10000;	//enable bus clock for ADC
	//REG_GCLK_CLKCTRL=0x401E;	//GCLK0 to ADC
	REG_ADC_CTRLB=0x03;			//prescaler=div16
	REG_ADC_SAMPCTRL=5;			//sampling time 5 clocks
	ARRAY_PORT_PINCFG0[3]|=1;	//use PMUX options fro PA03
	ARRAY_PORT_PMUX0[1]=0x10;	//PA03=VREFa
	
	REG_ADC_REFCTRL=5;			//use VDDANA
	//REG_ADC_REFCTRL=3;		//use VREFa and REF192
	
	REG_ADC_INPUTCTRL=0x1804;	//V-=GND;V+=AIN4
	ARRAY_PORT_PINCFG0[4]|=1;	//use PMUX functions for PA04
	ARRAY_PORT_PMUX0[2]=0x01;	//PA04=ANALOG IN4
	REG_ADC_CTRLA=2;			//enable ADC
	
}

int adc_read(void){
	int reading=0;
	REG_ADC_SWTRIG=2;			//start a conversion
	while(!(REG_ADC_INTFLAG & 1));//wait for conversion to complete
	reading=REG_ADC_RESULT;
	return reading;
}


