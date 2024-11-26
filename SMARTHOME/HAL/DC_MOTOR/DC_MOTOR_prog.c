/*
 * DC_MOTOR_prog.c
 *
 * Created: 9/13/2024 2:42:08 PM
 *  Author: AMIT
 */ 

#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPE.h"
#include "../../MCAL/DIO/DIO_interface.h" 
#include "../../MCAL/Timer1/TIM1_interface.h"
#include "DC_MOTOR_config.h"


void DCMOTOR_voidCW(u8 copy_u8dc) {
	
	  /* set pins dir */
	  
	  DIO_voidSetPinDir(H_EN1_PORT,H_EN1_PIN,OUTPUT) ; 
	  DIO_voidSetPinDir(H_A1_PORT,H_A1_PIN,OUTPUT) ;
	  DIO_voidSetPinDir(H_A2_PORT,H_A2_PIN,OUTPUT) ;
	  
	  TIM1_voidInit10BitfastPWM(OC1B_PIN,copy_u8dc) ; 
	  DIO_voidSetPinVal(H_A1_PORT,H_A1_PIN,HIGH) ; 
	  DIO_voidSetPinVal(H_A2_PORT,H_A2_PIN,LOW) ;

	
}
void DCMOTOR_voidACW(u8 copy_u8dc) {
	
	
	 /* set pins dir */
	 
	 DIO_voidSetPinDir(H_EN1_PORT,H_EN1_PIN,OUTPUT) ;
	 DIO_voidSetPinDir(H_A1_PORT,H_A1_PIN,OUTPUT) ;
	 DIO_voidSetPinDir(H_A2_PORT,H_A2_PIN,OUTPUT) ;
	 
	 TIM1_voidInit10BitfastPWM(OC1B_PIN,copy_u8dc) ;
	 DIO_voidSetPinVal(H_A1_PORT,H_A1_PIN,LOW) ;
	 DIO_voidSetPinVal(H_A2_PORT,H_A2_PIN,HIGH) ;
	 
}
void DCMOTOR_voidStop(u8 copy_u8dc){
	
	/* set pins dir */
	
	DIO_voidSetPinDir(H_EN1_PORT,H_EN1_PIN,OUTPUT) ;
	DIO_voidSetPinDir(H_A1_PORT,H_A1_PIN,OUTPUT) ;
	DIO_voidSetPinDir(H_A2_PORT,H_A2_PIN,OUTPUT) ;
	
	TIM1_voidInit10BitfastPWM(OC1B_PIN,copy_u8dc) ;
	DIO_voidSetPinVal(H_A1_PORT,H_A1_PIN,LOW) ;
	DIO_voidSetPinVal(H_A2_PORT,H_A2_PIN,LOW) ;
	
}