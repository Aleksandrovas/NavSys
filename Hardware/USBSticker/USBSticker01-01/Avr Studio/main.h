
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#define LED1_ON			PORTD &= ~(1<<PD1)
#define LED1_OFF		PORTD |= (1<<PD1)  
#define LED2_ON			PORTD &= ~(1<<PD2) 
#define LED2_OFF		PORTD |= (1<<PD2) 

#define nIRQ			PB4
#define nIRQ_PIN		(PINB&(1<<nIRQ))

#endif
