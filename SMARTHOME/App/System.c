/*
 * System.c
 *
 * Created: 26/11/2024 11:37:28 ص
 *  Author: zidrar
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

#define F_CPU 16000000UL

#include <util/delay.h>

#include "System.h"

////////// some variables that we use ///////////////////
/*--------------------passwords-----------------------*/
u8 Diffualt[4] ={1,2,3,4}; // default password for admin
u8 user1[4]={1,1,1,1}; // user 1 password
u8 user2[4]={2,2,2,2}; // user 2 password
u8 user3[4]={3,3,3,3}; // user 3 password
u8 user4[4]={4,4,4,4}; // user 4 password
/*------------------------------------------------*/
u8 Number_of_Tries = 0; // you have only 3 trial
u8 Remote_State =0; 
u8 user_input[4]; // input password from user keypad
u16 Password=0;
u8 led_on=0;
////////////////////////////////////////////////////////

void Init(void) {

   /* Keypad Init */
   KEYPAD_voidInit();
   
   /* LCD Init */
   DIO_voidSetPortDir(DIO_PORTA, 0xF0); // 1111 0000 -> in port A
   DIO_voidSetPortDir(DIO_PORTB, 0x0f); // 0000 1111 -> in port B
   LCD_voidInit();

   /* UART Init */
   UART_voidInit();
   GI_voidEnable(); // global interrupt 
   
   /* ADC */
   ADC_voidInit();
   DIO_voidSetPinDir(DIO_PORTA,DIO_PIN0,INPUT);
   
   /* Motors Pins */
   DIO_voidSetPinDir(DIO_PORTB,DIO_PIN3,OUTPUT); // AC Pin
   DIO_voidSetPinDir(DIO_PORTD,DIO_PIN5,OUTPUT); // Servo Pin
   SERVO_voidInit();

   
   /* Led Pin layout */	
   DIO_voidSetPinDir(DIO_PORTD,DIO_PIN2,OUTPUT); // Lamp 1 OUTPUT
   DIO_voidSetPinDir(DIO_PORTD,DIO_PIN3,OUTPUT); // Lamp 2 OUTPUT
   DIO_voidSetPinDir(DIO_PORTB,DIO_PIN4,OUTPUT); // Lamp 3 OUTPUT
   DIO_voidSetPinDir(DIO_PORTB,DIO_PIN5,OUTPUT); // Lamp 4 OUTPUT
   DIO_voidSetPinDir(DIO_PORTB,DIO_PIN6,OUTPUT); // Lamp 5 OUTPUT
   
   /* Dimmer circuit */
   DIO_voidSetPinDir(DIO_PORTB,DIO_PIN7,OUTPUT);

}

u8 Key(void) { // Keypad 
	u8 Key_input = 0xff;
	do {
		Key_input = KEYPAD_u8GetKey();
	} while (Key_input == 0xff);

	return Key_input;
}

void System_Lock(void) {
   static u8 data1[] = {0b01100,0b00110,0b00011,0b11111,0b00000,0b00000,0b00000,0b00000};
   static u8 data2[] = {0b00000,0b00000,0b00001,0b00011,0b00110,0b01100,0b11000,0b00000};
   static u8 data5[] = {0b00100,0b00100,0b00100,0b00100,0b00100,0b00100,0b00100,0b00000};
   static u8 data3[] = {0b00000,0b00000,0b00000,0b11111,0b00101,0b00111,0b00000,0b00000};
   static u8 data4[] = {0b00000,0b00000,0b00000,0b01011,0b01010,0b01010,0b01110,0b00000};

   LCD_voidSendCommand(1);
   LCD_voidDrawData(0, data1, 10, 0);
   LCD_voidDrawData(1, data2, 9, 0);
   LCD_voidDrawData(4, data5, 8, 0);
   LCD_voidDrawData(2, data3, 7, 0);
   LCD_voidDrawData(3, data4, 6, 0);
   LCD_voidGotoxy(5, 0);
   LCD_voidSendData('!');
   LCD_voidGotoxy(4, 0);
   LCD_voidSendData('!');
   Number_of_Tries = 0;
   DIO_voidSetPinVal(DIO_PORTA, DIO_PIN3, HIGH); // buzzer sound on 
  while ('C' != Key());
  DIO_voidSetPinVal(DIO_PORTA, DIO_PIN3, LOW); // buzzer sound off
}

