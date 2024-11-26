/*
 * EXTEEPROM_prog.c
 *
 * Created: 28/09/2024 15:41:02
 *  Author: laila
 */ 

 #include "../../LIB/BIT_MATH.h"
 #include "../../LIB/STD_TYPE.h"

 #include "EXTEEPROM_interface.h"

 #include "../../MCAL/TWI/TWI_interface.h"

 #define F_CPU 16000000UL
 #include <util/delay.h>

 void EXTEEPROM_voidTxData(u16 copy_u16address , u8 copy_u8data){
	
	u8 Address;
	Address = 0b1010000 | copy_u16address>>8;
	/* Start Condition */
	TWI_TWI_statusStartCondition();
	/* Send Device address  */
	TWI_TWI_statusSendSlaveAdressWithWrite(Address);
		
	/* Send Slave address  */
	TWI_TWI_statusMasterSendData((u8)copy_u16address);

	/* Transmit Data to Slave */
	TWI_TWI_statusMasterSendData(copy_u8data);
	
	/* Stop condition */	
	TWI_voidStopCondition();

	_delay_ms(5);

 }

 u8 EXTEEPROM_u8RxData(u16 copy_u16address){
 
	 u8 Address,Data=0;

	 Address = 0b1010000 | (u8)copy_u16address>>8;
	
	/* Start Condition */
	TWI_TWI_statusStartCondition();
	
	/* Send Device address  */
	TWI_TWI_statusSendSlaveAdressWithWrite(Address);
	
	/* Send Slave address  */
	TWI_TWI_statusMasterSendData((u8)copy_u16address);

	/* Change the Write command to read */
	TWI_TWI_statusRepeatedStart();
	
	/* Sends a command to read the device address */
	TWI_TWI_statusSendSlaveAdressWithRead(Address);
	
	/* Stores first Byte */
	TWI_TWI_statusMasterReciveData(&Data);
	
	/* Stop condition */	
	TWI_voidStopCondition();
	
	return Data;

 }

void EXTEEPROM_voidTxPageWrite(u16 copy_u16address , u8* copy_u8data, u8 copy_u8Byte) {
	u8 Address = 0b1010000 | (copy_u16address >> 8);

	/* Start condition */
	TWI_TWI_statusStartCondition();

	/* Send EEPROM address with write command */
	TWI_TWI_statusSendSlaveAdressWithWrite(Address);

	/* Send the memory location (lower 8 bits of the address) */
	TWI_TWI_statusMasterSendData((u8)copy_u16address);

	/* Transmit the data bytes */
	for (u8 i = 0; i < copy_u8Byte; i++) {
		TWI_TWI_statusMasterSendData(copy_u8data[i]);
	}

	/* Stop condition */
	TWI_voidStopCondition();

	/* Poll for write completion */
	do {
		TWI_TWI_statusStartCondition();
	} while (TWI_TWI_statusSendSlaveAdressWithWrite(Address) != NOErr);

	TWI_voidStopCondition();

	/* Add a sufficient delay after write */
	_delay_ms(10);
}



void EXTEEPROM_u8RxPageRead(u16 copy_u16address, u8* copy_u8Data, u8 copy_u8Bytes) {
	u8 Address = 0b1010000 | (copy_u16address >> 8);

	/* Start Condition */
	TWI_TWI_statusStartCondition();

	/* Send Device address with a write request */
	TWI_TWI_statusSendSlaveAdressWithWrite(Address);

	/* Send memory address (lower byte) */
	TWI_TWI_statusMasterSendData((u8)copy_u16address);

	/* Repeated Start for read operation */
	TWI_TWI_statusRepeatedStart();

	/* Send Device address with a read request */
	TWI_TWI_statusSendSlaveAdressWithRead(Address);

	/* Read the bytes */
	for (u8 i = 0; i < copy_u8Bytes; i++) {
		if (i == (copy_u8Bytes - 1)) {
			/* For the last byte, receive with NACK */
			TWI_TWI_statusMasterReciveDataNACK(&copy_u8Data[i]);
			} else {
			/* Receive data with ACK */
			TWI_TWI_statusMasterReciveData(&copy_u8Data[i]);
		}
	}

	/* Stop condition */
	TWI_voidStopCondition();
}