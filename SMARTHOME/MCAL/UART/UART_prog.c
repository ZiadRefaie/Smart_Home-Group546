/*
 * UART_prog.c
 *
 * Created: 20/09/2024 15:29:04
 *  Author: laila
 */ 

#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPE.h"
#include "../../LIB/interrupt_vector.h"

#include "../DIO/DIO_interface.h"

#include "UART_interface.h"
#include "UART_reg.h"

volatile u8 data = 0;

void UART_voidInit() {
    u8 Op = 0;
    /* URSEL Register select to UCSRC */
    SET_BIT(Op, 7);

    /* Data Bits set to 8 */
    SET_BIT(Op, 1);
    SET_BIT(Op, 2);

    /* Set Stop Bit to 1-bit (CLR_BIT here for 1 stop bit) */
    CLR_BIT(Op, 3);

    /* No Parity */
    CLR_BIT(Op, 4);
    CLR_BIT(Op, 5);

    /* Set Asynchronous */
    CLR_BIT(Op, 6);

    /* Set UCSRC */
    UCSRC_REG = Op;

    /* Baud Rate to 9600 */
    UBRRL_REG = 103;

    /* Enable Transmit and Receive */
    SET_BIT(UCSRB_REG, 3);  // Enable TX
    SET_BIT(UCSRB_REG, 4);  // Enable RX

    /* Enable Interrupt Mode if defined */
    #if UART_Mode == Normal
        CLR_BIT(UCSRB_REG, 7); // Disable RX Complete Interrupt
    #elif UART_Mode == Interrupt
        SET_BIT(UCSRB_REG, 7); // Enable RX Complete Interrupt
    #endif
}

void UART_voidTx(u8 copy_u8Data) {
    UDR_REG = copy_u8Data;
    while (GET_BIT(UCSRA_REG, 5) == 0); // Wait until transmission complete
}

u8 UART_u8Rx(void) {
    #if UART_Mode == Normal
        while (GET_BIT(UCSRA_REG, 7) == 0); // Wait until data received
        return UDR_REG;
    #elif UART_Mode == Interrupt
        u8 received_data = data;  // Retrieve received data
        data = 0;                 // Clear data after reading
        return received_data;
    #endif
}

ISR(USART_Rx) {
    data = UDR_REG; // Store received data in global variable
}