u32 Temperature(void){ // Temperature Calculation  
	
	u16 val = ADC_u16ReadChannel(ADC_0);
	f32 Temp =(f32)val*500/1023 ;
	_delay_ms(30);
	return (u32)Temp;
}

u8 Velocity(void){ // AC Control
		if (Temperature() >= 28)
		{
			TIM0_voidFastPWM(60,FAST_PWM);
			return 60;
		}else{
			TIM0_voidFastPWM(0,FAST_PWM);
			return 0;
			}
}

void User_Panel(u8 Button){ // see temperature - lamp control - dimmer control
		LCD_voidSendCommand(1);
	u8 First_Entry=0;
	while (1){ // start of while loop
		if (First_Entry)
		{
			LCD_voidSendCommand(1);
			LCD_voidGotoxy(0,0);
			LCD_voidSendString((u8*)"D-Door");
			LCD_voidGotoxy(7,0);
			LCD_voidSendString((u8*)"L-Lamps"); // Add Lamp control
			LCD_voidGotoxy(0, 1);
			LCD_voidSendString((u8*)"T-Temp");
			LCD_voidGotoxy(7,1);
			LCD_voidSendString((u8*)"Q-Dimmer");
			Button = UART_u8Rx();
		}
		//////////////////////////////////////////{door}///////////////////////////////////////////////
		if ( 'D' == Button){ // Door Control
			First_Entry=1;
			
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Access Denied");

			Button = 0;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////
		
		
		//////////////////////////////////////////{Temperature}///////////////////////////////////////////////
		else if( Button == 'T'){ // Temperature& AC Display
			First_Entry=1;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Temp:");
			LCD_voidGotoxy(0,1);
			LCD_voidSendString((u8*)"AC:");
			do{
				LCD_voidGotoxy(5,0);
				LCD_voidSendNumber(Temperature());
				LCD_voidGotoxy(3,1);
				LCD_voidSendNumber(Velocity());
				LCD_voidGotoxy(6,1);
				LCD_voidSendString((u8*)"          ");
			} while (Button == 'T');
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////{Lamp Control}////////////////////////////////////////////
		else if(Button =='L'){ // LED control
			First_Entry=1;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"LED Control:");
			LCD_voidGotoxy(0,1);
			LCD_voidSendString((u8*)"1: ON / 2: OFF");
			u8 LedControl = UART_u8Rx(); // choose if you want to open led or not
			
			if (LedControl =='1'){

				LCD_voidSendCommand(1);
				LCD_voidSendString((u8*)"Enter LED to be ON:");
				u8 ledOn = UART_u8Rx(); // Read LED number to turn On
				switch (ledOn) {
					case '1':
					DIO_voidSetPinVal(DIO_PORTD, DIO_PIN2, HIGH); // LED 1 ON
					break;
					case '2':
					DIO_voidSetPinVal(DIO_PORTD, DIO_PIN3, HIGH); // LED 2 ON
					break;
					case '3':
					DIO_voidSetPinVal(DIO_PORTB, DIO_PIN4, HIGH); // LED 3 ON
					break;
					case '4':
					DIO_voidSetPinVal(DIO_PORTB, DIO_PIN5, HIGH); // LED 4 ON
					break;
					case '5':
					DIO_voidSetPinVal(DIO_PORTB, DIO_PIN6, HIGH); // LED 5 ON
					break;
					default:
					LCD_voidSendCommand(1);
					LCD_voidSendString((u8*)"Invalid LED");
				}
				LCD_voidSendCommand(1);
				LCD_voidSendString((u8*)"Lamp ");
				LCD_voidSendData(ledOn);
				_delay_ms(200);
			}
			if(LedControl == '2' ){
				LCD_voidSendCommand(1);
				LCD_voidSendString((u8*)"Enter LED to OFF:");
				u8 ledOff = UART_u8Rx(); // Read LED number to turn OFF
				switch (ledOff) {
					case '1':
					DIO_voidSetPinVal(DIO_PORTD, DIO_PIN2,LOW); // Turn OFF LED 1
					break;
					case '2':
					DIO_voidSetPinVal(DIO_PORTD, DIO_PIN3,LOW); // Turn OFF LED 2
					break;
					case '3':
					DIO_voidSetPinVal(DIO_PORTB, DIO_PIN4,LOW); // Turn OFF LED 3
					break;
					case '4':
					DIO_voidSetPinVal(DIO_PORTB, DIO_PIN5,LOW); // Turn OFF LED 4
					break;
					case '5':
					DIO_voidSetPinVal(DIO_PORTB, DIO_PIN6,LOW); // Turn OFF LED 5
					break;
					default:
					LCD_voidSendCommand(1);
					LCD_voidSendString((u8*)"Invalid LED");
				}
				LCD_voidSendCommand(1);
				LCD_voidSendString((u8*)"Lamp ");
				LCD_voidSendData(ledOff);
				_delay_ms(200);
			}
			Button = 0;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////{dimmer control}//////////////////////////////////////////
		else if(Button == 'Q'){ // dimmer section
				First_Entry=1;
				u8 led_state;
				for (led_state=1;led_state<3;led_state++){
					if (led_on==0){
						LCD_voidSendCommand(1);
						LCD_voidGotoxy(0,0);
						LCD_voidSendString((u8*)"Dimmer circuit");
						LCD_voidGotoxy(0,1);
						LCD_voidSendString((u8*)"ON");
						DIO_voidSetPinVal(DIO_PORTB,DIO_PIN7,HIGH);
						_delay_ms(500);
						led_on=1;
						break;
					}
				 else if(led_on==1){
						LCD_voidSendCommand(1);
						LCD_voidGotoxy(0,0);
						LCD_voidSendString((u8*)"Dimmer circuit");
						LCD_voidGotoxy(0,1);
						LCD_voidSendString((u8*)"OFF");
						DIO_voidSetPinVal(DIO_PORTB,DIO_PIN7,LOW);
						_delay_ms(500);
						led_on=0;
						break;
					}
				}
				Button=0;
			}
			if(Button == 'C')// exits
			{
				break;
			}
		}
	} // end function

void Admin_Panel(u8 Button){ // admin can open the door and see the temp and can delete or add users
			u8 First_Entry=0;
			while (1){ // start of while loop
				if (First_Entry)
				{
					LCD_voidSendCommand(1);
					LCD_voidGotoxy(0,0);
					LCD_voidSendString((u8*)"D-Door");
					LCD_voidGotoxy(7,0);
					LCD_voidSendString((u8*)"L-Lamps"); // Add Lamp control
					LCD_voidGotoxy(0, 1);
					LCD_voidSendString((u8*)"T-Temp");
					LCD_voidGotoxy(7,1);
					LCD_voidSendString((u8*)"Q-Dimmer");
					Button = UART_u8Rx();
				}
			//////////////////////////////////////////{door}///////////////////////////////////////////////
				if ( 'D' == Button){ // Door Control
					First_Entry=1;

					LCD_voidSendCommand(1);
					LCD_voidSendString((u8*)"Door opening.");
					SERVO_voidMap(90);
					
					_delay_ms(400);
					LCD_voidSendCommand(1);
					LCD_voidSendString((u8*)"Door Closing");
					SERVO_voidMap(0);

					Button = 0;
				}
			/////////////////////////////////////////////////////////////////////////////////////////////
			
			
			//////////////////////////////////////////{Temperature}///////////////////////////////////////////////
				else if( Button == 'T'){ // Temperature& AC Display
					First_Entry=1;
					LCD_voidSendCommand(1);
					LCD_voidSendString((u8*)"Temp:");
					LCD_voidGotoxy(0,1);
					LCD_voidSendString((u8*)"AC:");
					do
					{
						LCD_voidGotoxy(5,0);
						LCD_voidSendNumber(Temperature());
						LCD_voidGotoxy(3,1);
						LCD_voidSendNumber(Velocity());
						LCD_voidGotoxy(6,1);
						LCD_voidSendString((u8*)"          ");
					} while (Button == 'T');
				}
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//////////////////////////////////////////{Lamp Control}////////////////////////////////////////////
				else if(Button =='L'){ // LED control
				First_Entry=1;
				LCD_voidSendCommand(1);
				LCD_voidSendString((u8*)"LED Control:");
				LCD_voidGotoxy(0,1);
				LCD_voidSendString((u8*)"1: ON / 2: OFF");
				u8 LedControl = UART_u8Rx(); // choose if you want to open led or not
				
					if (LedControl =='1'){
						LCD_voidSendCommand(1);
						LCD_voidSendString((u8*)"Enter LED to be ON:");
						u8 ledOn = UART_u8Rx(); // Read LED number to turn On
							switch (ledOn) {
								case '1':
									DIO_voidSetPinVal(DIO_PORTD, DIO_PIN2, HIGH); // LED 1 ON
									break;
								case '2':
									DIO_voidSetPinVal(DIO_PORTD, DIO_PIN3, HIGH); // LED 2 ON
									break;
								case '3':
									DIO_voidSetPinVal(DIO_PORTB, DIO_PIN4, HIGH); // LED 3 ON
									break;
								case '4':
									DIO_voidSetPinVal(DIO_PORTB, DIO_PIN5, HIGH); // LED 4 ON
									break;
								case '5':
									DIO_voidSetPinVal(DIO_PORTB, DIO_PIN6, HIGH); // LED 5 ON
									break;
								default:
								LCD_voidSendCommand(1);
								LCD_voidSendString((u8*)"Invalid LED");
							}
							LCD_voidSendCommand(1);
							LCD_voidSendString((u8*)"Lamp ");
							LCD_voidSendData(ledOn);
							_delay_ms(200);
					}
					if(LedControl == '2' ){ 
						LCD_voidSendCommand(1);
						LCD_voidSendString((u8*)"Enter LED to OFF:");
						u8 ledOff = UART_u8Rx(); // Read LED number to turn OFF
							switch (ledOff) {
								case '1':
									DIO_voidSetPinVal(DIO_PORTD, DIO_PIN2,LOW); // Turn OFF LED 1
									break;
								case '2':
									DIO_voidSetPinVal(DIO_PORTD, DIO_PIN3,LOW); // Turn OFF LED 2
									break;
								case '3':
									DIO_voidSetPinVal(DIO_PORTB, DIO_PIN4,LOW); // Turn OFF LED 3
									break;
								case '4':
									DIO_voidSetPinVal(DIO_PORTB, DIO_PIN5,LOW); // Turn OFF LED 4
									break;
								case '5':
									DIO_voidSetPinVal(DIO_PORTB, DIO_PIN6,LOW); // Turn OFF LED 5
									break;
							 default:
								 LCD_voidSendCommand(1);
								 LCD_voidSendString((u8*)"Invalid LED");
						}	
						LCD_voidSendCommand(1);
						LCD_voidSendString((u8*)"Lamp ");
						LCD_voidSendData(ledOff);
						_delay_ms(200);
					}
				
				Button = 0;	
				}
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//////////////////////////////////////////{dimmer control}//////////////////////////////////////////
				else if(Button == 'Q'){ // dimmer section
						First_Entry=1;
						u8 led_state;
						for (led_state=1;led_state<3;led_state++){ 
							if (led_on==0){
								LCD_voidSendCommand(1);
								LCD_voidGotoxy(0,0);
								LCD_voidSendString((u8*)"Dimmer circuit");
								LCD_voidGotoxy(0,1);
								LCD_voidSendString((u8*)"ON");
								DIO_voidSetPinVal(DIO_PORTB,DIO_PIN7,HIGH);
								_delay_ms(500);
								led_on=1;
								break;
							}
							else if(led_on==1){
								LCD_voidSendCommand(1);
								LCD_voidGotoxy(0,0);
								LCD_voidSendString((u8*)"Dimmer circuit");
								LCD_voidGotoxy(0,1);
								LCD_voidSendString((u8*)"OFF");
								DIO_voidSetPinVal(DIO_PORTB,DIO_PIN7,LOW);
								_delay_ms(500);
								led_on=0;
								break;
								
							}
						}
						
						Button=0;
						
					}
					
				if(Button == 'C')// exits
				{ 
					break;
				}
				
			}
		
}

void User_Input_Bluetooth(u8* Input){ // with UART protocol. 
	LCD_voidSendCommand(1);
	_delay_ms(100);
	u8 Login_State = 0; // 0 = not logged  1 = Logged 
	while(1){
		
		/* Admin Checker */
		if (Input[0] == Diffualt[0]  && Input[1] == Diffualt[1]  && Input[2] == Diffualt[2]  && Input[3] == Diffualt[3]) {
			Login_State= 1;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, Admin");
			_delay_ms(500);
			LCD_voidSendCommand(1);
			LCD_voidGotoxy(0,0);
			LCD_voidSendString((u8*)"D-Door"); // Displays door Options Close/Open
			LCD_voidGotoxy(7,0);
			LCD_voidSendString((u8*)"L-Lamps"); // Displays Lamps Options ON/OFF
			LCD_voidGotoxy(0, 1);
			LCD_voidSendString((u8*)"T-Temp"); // Displays Temperature / AC power
			LCD_voidGotoxy(7,1);
			LCD_voidSendString((u8*)"Q-Dimmer"); // Displays Dimmer control ON/OFF
			
			u8 User_Mobile = UART_u8Rx();
			Admin_Panel(User_Mobile);
			LCD_voidSendCommand(1);
			//------------------------------------------
			User_Mobile = UART_u8Rx();
				if (User_Mobile == 'C')
				{
						break; // break while
				}
		} 
		
		if (Input[0] == user1[0]  && Input[1] == user1[1]  && Input[2] == user1[2]  && Input[3] == user1[3]) { // user 1
			Number_of_Tries = 0 ;
			//the same as main menu in user panel..........
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, User1");
			_delay_ms(500);
			LCD_voidSendCommand(1);
			LCD_voidGotoxy(0,0);
			LCD_voidSendString((u8*)"D-Door"); // Displays door Options Close/Open
			LCD_voidGotoxy(7,0);
			LCD_voidSendString((u8*)"L-Lamps"); // Displays Lamps Options ON/OFF
			LCD_voidGotoxy(0, 1);
			LCD_voidSendString((u8*)"T-Temp"); // Displays Temperature / AC power
			LCD_voidGotoxy(7,1);
			LCD_voidSendString((u8*)"Q-Dimmer"); // Displays Dimmer control ON/OFF
			
			u8 User_Mobile = UART_u8Rx();
			User_Panel(User_Mobile);
			//-------------------------------------------------
			while('C' != UART_u8Rx() ); // busy wait tell but 'C'
			LCD_voidSendCommand(1);
			break; // break while
		}
		if (Input[0] == user2[0]  && Input[1] == user2[1]  && Input[2] == user2[2]  && Input[3] == user2[3]) { // user 2
			Number_of_Tries = 0 ;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, User2");
			_delay_ms(500);
			LCD_voidSendCommand(1);
			LCD_voidGotoxy(0,0);
			LCD_voidSendString((u8*)"D-Door"); // Displays door Options Close/Open
			LCD_voidGotoxy(7,0);
			LCD_voidSendString((u8*)"L-Lamps"); // Displays Lamps Options ON/OFF
			LCD_voidGotoxy(0, 1);
			LCD_voidSendString((u8*)"T-Temp"); // Displays Temperature / AC power
			LCD_voidGotoxy(7,1);
			LCD_voidSendString((u8*)"Q-Dimmer"); // Displays Dimmer control ON/OFF
			
			u8 User_Mobile = UART_u8Rx();
			User_Panel(User_Mobile);
			while( 'C' != UART_u8Rx() ); 
			LCD_voidSendCommand(1);
			break;
		}
		if (Input[0] == user3[0]  && Input[1] == user3[1]  && Input[2] == user3[2]  && Input[3] == user3[3]) { // user 3
			Number_of_Tries = 0 ;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, User3");
			_delay_ms(500);
			LCD_voidSendCommand(1);
			LCD_voidGotoxy(0,0);
			LCD_voidSendString((u8*)"D-Door"); // Displays door Options Close/Open
			LCD_voidGotoxy(7,0);
			LCD_voidSendString((u8*)"L-Lamps"); // Displays Lamps Options ON/OFF
			LCD_voidGotoxy(0, 1);
			LCD_voidSendString((u8*)"T-Temp"); // Displays Temperature / AC power
			LCD_voidGotoxy(7,1);
			LCD_voidSendString((u8*)"Q-Dimmer"); // Displays Dimmer control ON/OFF
			
			u8 User_Mobile = UART_u8Rx();
			User_Panel(User_Mobile);
			while( 'C' != UART_u8Rx()  );
			LCD_voidSendCommand(1);
			break;
		}
		if (Input[0] == user4[0]  && Input[1] == user4[1]  && Input[2] == user4[2]  && Input[3] == user4[3]) { // user 4
			Number_of_Tries = 0 ;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, User4");
			_delay_ms(500);
			LCD_voidSendCommand(1);
			LCD_voidGotoxy(0,0);
			LCD_voidSendString((u8*)"D-Door"); // Displays door Options Close/Open
			LCD_voidGotoxy(7,0);
			LCD_voidSendString((u8*)"L-Lamps"); // Displays Lamps Options ON/OFF
			LCD_voidGotoxy(0, 1);
			LCD_voidSendString((u8*)"T-Temp"); // Displays Temperature / AC power
			LCD_voidGotoxy(7,1);
			LCD_voidSendString((u8*)"Q-Dimmer"); // Displays Dimmer control ON/OFF
			
			u8 User_Mobile = UART_u8Rx();	
			User_Panel(User_Mobile);
			while( 'C' != UART_u8Rx() );
			break;
		}
		 if (Number_of_Tries == 0 && Login_State == 0) { // first try
			Number_of_Tries = 1;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"2 tries left");
			_delay_ms(300);
			break;
		} if (Number_of_Tries == 1) { // Second try
			Number_of_Tries = 2;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"1 try left");
			_delay_ms(300);
			break;
		} if (Number_of_Tries == 2) { // Third try -> thief
			System_Lock();
			break;
		}
	}
}

