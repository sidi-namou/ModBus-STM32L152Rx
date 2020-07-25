/*
 * MBMESN_slavepdu.c
 *
 *  Created on: 7 avr. 2020
 *      Author: 33766
 */

#include "MBMESN_slavepdu.h"
MBMESN_errorstatus_t HandleMBFunction06(uint8_t * requestParameters, uint8_t * responseParameters, uint16_t * lengthOfResponsParameters)
{

	uint16_t RegiAdd,data;

	RegiAdd = (uint16_t)requestParameters[1];
	RegiAdd <<=8;
	RegiAdd |= (uint16_t )requestParameters[2];
	// si l'adresse du registre est autorisé alors
	if(RegiAdd  <  NB_OF_MBREGISTERS){
		// On écrit la donnée dans le regsitre
		data=(uint16_t )requestParameters[3];
		data<<=8;
		data|=(uint16_t )requestParameters[4];
		MB_Registers[RegiAdd]=data;



		// On construit la reponse qui doit-etre sur 5 octets
		responseParameters[0]= 6;
		responseParameters[1]=(uint8_t)(RegiAdd>>8);
		responseParameters[2]=(uint8_t)RegiAdd;
		responseParameters[3]=(uint8_t)(data>>8);
		responseParameters[4]=(uint8_t)data;
		*lengthOfResponsParameters=5;// 5octets
		return MBMESN_OK;
	}
	// si non
	else {
		 // il y'a une erreur d'adresse on envoie le code 0x83
		responseParameters[0]=0x86;
		responseParameters[1]=  2;				// L'adresse des données indiquée est invalide
		*lengthOfResponsParameters=2;
		return MBMESN_DATA_ADDRESS_ERROR;
	}





}
MBMESN_errorstatus_t HandleMBFunction03(uint8_t * requestParameters, uint8_t * responseParameters, uint16_t * pNbOfRegisters){

	uint16_t startAdd,numberOfRegisters,plageAdd;
	int j=0;
		startAdd = (uint16_t)requestParameters[1];
		startAdd <<=8;
		startAdd |= (uint16_t )requestParameters[2];
		numberOfRegisters=(uint16_t)requestParameters[3];
		numberOfRegisters<<=8;
		numberOfRegisters|= (uint16_t )requestParameters[4];
		plageAdd = startAdd + numberOfRegisters;// ça définie la plage des adresses
		// si la plage des adresses indiqués est valide
		if(plageAdd  <=  NB_OF_MBREGISTERS){
			responseParameters[0]= 3;
			responseParameters[1]= 2*numberOfRegisters;

			for(int i=0;i<numberOfRegisters;i++){

					responseParameters[j+2]=(uint8_t)(MB_Registers[startAdd+i]>>8);
					j++;
					responseParameters[j+2]=(uint8_t)MB_Registers[startAdd+i];
					j++;

			}
			j=0;// Pour parcouru à nouveau toutes les octets dans les registres d'ecriture

			*pNbOfRegisters=2*numberOfRegisters+2;
			return MBMESN_OK;
		}
		// si non
		else {
			 // la plage des adresses est invalid on envoie le code 0x83
			responseParameters[0]=0x83;
			responseParameters[1]=  2;				//la plage des adresse est invalide
			*pNbOfRegisters=2;
			return MBMESN_DATA_ADDRESS_ERROR;
		}


}

MBMESN_errorstatus_t HandleMBFunction16(uint8_t * requestParameters, uint8_t * responseParameters, uint16_t * lengthOfResponsParameters){


	uint16_t startAdd,numberOfRegisters,plageAdd,byteCount;
	int j=0;
		startAdd = (uint16_t)requestParameters[1];
		startAdd <<=8;
		startAdd |= (uint16_t )requestParameters[2];
		numberOfRegisters=(uint16_t)requestParameters[3];
		numberOfRegisters<<=8;
		numberOfRegisters|= (uint16_t )requestParameters[4];
		byteCount=requestParameters[5];
		plageAdd = startAdd + numberOfRegisters;// ça définie la plage des adresses
		// si la plage des adresses indiqués est valide
		if(plageAdd  <=  NB_OF_MBREGISTERS){


			for(int i=0;i<byteCount;i++){

				MB_Registers[j]=(uint16_t)requestParameters[i+6];// On commence par le 7ème octets
				i++;
				MB_Registers[j]<<=8;

				MB_Registers[j] |= (uint16_t)requestParameters[i+6];
				j++;

			}
			j=0;// Pour parcouru à nouveau toutes les octets dans les registres

						responseParameters[0]= 16;
						responseParameters[1]= (uint8_t)(startAdd>>8);
						responseParameters[2]=(uint8_t)startAdd;
						responseParameters[3]=(uint8_t)(numberOfRegisters>>8);
						responseParameters[4]=(uint8_t)numberOfRegisters;
						*lengthOfResponsParameters=5;
			return MBMESN_OK;
		}
		// si non
		else {
			 // la plage des adresses est invalid on envoie le code 0x90
			responseParameters[0]=0x90;
			responseParameters[1]=  2;				//la plage des adresse est invalide
			*lengthOfResponsParameters=2;
			return MBMESN_DATA_ADDRESS_ERROR;
		}





}
