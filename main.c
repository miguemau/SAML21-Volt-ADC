//***************************************************************************
//
// Title                :"ese_381_lab8_task3"
// Date                 :4/25/2020
// Version              : 1.0
// Target MCU           :saml21j18b
// Target Hardware      ;temperature sensor
// Author               :Ramez Kaupak && Miguel Rivas
/* DESCRIPTION: The program polls the push button
sw0 on the board and displays results of adc 
conversion and calculated values of voltage or temperature. 
// Revision History     : Initial version
//
*/
//**************************************************************************
#include "saml21j18b.h"
#include "sys_support.h"
#include "lcd.h"
#include "stdio.h"
#include "adc.h"
#include "math.h"

 unsigned char* ARRAY_PORT_PINCFG0=(unsigned char*)&REG_PORT_PINCFG0;
 unsigned char* ARRAY_PORT_PMUX0=(unsigned char*)&REG_PORT_PMUX0;

//#include "rs232.h"------------used in lab 7 for tera_term has rs232_sercom4.c file
//********************************************


int main (void) {
	
	//configuring sw0 button
	REG_PORT_DIRCLR0=0x4;		//make PA02 input for SW0
	ARRAY_PORT_PINCFG0[2]|=6;	//Enable PA02 input buffer with pull up
	REG_PORT_OUTSET0=0x4;		//make PA10 pull-up
	
	init_lcd_dog();
	update_lcd_dog();
	init_adc();
	
	int state_status=0;//initial state is 0(voltage measurement)
	
	while (1)
	{
		if (!(REG_PORT_IN0 & 0x00000004)&& !state_status) //check switch status, check if pressed (1)
		{
			state_status=1;
		}
		
		else if (!(REG_PORT_IN0 & 0x00000004)&&state_status)
		{
			state_status=0;
		}
		
//after the state is determined, output is selected
			if (state_status==0)
				{
					//reading the voltage
					REG_ADC_INPUTCTRL=0x1804;	//V-=GND;V+=AIN4
					int result1=adc_read();
					float voltage_analog=result1*3.3/4096;
					sprintf(rawBuff, "AD Result=%4X \n \rVolts = %.3fV", result1, voltage_analog);
					lcd_escape(rawBuff, displayBuff, dsp_buff_1, dsp_buff_2, dsp_buff_3);
					update_lcd_dog();
					
					for (int i=0; i<4;i++)
					delay_40mS();
				}
			else
				{
					//reading the temperature
					REG_ADC_INPUTCTRL=0x1805;	//V-=GND;V+=AIN5
					int result1=adc_read();
					float temperature=result1*330/4096;
					sprintf(rawBuff, "AD Result=%4X \n \rTemp = %3.1fF ", result1, temperature);
					lcd_escape(rawBuff, displayBuff, dsp_buff_1, dsp_buff_2, dsp_buff_3);
					update_lcd_dog();
					
					for (int i=0; i<4;i++)
					delay_40mS();
				}	
	}	
}