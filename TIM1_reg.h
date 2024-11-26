/*
 * TIM1_reg.h
 *
 * Created: 07/09/2024 16:12:43
 *  Author: laila
 */ 


#ifndef TIM1_REG_H_
#define TIM1_REG_H_

// A
#define TCCR1A_REG  *((volatile u8*)0x4F)
#define OCR1AL_REG  *((volatile u16*)0x4A)

// B
#define TCCR1B_REG  *((volatile u8*)0x4E)
#define OCR1BL_REG  *((volatile u16*)0x48)

#define TCNT1L_REG  *((volatile u16*)0x4C)
#define ICR1L_REG   *((volatile u16*)0x46)
#define TIMSK_REG   *((volatile u8*)0x59)
#define TIFR_REG    *((volatile u8*)0x58)

#endif /* TIM1_REG_H_ */
