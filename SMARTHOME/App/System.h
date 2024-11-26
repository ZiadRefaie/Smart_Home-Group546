/*
 * System.h
 *
 * Created: 26/11/2024 11:37:47 ص
 *  Author: zidrar
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_
extern u8 user_input[4];

void Init(void);

u8 Key(void);

void System_Lock(void);

u32 Temperature(void);

u8 Velocity(void);

void User_Panel(u8 Button);

void Admin_Panel(u8 Button);

void User_Input_Bluetooth(u8* Input);

void User_input_Keypad(u8 * Input);

void User_Mode(void);


#endif /* SYSTEM_H_ */