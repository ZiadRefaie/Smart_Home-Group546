/*
 * SMART_HOME.c
 *
 * Created: 03/10/2024 11:56:20 ص
 */

/* LIB Includes */
#include "../LIB/BIT_MATH.h"

#include "../LIB/STD_TYPE.h"

/* HAL Includes */
#include "../HAL/LCD/LCD_interface.h"

#include "../HAL/KEYPAD/KEYPAD_interface.h"

#include "../HAL/Servo/Servo_interface.h"

#include "../HAL/EXTEEPROM/EXTEEPROM_interface.h"

/* MCAL Includes */
#include "../MCAL/DIO/DIO_interface.h"

#include "../MCAL/GI/GI_interface.h"

#include "../MCAL/ADC/ADC_interface.h"

#include "../MCAL/UART/UART_interface.h"

#include "../MCAL/Timer0/TIM0_interface.h"

#include "../MCAL/Timer1/TIM1_interface.h"

#include "../MCAL/Timer2/TIM2_interface.h"

/* Internal Libraries */
#define F_CPU 16000000UL
#include <util/delay.h>

#include "System.h"


int main(void) {
//////////////////////////{initialization section}/////////////////////////// 
	Init();
//////////////////////////////////////////////////////////////////////////


///////////////////////////////{start menu}////////////////////////////////
	 LCD_voidSendCommand(1); /*----CLR "LCD"----*/
     LCD_voidSendString((u8*)"===SMART HOME===");
/////////////////////////////////////////////////////////////////////////
	  while (1) {
		  
			  User_Mode(); // get InPut from keypad 
			  
			  LCD_voidSendCommand(1);
			  LCD_voidSendString((u8*)"Checking");
			  _delay_ms(500);
			  LCD_voidSendString((u8*)"...");
			  
			  User_Input_Bluetooth(user_input);
	  }
}