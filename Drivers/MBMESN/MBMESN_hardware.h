#ifndef MBMESN_HARDWARE_INCLUDED
#define MBMESN_HARDWARE_INCLUDED

#include <stdint.h>
#include "stm32l1xx_hal.h"
#include "MBMESN_errorstatus.h"
#include "usart.h"
#include "gpio.h"
extern UART_HandleTypeDef huart2;

MBMESN_errorstatus_t MBMESN_serialCommInit();
MBMESN_errorstatus_t MBMESN_serialCommClose();
MBMESN_errorstatus_t MBMESN_serialCommWrite(uint8_t* dataToSend,uint32_t lengthOfData);
MBMESN_errorstatus_t MBMESN_serialCommRead(uint8_t * data, uint16_t lengthOfData, uint32_t * totalBytesRead);

#endif // MBMESN_HARDWARE_INCLUDED
