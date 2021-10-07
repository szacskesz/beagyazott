#ifndef _TM1638_H_
#define _TM1638_H_

// Includes
#include "main.h"

// Types
typedef enum
{
  DISPLAY_BRIGHTNESS_LEVEL_0 = 0u,
  DISPLAY_BRIGHTNESS_LEVEL_1,
  DISPLAY_BRIGHTNESS_LEVEL_2,
  DISPLAY_BRIGHTNESS_LEVEL_3,
  DISPLAY_BRIGHTNESS_LEVEL_4,
  DISPLAY_BRIGHTNESS_LEVEL_5,
  DISPLAY_BRIGHTNESS_LEVEL_6,
  DISPLAY_BRIGHTNESS_LEVEL_7
} TM1638_DisplayBrightness;

typedef enum
{
  DISPLAY_STATE_OFF = 0u,
  DISPLAY_STATE_ON
} TM1638_DisplayState;

typedef enum
{
  LED_OFF = 0u,
  LED_ON
} TM1638_LedStatus;

void TM1638_Init(
	GPIO_TypeDef* STB_PORT,
	uint16_t STB_PIN,
	GPIO_TypeDef* CLK_PORT,
	uint16_t CLK_PIN,
	GPIO_TypeDef* DIO_PORT,
	uint16_t DIO_PIN
);
void TM1638_SetDisplayBrightness(TM1638_DisplayBrightness brightness);
void TM1638_SetDisplayState(TM1638_DisplayState state);
void TM1638_SetNumberToDisplay(int number);
void TM1638_SetLedStatus(uint8_t position, TM1638_LedStatus status);
void TM1638_ToggleLedStatus(uint8_t position);

#endif // _TM1638_H_
