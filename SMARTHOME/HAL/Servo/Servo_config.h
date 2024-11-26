/*
 * Servo_config.h
 *
 * Created: 13/09/2024 11:27:59 م
 *  Author: zidrar
 */ 


#ifndef SERVO_CONFIG_H_
#define SERVO_CONFIG_H_

#define TIM0   0
#define TIM1_A 1
#define TIM1_B 2
#define TIM2   3


#define TIMER0_PORT DIO_PORTB
#define TIMER0 DIO_PIN3

#define TIMER1_PORT DIO_PORTD
#define TIMER1_B DIO_PIN4
#define TIMER1_A DIO_PIN5


#define TIMER2_PORT DIO_PORTD
#define TIMER2 DIO_PIN7

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

#endif /* SERVO_CONFIG_H_ */