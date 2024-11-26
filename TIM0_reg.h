/*
 * Timer_reg.h
 *
 * Created: 06/09/2024 16:34:34
 *  Author: laila
 */ 


#ifndef TIMER_REG_H_
#define TIMER_REG_H_

#define TCCR0_REG *((volatile u8*)0x53) 
#define TCNT0_REG *((volatile u8*)0x52)
#define OCR0_REG  *((volatile u8*)0x5C)
#define TIMSK_REG *((volatile u8*)0x59)
#define TIFR_REG  *((volatile u8*)0x58)


#endif /* TIMER_REG_H_ */