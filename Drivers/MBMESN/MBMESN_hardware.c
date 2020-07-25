#include "MBMESN_hardware.h"


volatile uint32_t state=0;
#define Empty 0
#define Receiver_Done 1
#define Transmitter_Done 2
#define Abort_Receive_Done 3


MBMESN_errorstatus_t MBMESN_serialCommInit(){


	return MBMESN_OK;


}

MBMESN_errorstatus_t MBMESN_serialCommClose(){

	return MBMESN_OK;
}

MBMESN_errorstatus_t MBMESN_serialCommWrite(uint8_t* dataToSend,uint32_t lengthOfData){
	HAL_StatusTypeDef ret=HAL_ERROR;
	state=Empty;

	if(lengthOfData>0){
		ret = HAL_UART_Transmit_IT(&huart2,dataToSend, lengthOfData);
		while(state!=Transmitter_Done);
	}

	if(ret==HAL_OK)return MBMESN_OK;
	else if(ret==HAL_ERROR)return MBMESN_ERROR;
	else return MBMESN_TIMEOUT;
}
MBMESN_errorstatus_t MBMESN_serialCommRead(uint8_t * data, uint16_t lengthOfData, uint32_t * totalBytesRead){


	HAL_StatusTypeDef ret=HAL_ERROR;
	uint32_t compteur,lCount=0 , nbByte=1,TByte=0;

	*totalBytesRead=0;

	 state=Empty;
	// On doit attendre le début de le transmission


	 HAL_UART_Receive_IT(&huart2,data,1);
	 while(state!=Receiver_Done){
		 __NOP();
	 }
	 TByte=1;
	    ////On lit les données caractère par caractère
while(nbByte<lengthOfData){



	 state=Empty;
		  	  HAL_UART_Receive_IT(&huart2,data+nbByte,1);// octet par octet
		  	compteur = HAL_GetTick();
		  	lCount = compteur;
		  	  while((compteur - lCount < 3)&& (state!=Receiver_Done)){	// Pour 2ms
		  		  compteur = HAL_GetTick();
		  	  }

		  	  if(compteur - lCount > 2){
					nbByte=lengthOfData;
					HAL_UART_AbortReceive_IT(&huart2);
						  		while(state!=Abort_Receive_Done);
						  		ret = HAL_TIMEOUT;
		}

		  		  TByte++;
		  		nbByte++;
	if(state!=Receiver_Done) TByte--;

	}
	nbByte=1;
	*totalBytesRead=TByte;


		if(ret==HAL_OK)return MBMESN_OK;
		else if(ret == HAL_ERROR)return MBMESN_ERROR;
		else return MBMESN_TIMEOUT;

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	state=Transmitter_Done;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
state=Receiver_Done;
}
HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart){
	state=Abort_Receive_Done;
}
