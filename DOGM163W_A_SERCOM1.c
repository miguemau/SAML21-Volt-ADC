/*
 *
 *
 * Created 4/8/2020 
 * Author: ramez 
 */



#include "saml21j18b.h"
#include <string.h>

extern unsigned char* ARRAY_PORT_PINCFG0;
extern unsigned char* ARRAY_PORT_PMUX0;

//buffers definition:
unsigned char dsp_buff_1[17];
unsigned char dsp_buff_2[17];
unsigned char dsp_buff_3[17];

unsigned char displayBuff[80];
unsigned char rawBuff[80];

/*
;********************************
;NAME:       init_lcd_dog
;ASSUMES:    nothing
;RETURNS:    nothing
;CALLED BY:  main application
;DESCRITION: inits DOG module LCD display for SPI (serial) operation.
;NOTE:  Can be used as is with MCU clock speeds of 4MHz or less.
;********************************
*/
void init_lcd_dog (void) {
	init_spi_lcd();     //init spi port for LCD
	delay_40mS();       //startup delay
	
	lcd_spi_transmit_CMD(0x39);     //initialize function set #1
	delay_30uS();       //delay for processing
	
	lcd_spi_transmit_CMD(0x39);     //function set #2
	delay_30uS();               //delay for processing
	
	lcd_spi_transmit_CMD(0x1E); //bias set value
	delay_30uS();

	lcd_spi_transmit_CMD(0x55);     //power control, 0x55 nominal for 3.3V
	delay_30uS();                   //0x50 for 5V

	lcd_spi_transmit_CMD(0x6C);     //follower mode on
	delay_40mS();

	lcd_spi_transmit_CMD(0x7F);     //contrast set, 7F for 3.3V, 77 for 5V
	delay_30uS();

	lcd_spi_transmit_CMD(0x0C);     //display on, cursor off, blink off
	delay_30uS();

	lcd_spi_transmit_CMD(0x01);     //clear display, cursor home
	delay_30uS();

	lcd_spi_transmit_CMD(0x06);     //clear display cursor home
	delay_30uS();
}

/*
;********************************
;NAME:       update_lcd_dog
;ASSUMES:    display buffers with display data
;RETURNS:    nothing
;CALLED BY:  main application
;DESCRIPTION: Updates the LCD display lines 1, 2, and 3, using the
;  contents of dsp_buff_1, dsp_buff_2, and dsp_buff_3, respectively.
;********************************
*/

void update_lcd_dog (void) {
	int n=0;
	init_spi_lcd();
	
	lcd_spi_transmit_CMD(0x80); //initialize DDRAM address ctrl
	delay_30uS();

	for (n=0; n < 16; n++)      //loop to push each char in buffer 1 to display
	lcd_spi_transmit_DATA(dsp_buff_1[n]);

	lcd_spi_transmit_CMD(0x90); //initialize DDRAM address ctrl
	delay_30uS();

	for (n=0; n < 16; n++)      //loop to push each char in buffer 1 to display
	lcd_spi_transmit_DATA(dsp_buff_2[n]);
	
	lcd_spi_transmit_CMD(0xA0); //initialize DDRAM address ctrl
	delay_30uS();

	for (n=0; n < 16; n++)      //loop to push each char in buffer 1 to display
	lcd_spi_transmit_DATA(dsp_buff_3[n]);
}



/*
;************************
;NAME:        delay_30uS
;CALLED BY:   init_dsp
;DESCRIPTION: This procedure will generate a fixed delay of just over
;             30 uS
;********************************************************************
*/
void delay_30uS(void) {
int i,j;
for (j = 3; j > 0; j--)
for (i = 0; i < 3; i++)   //~10us
__asm("nop");
}

/*
;************************
;NAME:        delay_40mS
;CALLED BY:   init_dsp
;DESCRIPTION: This procedure will generate a fixed delay of just over
;             40 uS as many times as we define n
;********************************************************************
*/

void delay_40mS(void) {
	int i,j;
	for (j = 40 ; j > 0; j--)
	for (i = 0; i < 307; i++)   //~1ms
	__asm("nop");
}



