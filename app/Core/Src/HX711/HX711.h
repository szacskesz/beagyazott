#ifndef _HX711_H_
#define _HX711_H_

// Includes
#include "main.h"

// Types
typedef enum
{
  GAIN_128 = 0u,
  GAIN_64,
  GAIN_32
} HX711_GainLevel;

typedef enum {
	FALSE,
	TRUE
} boolean;

void HX711_Init(
	GPIO_TypeDef* SCK_PORT,
	uint16_t SCK_PIN,
	GPIO_TypeDef* DT_PORT,
	uint16_t DT_PIN
);

void HX711_PowerDown();

void HX711_PowerUp();

boolean HX711_IsPoweredUp();

void HX711_SetGainLevel(HX711_GainLevel _gainLevel);

void HX711_Calibrate(double _shift, double _coeff);

double HX711_ReadValue();

double HX711_ReadAverageValue(int times);

#endif // _HX711_H_
