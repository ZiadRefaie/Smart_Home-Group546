/*
 * TIM1_prog.c
 *
 * Created: 07/09/2024 16:12:12
 *  Author: laila
 */ 
 #include "../../LIB/BIT_MATH.h"
 #include "../../LIB/STD_TYPE.h"
 #include "../../LIB/interrupt_vector.h"

 #include "TIM1_reg.h"
 #include "TIM1_interface.h"
 
 void(*TIM1_PTR[2])(void) = {null};
 
void TIM1_voidInitOVFMode(u8 copy_u8pin){
	  /* SET PRESCALER */
	  TCCR1B_REG &= 0b11111000;
	  TCCR1B_REG |= TIM1_PRESCALER;
	  /* Overflow Mode */
	CLR_BIT(TCCR1A_REG,0);
	CLR_BIT(TCCR1A_REG,1);
	CLR_BIT(TCCR1B_REG,3);
	CLR_BIT(TCCR1B_REG,4);
			/*Interrupt flag*/	
	SET_BIT(TIMSK_REG,2);
		switch(copy_u8pin){
			case OC1A_PIN:
					SET_BIT(TCCR1A_REG,6);
					break;
			case OC1B_PIN:
					SET_BIT(TCCR1A_REG,4);
					break;
		}				
	
}
void TIM1_voidInitCTCMode(u8 copy_u8pin){
	  /* SET PRESCALER */
	  TCCR1B_REG &= 0b11111000;
	  TCCR1B_REG |= TIM1_PRESCALER;
	  /* Overflow Mode */
	  CLR_BIT(TCCR1A_REG,0);
	  CLR_BIT(TCCR1A_REG,1);
	  SET_BIT(TCCR1B_REG,3);
	  SET_BIT(TCCR1B_REG,4);
	  
	  OCR1AL_REG = OCR_Val;
		switch(copy_u8pin){
			case OC1A_PIN:
				SET_BIT(TCCR1A_REG,6);
			break;
			case OC1B_PIN:
				SET_BIT(TCCR1A_REG,4);
			break;
		}
	
}
	
 void TIM1_voidInit10BitfastPWM(u8 copy_u8pin , u8 copy_u8DC){
	 /* 10 bit Fast PWM Mode */

	 SET_BIT(TCCR1A_REG,0);
	 SET_BIT(TCCR1A_REG,1);
	 SET_BIT(TCCR1B_REG,3);
	 CLR_BIT(TCCR1B_REG,4);



	  /* SET PRESCALER */

	  TCCR1B_REG &= 0b11111000;
	  TCCR1B_REG |= TIM1_PRESCALER;

	switch(copy_u8pin){	// Non-inverting

		case OC1A_PIN:	
			CLR_BIT(TCCR1A_REG,6);
			SET_BIT(TCCR1A_REG,7);
			OCR1AL_REG = copy_u8DC *10.24;
			break;
		case OC1B_PIN:
			CLR_BIT(TCCR1A_REG,4);
			SET_BIT(TCCR1A_REG,5);
			OCR1BL_REG = copy_u8DC *10.24;
			break;
	}

 }
 void TIM1_voidFastPWMICR1(void){
	 
	 	/* 14th Wave Generation (Fast PWM ICR1) */
	 	CLR_BIT(TCCR1A_REG,0);
	 	SET_BIT(TCCR1A_REG,1);
	 	SET_BIT(TCCR1B_REG,3);
	 	SET_BIT(TCCR1B_REG,4);

	 	/* SET PRESCALER */
	 	TCCR1B_REG&=0b11111000 ;
	 	TCCR1B_REG|=TIM1_PRESCALER ;

	 	/* Setting PIN OCR1A Timer 1 Non-Inverting */
	 	CLR_BIT(TCCR1A_REG,4);
	 	SET_BIT(TCCR1A_REG,5);

	 	/* TOP Value */
	 	ICR1L_REG = 40000;

 }

 void TIM1_voidServoPWM(u16 copy_u16DC){
	 	OCR1BL_REG = (copy_u16DC * 11.11) + 2000;
								
 }
void TIM1_voidCallBack(void(*ptr)(void),u8 copy_u8Mode){
	switch(copy_u8Mode){
		case OV_MODE:
		TIM1_PTR[0] = ptr;
		break;
		case CTC_MODE:
		TIM1_PTR[1] = ptr;
		break;
	}
}

ISR(TIMER1_OVF){
	TIM1_PTR[0]();
}
ISR(TIMER1_COMPB){
	TIM1_PTR[1]();
}