/*
 * TIM2_prog.c
 *
 * Created: 08/09/2024 11:35:44 ص
 *  Author: zidrar
 */ 
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPE.h"
#include "../../LIB/interrupt_vector.h"
#include "TIM2_interface.h"
#include "TIM2_reg.h"

void(*TIM2_PTR[2])(void) = {null};

void TIM2_voidInit(u8 copy_u8Mode){
	/* set PRESCALER */
	TCCR2_REG &= 0b11111000;
	TCCR2_REG |= TIM2_PRESCALER;
	
	switch(copy_u8Mode){
		case OV_MODE:
				CLR_BIT(TCCR2_REG,6);
				CLR_BIT(TCCR2_REG,3);
				/* Enable Timer2 Overflow Interrupt */
				SET_BIT(TIMSK_REG,6);
				break;
		case CTC_MODE:
				SET_BIT(TCCR2_REG,3);
				CLR_BIT(TCCR2_REG,6);
				/* Enable Timer2 CTC Interrupt */
				SET_BIT(TIMSK_REG,7);
				OCR2_REG = OCR2_VAL;
				break;
		case PHASE_PWM:
				CLR_BIT(TCCR2_REG,3);
				SET_BIT(TCCR2_REG,6);
				break;
		case FAST_PWM:
				SET_BIT(TCCR2_REG,3);
				SET_BIT(TCCR2_REG,6);
				break;
	}
	
}

void TIM2_voidPWM(u8 copy_u8Mode,u8 copy_u8DC){
	switch(copy_u8Mode){
		case FAST_PWM:
				#if PWM_MODE == INVERTING
					SET_BIT(TCCR2_REG,4);
					SET_BIT(TCCR2_REG,5);
					OCR2_REG = (1-copy_u8DC) *2.56;
				#elif PWM_MODE == NON_INVERTING
					CLR_BIT(TCCR2_REG,4);
					SET_BIT(TCCR2_REG,5);
					OCR2_REG = copy_u8DC * 2.56 ;
				#endif
				break;
		case PHASE_PWM:
				#if PWM_MODE == INVERTING
					SET_BIT(TCCR2_REG,4);
					SET_BIT(TCCR2_REG,5);
					OCR2_REG = (1-copy_u8DC) *2.56;
				#elif PWM_MODE == NON_INVERTING
					CLR_BIT(TCCR2_REG,4);
					SET_BIT(TCCR2_REG,5);
					OCR2_REG = copy_u8DC * 2.56 ;
				#endif
				break;
	}
}

void TIM2_voidCallBack(void(*ptr)(void),u8 copy_u8Mode){
	switch(copy_u8Mode){
		case OV_MODE:
		TIM2_PTR[0] = ptr;
		break;
		case CTC_MODE:
		TIM2_PTR[1] = ptr;
		break;
	}
}

ISR(TIMER2_OVF){
	TIM2_PTR[0]();
}
ISR(TIMER2_COMP){
	TIM2_PTR[1]();
}