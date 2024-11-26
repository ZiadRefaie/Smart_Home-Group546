/*
 * EXTEEPROM_interface.h
 *
 * Created: 28/09/2024 15:41:23
 *  Author: laila
 */ 


#ifndef EXTEEPROM_INTERFACE_H_
#define EXTEEPROM_INTERFACE_H_


void EXTEEPROM_voidTxData(u16 copy_u16address , u8 copy_u8data);

void EXTEEPROM_voidTxPageWrite(u16 copy_u16address , u8* copy_u16data, u8 copy_u8Byte);

u8 EXTEEPROM_u8RxData(u16 copy_u16address);

void EXTEEPROM_u8RxPageRead(u16 copy_u16address, u8* receivedData, u8 copy_u8Bytes) ;





#endif /* EXTEEPROM_INTERFACE_H_ */