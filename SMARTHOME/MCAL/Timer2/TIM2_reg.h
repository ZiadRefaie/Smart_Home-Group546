﻿/*
 * TIM2_reg.h
 *
 * Created: 08/09/2024 11:36:25 ص
 *  Author: zidrar
 */ 


#ifndef TIM2_REG_H_
#define TIM2_REG_H_


#define TCCR2_REG   *((volatile u8*)0x45)
#define TCNT2_REG   *((volatile u8*)0x44)
#define OCR2_REG    *((volatile u8*)0x43)
#define ASSR_REG    *((volatile u8*)0x42)
#define TIMSK_REG   *((volatile u8*)0x59)
#define TIFR_REG    *((volatile u8*)0x58)




#endif /* TIM2_REG_H_ */