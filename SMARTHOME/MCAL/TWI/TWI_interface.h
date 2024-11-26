/*
 * TWI_interface.h
 *
 * Created: 9/28/2024 1:12:20 PM
 *  Author: AMIT
 */ 


#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_


typedef enum{
	     
		 NOErr , 
		 StartConditionErr , 
		 RepeatedStartErr , 
		 SendSlaveAdressWithWriteErr , 
		 SendSlaveAdressWithReadErr , 
		 MasterSendDataErr , 
		 MasterReciveDataErr , 
		 
	   
	
	}TWI_status;

// Address : 0 
void TWI_voidMasterInit(u8 copy_u8address) ; 
void TWI_voidSlaveInit(u8 copy_u8address) ;

TWI_status  TWI_TWI_statusStartCondition(void); 

TWI_status TWI_TWI_statusRepeatedStart(void) ; 

TWI_status TWI_TWI_statusSendSlaveAdressWithWrite(u8 copy_u8address) ; 

TWI_status TWI_TWI_statusSendSlaveAdressWithRead(u8 copy_u8address) ;

TWI_status TWI_TWI_statusMasterSendData(u8 copy_u8data) ; 

TWI_status TWI_TWI_statusMasterReciveData(u8*copy_u8data) ;

TWI_status TWI_TWI_statusMasterReciveDataNACK(u8*copy_u8data);

void TWI_voidStopCondition(void) ; 











#endif /* TWI_INTERFACE_H_ */