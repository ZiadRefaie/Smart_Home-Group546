
#include "../../LIB/BIT_MATH.h" 
#include "../../LIB/STD_TYPE.h" 
#include "GI_reg.h" 




void GI_voidEnable(void) {
	
	SET_BIT(SREG_REG,7) ; 
	
//	__asm("SEI") ; 
	
} 

void GI_voidDisable(void){
	
	CLR_BIT(SREG_REG,7) ; 
	//__asm("CLI") ; 
}





