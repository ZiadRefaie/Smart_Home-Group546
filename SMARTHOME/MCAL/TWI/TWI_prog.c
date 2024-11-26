/*
 * TWI_prog.c
 *
 * Created: 9/28/2024 1:12:37 PM
 *  Author: AMIT
 */ 
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPE.h"
#include "TWI_interface.h"
#include "TWI_reg.h"




void TWI_voidMasterInit(u8 copy_u8address){
	
	/* SET TWBR REG  TO  init SCL LINE : 400KHZ  , FS =16MHZ , no prescaler */
	
	TWBR_REG=12 ;
	/* SET prescaler : 1 */
	
	CLR_BIT(TWSR_REG,0) ;
	CLR_BIT(TWSR_REG,1) ;
	
	if (copy_u8address != 0 )
	{
		TWAR_REG=copy_u8address<<1 ;
	}
	else {
		
		// do nothing
	}
	
	/* Enable ACK */
	SET_BIT(TWCR_REG,6) ;
	
	/* ENABLE TWI */
	SET_BIT(TWCR_REG,2) ;

	
}
void TWI_voidSlaveInit(u8 copy_u8address){
	
	/*SET Slave address*/
	TWAR_REG=copy_u8address<<1 ;
	
	/* Enable ACK */
	SET_BIT(TWCR_REG,6) ;
	
	/* ENABLE TWI */
	SET_BIT(TWCR_REG,2) ;
}

TWI_status  TWI_TWI_statusStartCondition(void){
	
	TWI_status LOC_TWI_statusVal=NOErr ;
	/* send start condition */
	SET_BIT(TWCR_REG,5) ;
	
	/* CLR flag To send start condition */
	SET_BIT(TWCR_REG,7) ;
	
	/* WAIT TWI has finished  current job*/
	while(GET_BIT(TWCR_REG,7)==0) ;
	
	if ((TWSR_REG & 0xF8)  != START_ACK)
	{
		LOC_TWI_statusVal=StartConditionErr ;
	}
	else{
		// do noting
	}
	
	return LOC_TWI_statusVal ;
	
}

TWI_status TWI_TWI_statusRepeatedStart(void) {
	
	TWI_status LOC_TWI_statusVal=NOErr ;
	/* send start condition */
	SET_BIT(TWCR_REG,5) ;
	
	/* CLR flag To send start condition */
	SET_BIT(TWCR_REG,7) ;
	
	/* WAIT TWI has finished  current job*/
	while(GET_BIT(TWCR_REG,7)==0) ;
	
	if ((TWSR_REG & 0xF8) != REP_START)
	{
		LOC_TWI_statusVal=RepeatedStartErr ;
	}
	else{
		// do noting
	}
	
	return LOC_TWI_statusVal ;
	
}

TWI_status TWI_TWI_statusSendSlaveAdressWithWrite(u8 copy_u8address){
	
	TWI_status LOC_TWI_statusVal=NOErr ;
	
	/* send salve to SDA LINE */
	
	TWDR_REG=copy_u8address<<1;
	/* CLR BIT0  TO Write */
	CLR_BIT(TWDR_REG,0) ;
	/* clr start condition */
	CLR_BIT(TWCR_REG,5) ;
	/* CLR flag To send  save address  */
	SET_BIT(TWCR_REG,7) ;
	
	/* WAIT TWI has finished  current job*/
	while(GET_BIT(TWCR_REG,7)==0) ;
	
	
	
	
	if ((TWSR_REG & 0xF8)  != SLAVE_ADD_AND_WR_ACK)
	{
		LOC_TWI_statusVal=SendSlaveAdressWithWriteErr ;
	}
	else{
		// do noting
	}
	
	return LOC_TWI_statusVal ;
	
}
TWI_status TWI_TWI_statusSendSlaveAdressWithRead(u8 copy_u8address){
	
	TWI_status LOC_TWI_statusVal=NOErr ;
	
	/* send salve to SDA LINE */
	
	TWDR_REG=copy_u8address<<1;
	/* CLR BIT0  TO Read */
	SET_BIT(TWDR_REG,0) ;
	/* clr start condition */
	CLR_BIT(TWCR_REG,5) ;
	/* CLR flag To send  save address  */
	SET_BIT(TWCR_REG,7) ;
	
	/* WAIT TWI has finished  current job*/
	while(GET_BIT(TWCR_REG,7)==0) ;

	if ((TWSR_REG & 0xF8)  != SLAVE_ADD_AND_RR_ACK)
	{
		LOC_TWI_statusVal=SendSlaveAdressWithReadErr ;
	}
	else{
		// do noting
	}
	
	return LOC_TWI_statusVal ;
	
}

TWI_status TWI_TWI_statusMasterSendData(u8 copy_u8data) {
	
	TWI_status LOC_TWI_statusVal=NOErr ;
	
	/* load data to bus */
	
	TWDR_REG=copy_u8data ;
	
	/* CLR flag To send  save address  */
	SET_BIT(TWCR_REG,7) ;
	
	/* WAIT TWI has finished  current job*/
	while(GET_BIT(TWCR_REG,7)==0);
	
	
	
	if ((TWSR_REG & 0xF8)  != MASTER_WR_BYTE_ACK)
	{
		LOC_TWI_statusVal=MasterSendDataErr ;
	}
	else{
		// do noting
	}
	
	return LOC_TWI_statusVal ;
	
	
}

TWI_status TWI_TWI_statusMasterReciveData(u8*copy_u8data){
	
	
	TWI_status LOC_TWI_statusVal=NOErr ;
	/* CLR flag To send  save address  */
	SET_BIT(TWCR_REG,7) ;
	
	/* WAIT TWI has finished  current job*/
	while(GET_BIT(TWCR_REG,7)==0);

	
	if ((TWSR_REG & 0xF8) != MASTER_RD_BYTE_WITH_ACK)
	{
		LOC_TWI_statusVal=MasterReciveDataErr ;
	}
	else{
		*copy_u8data=TWDR_REG ;
	}
	
	return LOC_TWI_statusVal ;
	
}

TWI_status TWI_TWI_statusMasterReciveDataNACK(u8 *copy_u8data) {
	TWI_status LOC_TWI_statusVal = NOErr;

	/* Clear ACK bit to indicate the end of transmission (send NACK after the next byte) */
	CLR_BIT(TWCR_REG, 6);  

	/* Clear flag to start receiving */
	SET_BIT(TWCR_REG, 7); 

	/* Wait until TWI finishes the current job */
	while (GET_BIT(TWCR_REG, 7) == 0);

	/* Check if the status register indicates the last byte received with NACK */
	if ((TWSR_REG & 0xF8) != MASTER_RD_BYTE_WITH_NACK) {
		LOC_TWI_statusVal = MasterReciveDataErr;
		} else {
		/* Read the received data */
		*copy_u8data = TWDR_REG;
	}
	return LOC_TWI_statusVal;
}

void TWI_voidStopCondition(void){
	
	
	/* send stop condition */
	
	SET_BIT(TWCR_REG,4) ;
	/* CLR flag To send  save address  */
	SET_BIT(TWCR_REG,7) ;
}