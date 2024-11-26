/*
 * TIM1_interface.h
 *
 * Created: 07/09/2024 16:12:31
 *  Author: laila
 */ 


#ifndef TIM1_INTERFACE_H_
#define TIM1_INTERFACE_H_
/* Pre-scaler*/
#define  No_Prescaler    0x01
#define  _8_Prescaler    0x02
#define  _64_Prescaler   0x03
#define  _256_Prescaler  0x04
#define  _1024_Prescaler 0x05

#define   TIM1_PRESCALER _8_Prescaler

#define OV_MODE 0
#define CTC_MODE 1

#define  OCR_Val 65535

 /* Pins */
#define OC1A_PIN  0
#define OC1B_PIN   1

void TIM1_voidInitOVFMode(u8 copy_u8pin);

void TIM1_voidInitCTCMode(u8 copy_u8pin);

void TIM1_voidInit10BitfastPWM(u8 copy_u8pin , u8 copy_u8DC);

void TIM1_voidFastPWMICR1(void);

void TIM1_voidServoPWM(u16 copy_u16DC);

void TIM1_voidCallBack(void(*ptr)(void),u8 copy_u8Mode);



#endif /* TIM1_INTERFACE_H_ */