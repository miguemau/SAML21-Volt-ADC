//***************************************************************************
//
// Title                :"lcd.h"
// Date                 :4/25/2020
// Version              : 1.0
// Target MCU           :saml21j18b
// Target Hardware      ;
// Author               :Ramez Kaupak && Miguel Rivas
/* DESCRIPTION: lcd initialization and configuration 
include file SPrintf and printf enhancement.
Has necessary function and
variables declaration for lcd display
*/
// Revision History     : Initial version
//
//
//**************************************************************************

#ifndef LCD_H_
#define LCD_H_


//Display buffer for DOG LCD
extern char dsp_buff_1[17];
extern char dsp_buff_2[17];
extern char dsp_buff_3[17];

extern unsigned char displayBuff[80];
extern unsigned char rawBuff[80];

extern void init_lcd_dog (void);
extern void update_lcd_dog (void);

//Formatted output function
extern void lcd_escape(unsigned char *rawBuff, unsigned char *dispBuff, unsigned char *line1, unsigned	char *line2, unsigned char *line3);


#endif /* LCD_H_*/