/*
;************************
;NAME:        init_spi_lcd
;CALLED BY:   init_dsp
DESCRIPTION:  configures the SERCOM before sending the data to the lcd display
;********************************************************************
*/
void init_spi_lcd(void) {
	
	//REG_MCLK_AHBMASK |= 0x00000004;	/* APBC bus clock enabled by default */
	//REG_MCLK_APBCMASK |= 0x00000002;	/* SERCOM1 APBC bus clock enabled by default */
	// Generic clock generator 0, enabled at reset @ 4MHz, is used for peripheral clock
	REG_GCLK_PCHCTRL19 = 0x00000040;	/* SERCOM1 core clock not enabled by default */

	ARRAY_PORT_PINCFG0[16] |= 1;    /* allow pmux to set PA16 pin configuration */
	ARRAY_PORT_PINCFG0[17] |= 1;    /* allow pmux to set PA17 pin configuration */
	ARRAY_PORT_PINCFG0[18] |= 1;    /* allow pmux to set PA18 pin configuration */
	ARRAY_PORT_PINCFG0[19] |= 1;    /* allow pmux to set PA19 pin configuration */
	
	ARRAY_PORT_PMUX0[8] = 0x22;     /* PA16 = MOSI, PA17 = SCK */
	ARRAY_PORT_PMUX0[9] = 0x22;     /* PA18 = SS,   PA19 = MISO */
	
	REG_PORT_DIRSET1 = 0x40;		/* Set PB06 as output */
	REG_PORT_OUTSET1 = 0x40;		/* RS = 1 = data */

	REG_SERCOM1_SPI_CTRLA = 1;              /* reset SERCOM1 */
	while (REG_SERCOM1_SPI_CTRLA & 1) {}    /* wait for reset to complete */
	REG_SERCOM1_SPI_CTRLA = 0x3030000C;     /* MISO-3, MOSI-0, SCK-1, SS-2, SPI master 0011 0000 0011 0000 0000 0000 0000 1100*/
	REG_SERCOM1_SPI_CTRLB = 0x00002000;     /* Master SS, 8-bit */
	REG_SERCOM1_SPI_BAUD = 1;               /* SPI clock is = 1MHz */
	REG_SERCOM1_SPI_CTRLA |= 2;             /* enable SERCOM1 */
	while(!(REG_SERCOM1_SPI_INTFLAG & 1)){} //wait until Tx ready
}


/*
;********************************
;NAME:       lcd_spi_transmit_CMD
;DESCRIPTION: This function sends the command to start receiving data for LCD driver
*/
void lcd_spi_transmit_CMD (unsigned char cmd)
{
	while(!(REG_SERCOM1_SPI_INTFLAG & 1)){} //wait until Tx ready
	REG_PORT_OUTCLR1=0x40;	//RS=0 for command
	//  REG_PORT_OUTCLR0=0x40;	//assert slave select, not needed when MSSEN=1
	REG_SERCOM1_SPI_DATA=cmd;
	while(!(REG_SERCOM1_SPI_INTFLAG & 1)){} //wait until Tx ready
	//	REG_PORT_OUTSET0 = 0x40000;	//unassert slave select, not needed when MSSEN=1
}

/*
;********************************
;NAME:       lcd_spi_transmit_DATA
;DESCRIPTION: This procedure will send a data frame to LCD driver
*/
void lcd_spi_transmit_DATA (unsigned char data)
{
	while(!(REG_SERCOM1_SPI_INTFLAG & 1)){} //wait until Tx ready
	REG_PORT_OUTSET1=0x40;	//RS=1 for data
	//  REG_PORT_OUTCLR0=0x40;	//assert slave select, not needed when MSSEN=1
	REG_SERCOM1_SPI_DATA= data;
	while(!(REG_SERCOM1_SPI_INTFLAG & 1)){} //wait until Tx ready
	//	REG_PORT_OUTSET0 = 0x40000;	//unassert slave select, not needed when MSSEN=1
}

void lcd_escape(unsigned char *rawBuff, unsigned char *dispBuff, unsigned char *line1, unsigned	char *line2, unsigned char *line3) {
	
	
	
	for (int i=0, j=0; i<80; i++, j++) {	//loop through buffer
		unsigned char tempChar = rawBuff[i];
		if (tempChar == '\b') {					// \b backspace
			j--;								//decrement index
			dispBuff[j] = rawBuff[i+1];			//copy element after \b
			i++;								//i pointing to \b + 1
		}
		else if (tempChar == '\n') {			// \n newline
			j+=15;								//skip to next line
			if (j > 47) {
				j -= 48;						//if past 3rd line, return to first
			}
			dispBuff[j] = rawBuff[i+1];			//copy next element after \n
			i++;								// \n + 1
		}
		else if (tempChar == '\f') {					// \f form feed, new page
			j=0;								//reset j index to start
			memset(dispBuff, ' ', 80);			//clear dispBuff (new page)
			dispBuff[j] = rawBuff[i+1];			//copy next element
			i++;								//escape char+1
		}
		else if (tempChar == '\r') {		//carriage return
			int mod_j = j%16;				//determining position on line
			if (mod_j != 0) {
				j -= mod_j;					//return to start of line
			}
			dispBuff[j] = rawBuff[i+1];
			i++;
		}
		else {
			dispBuff[j] = rawBuff[i];		//no escape char, just copy
		}
	}
	for (int temp = 0; temp<16; temp++){
		line1[temp] = dispBuff[temp];
		line2[temp] = dispBuff[temp+16];
		line3[temp] = dispBuff[temp+32];
	}
}






/*;************************
;NAME:      clr_dsp_buffs
;FUNCTION:  Initializes dsp_buffers 1, 2, and 3 with blanks (0x20)
;ASSUMES:   Three CONTIGUOUS 16-byte arrays named
;           dsp_buff_1, dsp_buff_2, dsp_buff_3.
;RETURNS:   nothing.
;CALLS:     none
;CALLED BY: main application and diagnostics
;********************************************************************
*/

//hording this clearing funtion for later:
/*
void clr_dsp_buffs (void){
	int i;
	for (i=0; i < 17; i++) {
		dsp_buff_1[i]= ' ';
		dsp_buff_2[i]= ' ';
		dsp_buff_3[i]= ' ';
	}
}

*/
