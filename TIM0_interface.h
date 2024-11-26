/*
 * Time_interface.h
 *
 * Created: 06/09/2024 16:28:16
 *  Author: laila
 */ 

#ifndef TIME_INTERFACE_H_
#define TIME_INTERFACE_H_

/* Select Prescaler */

#define  No_Prescaler    0x01
#define  _8_Prescaler    0x02
#define  _64_Prescaler   0x03
#define  _256_Prescaler  0x04
#define  _1024_Prescaler 0x05

#define   TIM0_PRESCALER _64_Prescaler

#define  OV_MODE  0
#define  CTC_MODE 1
#define  FAST_PWM 2

#define  INVERTING 0
#define  NON_INVERTING 1

#define  OCR_VAL 250 

/*****************************************************************************
* Function Name: TIM0_voidOverFlowWithInterrupt
* Purpose      : set timer pins in overflow mode 
* Parameters   : non
* Return value : void
********************************************************************/
void TIM0_voidInit(u8 copy_u8Mode);

/*****************************************************************************
* Function Name: TIM0_voidCallBack
* Purpose      : executes the Interrupt 
* Parameters   : void ptr , u8 copy_u8pin
* Return value : void
********************************************************************/
void TIM0_voidCallBack(void(*ptr)(void),u8 copy_u8Mode);

void TIM0_voidFastPWM(u8 copy_DC,u8 copy_u8Mode);

#endif /* TIME_INTERFACE_H_ */