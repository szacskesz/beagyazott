#ifndef _SIM900_H_
#define _SIM900_H_

// Includes
#include "main.h"

//return is the error code, 0 means no error
uint8_t SIM900_Init(UART_HandleTypeDef HUART, uint32_t timeout);

uint8_t SIM900_TrySendSMS(char *msg);

#endif // _SIM900_H_
