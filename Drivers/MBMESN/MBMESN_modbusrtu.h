#ifndef MBMESN_MODBUSRTU_H_INCLUDED
#define MBMESN_MODBUSRTU_H_INCLUDED

#include <stdint.h>

#include "MBMESN_errorstatus.h"

uint8_t ThisSlaveAddress;

MBMESN_errorstatus_t MBMESN_ModbusInit(void);
MBMESN_errorstatus_t MBMESN_ModbusClose(void);
MBMESN_errorstatus_t MBMESN_Modbus03ReadHoldingRegisters(uint8_t slaveAddress, uint16_t firstRegAddress, uint16_t numberOfRegisters, uint16_t * dataRead);
MBMESN_errorstatus_t MBMESN_Modbus06WriteSingleRegister(uint8_t slaveAddress, uint16_t regAddress, int16_t dataToWrite);
MBMESN_errorstatus_t MBMESN_Modbus16WriteMultipleRegisters(uint8_t slaveAddress, uint16_t firstRegAddress, int16_t* dataToWrite, uint16_t numberOfRegisters);

#endif // MBMESN_MODBUSRTU_H_INCLUDED
