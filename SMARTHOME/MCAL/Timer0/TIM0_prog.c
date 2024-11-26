/*
 * Timer_prog.c
 *
 * Created: 06/09/2024 16:27:58
 *  Author: laila
 */ 
 #include "../../LIB/BIT_MATH.h"
 #include "../../LIB/STD_TYPE.h"
 #include "../../LIB/interrupt_vector.h"
 #include "TIM0_reg.h"
 #include "TIM0_interface.h"

void(*TIM0_PTR[2])(void) = {null};

 void TIM0_voidInit(u8 copy_u8Mode){
	/* set PRESCALER */

	TCCR0_REG &= 0b1111100;
	TCCR0_REG |= TIM0_PRESCALER;

	switch(copy_u8Mode){
	
	case OV_MODE:
		/* OverFlow Mode */

		CLR_BIT(TCCR0_REG,3);
		CLR_BIT(TCCR0_REG,6);

		/* Enable Timer0 Overflow Interrupt */

		SET_BIT(TIMSK_REG,0);

		break;
	case CTC_MODE:
		/* CTC MODE */

		SET_BIT(TCCR0_REG,3);
		CLR_BIT(TCCR0_REG,6);

		/* Enable Timer0 CTC Interrupt */

		SET_BIT(TIMSK_REG,1);
		
		/* OCR Value */

		OCR0_REG = OCR_VAL;
		break;	
	default:break;
	}

 }
 
 void TIM0_voidCallBack(void(*ptr)(void),u8 copy_u8Mode){
	 switch(copy_u8Mode){
	 case OV_MODE:
		TIM0_PTR[0] = ptr;
		break;
	case CTC_MODE:
		TIM0_PTR[1] = ptr;
		break;  
		}
	}

void TIM0_voidFastPWM(u8 copy_DC,u8 copy_u8Mode){
 /* FAST PWM MODE */
 SET_BIT(TCCR0_REG, 3); // WGM01
 SET_BIT(TCCR0_REG, 6); // WGM00
 
 /* PRESCALER_SET */
 TCCR0_REG &= 0b11111000;
 TCCR0_REG |= TIM0_PRESCALER;

 /* Ensure OCR0 is set to output */
 SET_BIT(TCCR0_REG, 5); // COM01 = 1
 if (copy_u8Mode == NON_INVERTING) {
	 CLR_BIT(TCCR0_REG, 4); // COM00 = 0
	 } else if (copy_u8Mode == INVERTING) {
	 SET_BIT(TCCR0_REG, 4); // COM00 = 1
 }

 /* Set duty cycle */
 if (copy_DC > 100) copy_DC = 100; // Clamp to 100%
 if (copy_u8Mode == NON_INVERTING) {
	 OCR0_REG = (copy_DC * 255) / 100;
	 } else { // Inverting mode
	 OCR0_REG = ((100 - copy_DC) * 255) / 100;
 }
}
 
 ISR(TIMER0_OVF){
	 
	 TIM0_PTR[0]();
 }

  ISR(TIMER0_COMP){
	  
	  TIM0_PTR[1]();
  }