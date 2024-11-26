/*
 * UART_interface.h
 *
 * Created: 20/09/2024 15:29:36
 *  Author: laila
 */ 


#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

#define  Normal 0
#define  Interrupt 1
#define  UART_Mode Normal

void UART_voidInit();

void UART_voidTx(u8 copy_u8Data);

u8 UART_u8Rx(void);



#endif /* UART_INTERFACE_H_ */