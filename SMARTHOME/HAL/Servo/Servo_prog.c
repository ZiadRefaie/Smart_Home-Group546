/*
 * Servo_prog.c
 *
 * Created: 13/09/2024 11:26:48 م
 *  Author: zidrar
 */ 
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPE.h"

#include "../../MCAL/Timer1/TIM1_interface.h"

#include "Servo_interface.h"
#include "Servo_config.h"


void SERVO_voidInit(){
	
	/* 14th Wave Generation (Fast PWM ICR1) */
	CLR_BIT(TCCR1A_REG,0);
	SET_BIT(TCCR1A_REG,1);
	SET_BIT(TCCR1B_REG,3);
	SET_BIT(TCCR1B_REG,4);

	/* SET PRESCALER */
	TCCR1B_REG&=0b11111000 ;
	TCCR1B_REG|=TIM1_PRESCALER ;

	/* Setting PIN OCR1A Timer 1 Non-Inverting */
	CLR_BIT(TCCR1A_REG,6);
	SET_BIT(TCCR1A_REG,7);

	/* TOP Value */
	ICR1L_REG = 5100;
	
}

void SERVO_voidMap(u8 copy_u8Angel){
	// Ensure the angle is within the range of 0 to 180 degrees
	if (copy_u8Angel > 180) {
		copy_u8Angel = 180;
	}

	// Map the angle to the corresponding pulse width
	// Pulse width range: 1ms (1000us) to 2ms (2000us)
	f32 local_f32PulseWidth = ((copy_u8Angel * 11.1) + 1000);

	// Set OCR1A for the desired pulse width
	OCR1AL_REG = 1000 + local_f32PulseWidth;

	
}