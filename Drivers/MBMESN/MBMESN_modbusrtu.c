#include "MBMESN_modbusrtu.h"


static void MBMESN_AjoutCRC(uint8_t * frame, uint16_t frameLengthWoCRC,uint16_t *pLengthOfResponseFrame,uint16_t crc){

	// On insère le crc à la fin du trame
			frame[frameLengthWoCRC] = (uint8_t )(crc);
			frame[frameLengthWoCRC+1] = (uint8_t )( crc >> 8);
			// On écrit la taille du trame dans la variable pLengthOfResponseFrame
			*pLengthOfResponseFrame = frameLengthWoCRC + 2;// on ajoute les 2 octets pour crc à la taille du frame
}
static uint8_t MBMESN_VeriCRC(uint16_t crcC, uint16_t crcR){
	if(crcC != crcR)return 0;
	else return 1;
}
static uint16_t MBMESN_computeCRC(uint8_t * frame, uint16_t frameLengthWoCRC){
uint16_t crc = 0xFFFF;		// Initialisation du crc16 modbus-rtu

  for (int pos = 0; pos < frameLengthWoCRC; pos++)
  {
    crc ^= (uint16_t)frame[pos];          // XOR entre crc et l'octet suivant

    for (int i = 8; i != 0; i--) {    // une boucle pour chaque bit
      if ((crc & 0x0001) != 0) {      // si LSB est égal à 1
        crc >>= 1;                    // decalage à droite et xor avec 0xA001
        crc ^= 0xA001;
      }
      else                            // si non
        crc >>= 1;                    // On fait seulement un decalage à droite
    }
  }
  // On retourne la valeur du crc
  return crc;
}



/* This function receives a Modbus RTU frame, checks its addresse and integrity and extract the PDU (function code and parameters) */
MBMESN_errorstatus_t HandleMBRequest(uint8_t * requestFrame, uint16_t lengtOfRequest, uint8_t * modbusPDU, uint16_t * pLengthOfPDU){

	uint16_t crcC,crcR;
	uint8_t slaveAdress;

	// On calcul le crc du trame sans les octets du crc
	crcC = MBMESN_computeCRC(requestFrame, lengtOfRequest -2);
	// On recupere les octets du CRC du trame
	crcR = (uint16_t)requestFrame[lengtOfRequest -1];
	crcR <<=8;
	crcR |= (uint16_t )requestFrame[lengtOfRequest -2];
	//On verifie si on a bien reçu les données




	if(MBMESN_VeriCRC(crcR,crcC)==1){
		// On recupere l'adresse dans le trame
		slaveAdress = requestFrame[0];
		// S'il parle pas avec moi je rejete le trame
		// On vérifie si l'adresse est bien notre adresse
		if(slaveAdress!=ThisSlaveAddress)return MBMESN_SLAVE_DEVICE_ERROR;
		// si non
		// On copie le code PDU dans modbusPDU
		 if(requestFrame[1]!=3 && requestFrame[1]!=6 && requestFrame[1]!=16){

				 // Si la fonction n'est pas autorisé on envoie le code 0x80+numero de la fonction
				 modbusPDU[0]=128 +requestFrame[1];
							 modbusPDU[1]=  1;				// La fonction de la requete n'est pas supportée par cette esclave
							 *pLengthOfPDU=2;



		 }
		 else{
			 // Si c'est la fonction 06
			 if(requestFrame[1]==6){
				 // On appelle la fonction 06
				 HandleMBFunction06(requestFrame+1,modbusPDU,pLengthOfPDU);
		 }
			 else if(requestFrame[1]==3){
				 // si c'est la fonction 03
				 HandleMBFunction03(requestFrame+1,modbusPDU,pLengthOfPDU);
			 }
			 else{
				 HandleMBFunction16(requestFrame+1,modbusPDU,pLengthOfPDU);
			 }


		 }
		// on retourne un MBMESN_OK
		return MBMESN_OK;
	}
	// si non on retourne un CRC_ERROR
	else return MBMESN_CRC_ERROR;


}
/* This function receives a Modbus PDU and builds a response frame by adding slave address and the CRC */
MBMESN_errorstatus_t BuildMBResponse(uint8_t * modbusPDU, uint16_t lengthOfPDU, uint8_t *responseFrame, uint16_t *pLengthOfResponseFrame){

		uint16_t crc;


		// On insère au début du trame l'adresse de l'esclave qui est fixée à 01
		responseFrame[0]=ThisSlaveAddress;
		// puis On insère les données
		for(int i=0;i<lengthOfPDU;i++)responseFrame[i+1]=modbusPDU[i];
		// On calcule le crc du trame
				crc = MBMESN_computeCRC(responseFrame,lengthOfPDU+1);
				// On ajoute le crc à la fin du trame
				// on ajoute 1 parceque on a ajouté l'octet de l'adresse de l'esclave
				 MBMESN_AjoutCRC(responseFrame, lengthOfPDU+1,pLengthOfResponseFrame, crc);

		return MBMESN_OK;
}
