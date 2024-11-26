/*
 * TIM2_interface.h
 *
 * Created: 08/09/2024 11:36:15 ص
 *  Author: zidrar
 */ 


#ifndef TIM2_INTERFACE_H_
#define TIM2_INTERFACE_H_

/* Select Prescaler */

#define  No_Prescaler    0x01
#define  _8_Prescaler    0x02
#define  _64_Prescaler   0x03
#define  _256_Prescaler  0x04
#define  _1024_Prescaler 0x05

#define   TIM2_PRESCALER _8_Prescaler

#define  OV_MODE	0
#define  CTC_MODE	1
#define  FAST_PWM	2
#define  PHASE_PWM  3

#define  INVERTING     0
#define  NON_INVERTING 1

#define  PWM_MODE INVERTING

#define  OCR2_VAL 250 

void TIM2_voidInit(u8 copy_u8Mode);

void TIM2_voidPWM(u8 copy_u8Mode,u8 copy_u8DC);

void TIM2_voidCallBack(void(*ptr)(void),u8 copy_u8Mode);

#endif /* TIM2_INTERFACE_H_ */