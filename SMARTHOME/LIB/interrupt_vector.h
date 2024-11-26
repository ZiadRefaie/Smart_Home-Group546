/*
 * interrupt_vector.h
 *
 * Created: 8/30/2024 4:24:45 PM
 *  Author: AMIT
 */ 


#ifndef INTERRUPT_VECTOR_H_
#define INTERRUPT_VECTOR_H_
 
#define  ISR(__vector_num)      void __vector_num(void)__attribute__((signal)) ;\
                                void __vector_num(void)


#define  INT_0       __vector_1
#define  INT_1       __vector_2
#define  INT_2       __vector_3

#define USART_Rx       __vector_13
#define TIMER0_OVF     __vector_11
#define TIMER0_COMP    __vector_10
#define TIMER1_OVF     __vector_9
#define TIMER1_COMPB   __vector_8
#define TIMER1_COMPA   __vector_7
#define TIMER2_OVF     __vector_5
#define TIMER2_COMP    __vector_4


#endif /* INTERRUPT_VECTOR_H_ */