void User_input_Keypad(u8 * Input) {
	u8 Login_State = 0; // 0 = not logged  1 = Logged
	while (1) {
		/* Admin Checker */
		if (Input[0] == Diffualt[0]  && Input[1] == Diffualt[1]  && Input[2] == Diffualt[2]  && Input[3] == Diffualt[3]) {
			Login_State= 1;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello,Admin");
			_delay_ms(500);
			LCD_voidSendCommand(1);
			LCD_voidGotoxy(0,0);
			LCD_voidSendString((u8*)"D-Door");
			LCD_voidGotoxy(6,0);
			LCD_voidSendString((u8*)"L-Lamps"); // Add Lamp control
			LCD_voidGotoxy(0, 1);
			LCD_voidSendString((u8*)"T-Temp");
			LCD_voidGotoxy(6,1);
			LCD_voidSendString((u8*)"Q-Dimmer");
			u8 User_Mobile = Key();
			
			Admin_Panel(User_Mobile);
			LCD_voidSendCommand(1);
			if (User_Mobile == 'C')
			{
				break;
			}
		}
		/* User Checker */
		if (Input[0] == user1[0]  && Input[1] == user1[1]  && Input[2] == user1[2]  && Input[3] == user1[3]) { // user 1
			Number_of_Tries = 0 ;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, User1");
			User_Panel(Key());
			while('C' != Key() );
			break;
			}if (Input[0] == user2[0]  && Input[1] == user2[1]  && Input[2] == user2[2]  && Input[3] == user2[3]) { // user 2
			Number_of_Tries = 0 ;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, User2");
			User_Panel(Key());
			while('C' != Key() );
			break;
			}if (Input[0] == user3[0]  && Input[1] == user3[1]  && Input[2] == user3[2]  && Input[3] == user3[3]) { // user 3
			Number_of_Tries = 0 ;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, User3");
			User_Panel(Key());
			while('C' != Key() );
			break;
			}if (Input[0] == user4[0]  && Input[1] == user4[1]  && Input[2] == user4[2]  && Input[3] == user4[3]) { // user 4
			Number_of_Tries = 0 ;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"Hello, User4");
			User_Panel(Key());
			while('C' != Key() );
			break;
		}
		
		/////////////////////////////{password trials}//////////////////////////////////
		else if (Number_of_Tries == 0 && Login_State == 0) { // first try
			Number_of_Tries = 1;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"2 tries left");
			_delay_ms(300);
			break;
			} else if (Number_of_Tries == 1) { // Second try
			Number_of_Tries = 2;
			LCD_voidSendCommand(1);
			LCD_voidSendString((u8*)"1 try left");
			_delay_ms(300);
			break;
			} else if (Number_of_Tries == 2) { // Third try
			System_Lock(); // print thief in Arabic 'حرامي' on LCD.
			break;
		}
		//////////////////////////////////////////////////////////////////////////////

	}
}

void User_Mode(void){ // start user_mode function
	while (1){ // start while loop
		if (Key() == 'C'){ // start if condition
			LCD_voidSendCommand(1);
			LCD_voidGotoxy(0, 0);
			LCD_voidSendString((u8*)"Enter Password:");
			///////////////////////////get password from KEYPAD/////////////////////////////
			for (u8 i = 0; i < 4; i++){ // start for loop
				LCD_voidGotoxy(i, 1);
				user_input[i] = Key();
				_delay_ms(20);
				LCD_voidSendNumber(user_input[i]);
				_delay_ms(20);
				LCD_voidGotoxy(i, 1);
				LCD_voidSendData('*');
			} // end for loop
			//////////////////////////////////////////////////////////////////////////////
			break; // exit from while loop
		}// end if condition
	} // end while loop
} // end of the function