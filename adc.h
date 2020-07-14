//***************************************************************************
//
// Title                :"adc.h"
// Date                 :4/25/2020
// Version              : 1.0
// Target MCU           :saml21j18b
// Target Hardware      ;
// Author               :Ramez Kaupak && Miguel Rivas
/* DESCRIPTION: ADC conversion
support file. Has necessary function and
variables declaration for ADC configuration
main code is in adc.c file
*/
// Revision History     : Initial version
//
//
//**************************************************************************
#ifndef ADC_H_
#define ADC_H_


extern void init_adc(void);
extern int adc_read(void);

#endif /* ADC_H_*/