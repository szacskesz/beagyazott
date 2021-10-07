// Includes
#include "TM1638.h"

// Macros
#define ABS(x) ((x) > 0 ? (x) : -(x))

// Constants
#define TM1638_HEX_CMD_DATA 						0x40
#define TM1638_HEX_CMD_DISP 						0x80
#define TM1638_HEX_CMD_ADDR 						0xC0

#define TM1638_HEX_CMD_DATA_WRITE_MODE 				0x00
#define TM1638_HEX_CMD_DATA_READ_MODE 				0x02
#define TM1638_HEX_CMD_DATA_AUTO_ADDR_MODE 			0x00
#define TM1638_HEX_CMD_DATA_FIX_ADDR_MODE 			0x04
#define TM1638_HEX_CMD_DATA_NORMAL_MODE 			0x00
#define TM1638_HEX_CMD_DATA_TEST_MODE 				0x08

#define TM1638_HEX_CMD_DISP_ON 						0x08
#define TM1638_HEX_CMD_DISP_OFF 					0x00
#define TM1638_HEX_CMD_DISP_BRIGHTNESS_0  			0x00
#define TM1638_HEX_CMD_DISP_BRIGHTNESS_1  			0x01
#define TM1638_HEX_CMD_DISP_BRIGHTNESS_2  			0x02
#define TM1638_HEX_CMD_DISP_BRIGHTNESS_3  			0x03
#define TM1638_HEX_CMD_DISP_BRIGHTNESS_4  			0x04
#define TM1638_HEX_CMD_DISP_BRIGHTNESS_5  			0x05
#define TM1638_HEX_CMD_DISP_BRIGHTNESS_6  			0x06
#define TM1638_HEX_CMD_DISP_BRIGHTNESS_7  			0x07

#define TM1638_HEX_SEGMENT_TOP 				0x01
#define TM1638_HEX_SEGMENT_TOP_RIGHT 		0x02
#define TM1638_HEX_SEGMENT_BOTTOM_RIGHT 	0x04
#define TM1638_HEX_SEGMENT_BOTTOM 			0x08
#define TM1638_HEX_SEGMENT_BOTTOM_LEFT 		0x10
#define TM1638_HEX_SEGMENT_TOP_LEFT 		0x20
#define TM1638_HEX_SEGMENT_MIDDLE 			0x40
#define TM1638_HEX_SEGMENT_DECIMAL_POINT 	0x80

#define TM1638_HEX_LED_ON \
	0xFF
#define TM1638_HEX_LED_OFF \
	0x00
#define TM1638_HEX_DIGIT_0 \
	TM1638_HEX_SEGMENT_TOP \
	| TM1638_HEX_SEGMENT_TOP_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM \
	| TM1638_HEX_SEGMENT_BOTTOM_LEFT \
	| TM1638_HEX_SEGMENT_TOP_LEFT
#define TM1638_HEX_DIGIT_1 \
	TM1638_HEX_SEGMENT_TOP_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT
#define TM1638_HEX_DIGIT_2 \
	TM1638_HEX_SEGMENT_TOP \
	| TM1638_HEX_SEGMENT_TOP_RIGHT \
	| TM1638_HEX_SEGMENT_MIDDLE \
	| TM1638_HEX_SEGMENT_BOTTOM_LEFT \
	| TM1638_HEX_SEGMENT_BOTTOM
#define TM1638_HEX_DIGIT_3 \
	TM1638_HEX_SEGMENT_TOP \
	| TM1638_HEX_SEGMENT_TOP_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM \
	| TM1638_HEX_SEGMENT_MIDDLE
#define TM1638_HEX_DIGIT_4 \
	TM1638_HEX_SEGMENT_TOP_LEFT \
	| TM1638_HEX_SEGMENT_MIDDLE \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT \
	| TM1638_HEX_SEGMENT_TOP_RIGHT
#define TM1638_HEX_DIGIT_5 \
	TM1638_HEX_SEGMENT_TOP \
	| TM1638_HEX_SEGMENT_TOP_LEFT \
	| TM1638_HEX_SEGMENT_MIDDLE \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM
#define TM1638_HEX_DIGIT_6 \
	TM1638_HEX_SEGMENT_TOP \
	| TM1638_HEX_SEGMENT_TOP_LEFT \
	| TM1638_HEX_SEGMENT_MIDDLE \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM \
	| TM1638_HEX_SEGMENT_BOTTOM_LEFT
#define TM1638_HEX_DIGIT_7 \
	TM1638_HEX_SEGMENT_TOP \
	| TM1638_HEX_SEGMENT_TOP_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT
#define TM1638_HEX_DIGIT_8 \
	TM1638_HEX_SEGMENT_TOP \
	| TM1638_HEX_SEGMENT_TOP_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT \
	| TM1638_HEX_SEGMENT_BOTTOM \
	| TM1638_HEX_SEGMENT_BOTTOM_LEFT \
	| TM1638_HEX_SEGMENT_TOP_LEFT \
	| TM1638_HEX_SEGMENT_MIDDLE
#define TM1638_HEX_DIGIT_9 \
	TM1638_HEX_SEGMENT_BOTTOM \
	| TM1638_HEX_SEGMENT_BOTTOM_RIGHT \
	| TM1638_HEX_SEGMENT_TOP_RIGHT \
	| TM1638_HEX_SEGMENT_TOP \
	| TM1638_HEX_SEGMENT_TOP_LEFT \
	| TM1638_HEX_SEGMENT_MIDDLE
#define TM1638_HEX_SIGN_EMPTY \
	0x00
#define TM1638_HEX_SIGN_MINUS \
	TM1638_HEX_SEGMENT_MIDDLE

// Types
typedef enum
{
  DATA_ACCESS_WRITE = 0u,
  DATA_ACCESS_READ
} TM1638_DataAccessMode;

typedef enum
{
  DATA_ADDRESS_AUTO= 0u,
  DATA_ADDRESS_FIX
} TM1638_DataAddressMode;

typedef enum
{
  DATA_OPERATION_NORMAL= 0u,
  DATA_OPERATION_TEST
} TM1638_DataOperationMode;

// Global variables
GPIO_TypeDef* TM1638_STB_PORT;
uint16_t TM1638_STB_PIN;
GPIO_TypeDef* TM1638_CLK_PORT;
uint16_t TM1638_CLK_PIN;
GPIO_TypeDef* TM1638_DIO_PORT;
uint16_t TM1638_DIO_PIN;

TM1638_DisplayBrightness displayBrightness;
TM1638_DisplayState displayState;
TM1638_DataAccessMode dataAccessMode;
TM1638_DataAddressMode dataAddressMode;
TM1638_DataOperationMode dataOperationMode;

TM1638_LedStatus ledStatuses[8];
int numberToDisplay;

// Private function
void TM1638_STB_SET(GPIO_PinState state) {
	HAL_GPIO_WritePin(TM1638_STB_PORT, TM1638_STB_PIN, state);
}

void TM1638_STB_HIGH() {
	TM1638_STB_SET(GPIO_PIN_SET);
}

void TM1638_STB_LOW() {
	TM1638_STB_SET(GPIO_PIN_RESET);
}

void TM1638_CLK_SET(GPIO_PinState state) {
	HAL_GPIO_WritePin(TM1638_CLK_PORT, TM1638_CLK_PIN, state);
}

void TM1638_CLK_HIGH() {
	TM1638_CLK_SET(GPIO_PIN_SET);
}

void TM1638_CLK_LOW() {
	TM1638_CLK_SET(GPIO_PIN_RESET);
}

void TM1638_DIO_SET(GPIO_PinState state) {
	HAL_GPIO_WritePin(TM1638_DIO_PORT, TM1638_DIO_PIN, state);
}

void TM1638_DIO_HIGH() {
	TM1638_DIO_SET(GPIO_PIN_SET);
}

void TM1638_DIO_LOW() {
	TM1638_DIO_SET(GPIO_PIN_RESET);
}

void TM1638_SEND_BYTE(uint8_t byte) {
	// LSB_FIRST
	for (uint8_t i = 0; i < 8; i++)  {
		// send bit at index i
		TM1638_DIO_SET(!!(byte & (1 << i)));

		// send pulse
		TM1638_CLK_HIGH();
		TM1638_CLK_LOW();
	}
}

uint8_t TM1638_GetDisplayConfigCommand() {
	uint8_t byteToSend = TM1638_HEX_CMD_DISP;
	switch(displayState) {
		case DISPLAY_STATE_ON:
			byteToSend |= TM1638_HEX_CMD_DISP_ON;
			break;
		case DISPLAY_STATE_OFF:
			byteToSend |= TM1638_HEX_CMD_DISP_OFF;
			break;
	}

	switch(displayBrightness) {
		case DISPLAY_BRIGHTNESS_LEVEL_0:
			byteToSend |= TM1638_HEX_CMD_DISP_BRIGHTNESS_0;
			break;
		case DISPLAY_BRIGHTNESS_LEVEL_1:
			byteToSend |= TM1638_HEX_CMD_DISP_BRIGHTNESS_1;
			break;
		case DISPLAY_BRIGHTNESS_LEVEL_2:
			byteToSend |= TM1638_HEX_CMD_DISP_BRIGHTNESS_2;
			break;
		case DISPLAY_BRIGHTNESS_LEVEL_3:
			byteToSend |= TM1638_HEX_CMD_DISP_BRIGHTNESS_3;
			break;
		case DISPLAY_BRIGHTNESS_LEVEL_4:
			byteToSend |= TM1638_HEX_CMD_DISP_BRIGHTNESS_4;
			break;
		case DISPLAY_BRIGHTNESS_LEVEL_5:
			byteToSend |= TM1638_HEX_CMD_DISP_BRIGHTNESS_5;
			break;
		case DISPLAY_BRIGHTNESS_LEVEL_6:
			byteToSend |= TM1638_HEX_CMD_DISP_BRIGHTNESS_6;
			break;
		case DISPLAY_BRIGHTNESS_LEVEL_7:
			byteToSend |= TM1638_HEX_CMD_DISP_BRIGHTNESS_7;
			break;
	}

	return byteToSend;
}

uint8_t TM1638_GetDigitHexAt(uint8_t positionFromLeft) {
	if(positionFromLeft == 0 && numberToDisplay < 0) return TM1638_HEX_SIGN_MINUS;

	int absNumberToDisplay = ABS(numberToDisplay);
	for(int i = 7 - positionFromLeft; i > 0; --i) {
		absNumberToDisplay /= 10;
	}

	if(absNumberToDisplay > 0 || positionFromLeft == 7) {
		switch(absNumberToDisplay %= 10) {
			case 0: return TM1638_HEX_DIGIT_0;
			case 1: return TM1638_HEX_DIGIT_1;
			case 2: return TM1638_HEX_DIGIT_2;
			case 3: return TM1638_HEX_DIGIT_3;
			case 4: return TM1638_HEX_DIGIT_4;
			case 5: return TM1638_HEX_DIGIT_5;
			case 6: return TM1638_HEX_DIGIT_6;
			case 7: return TM1638_HEX_DIGIT_7;
			case 8: return TM1638_HEX_DIGIT_8;
			case 9: return TM1638_HEX_DIGIT_9;
		}
	}

	return TM1638_HEX_SIGN_EMPTY;
}

uint8_t TM1638_GetLedHexAt(uint8_t positionFromLeft) {
	if(positionFromLeft > 7 )
		return TM1638_HEX_LED_OFF;
	else
		return ledStatuses[positionFromLeft] == LED_ON ? TM1638_HEX_LED_ON : TM1638_HEX_LED_OFF;
}

void TM1638_SendDisplayCommand() {
	TM1638_SEND_BYTE(TM1638_GetDisplayConfigCommand());

	for (uint8_t i = 0; i < 8; ++i) {
		TM1638_SEND_BYTE(TM1638_GetDigitHexAt(i));
		TM1638_SEND_BYTE(TM1638_GetLedHexAt(i));
	}
}

void TM1638_SendDataCommand() {
	uint8_t byteToSend = TM1638_HEX_CMD_DATA;
	switch(dataAccessMode) {
		case DATA_ACCESS_WRITE:
			byteToSend |= TM1638_HEX_CMD_DATA_WRITE_MODE;
			break;
		case DATA_ACCESS_READ:
			byteToSend |= TM1638_HEX_CMD_DATA_READ_MODE;
			break;
	}

	switch(dataAddressMode) {
		case DATA_ADDRESS_AUTO:
			byteToSend |= TM1638_HEX_CMD_DATA_AUTO_ADDR_MODE;
			break;
		case DATA_ADDRESS_FIX:
			byteToSend |= TM1638_HEX_CMD_DATA_FIX_ADDR_MODE;
			break;
	}

	switch(dataOperationMode) {
		case DATA_OPERATION_NORMAL:
			byteToSend |= TM1638_HEX_CMD_DATA_NORMAL_MODE;
			break;
		case DATA_OPERATION_TEST:
			byteToSend |= TM1638_HEX_CMD_DATA_TEST_MODE;
			break;
	}

	TM1638_SEND_BYTE(byteToSend);
}

void TM1638_UpdateDisplay() {

	TM1638_DataAccessMode _dataAccessMode = dataAccessMode;
	TM1638_DataAddressMode _dataAddressMode = dataAddressMode;
	TM1638_DataOperationMode _dataOperationMode = dataOperationMode;

	dataAccessMode = DATA_ACCESS_WRITE;
	dataAddressMode = DATA_ADDRESS_AUTO;
	dataOperationMode = DATA_OPERATION_NORMAL;

	TM1638_STB_LOW();
	TM1638_SendDataCommand();
	TM1638_STB_HIGH();

	TM1638_STB_LOW();
	TM1638_SendDisplayCommand();
	TM1638_STB_HIGH();

	dataAccessMode = _dataAccessMode;
	dataAddressMode = _dataAddressMode;
	dataOperationMode = _dataOperationMode;
}

// Public functions
void TM1638_Init(
	GPIO_TypeDef* STB_PORT,
	uint16_t STB_PIN,
	GPIO_TypeDef* CLK_PORT,
	uint16_t CLK_PIN,
	GPIO_TypeDef* DIO_PORT,
	uint16_t DIO_PIN
) {
	TM1638_STB_PORT = STB_PORT;
	TM1638_STB_PIN = STB_PIN;
	TM1638_CLK_PORT = CLK_PORT;
	TM1638_CLK_PIN = CLK_PIN;
	TM1638_DIO_PORT = DIO_PORT;
	TM1638_DIO_PIN = DIO_PIN;

	displayBrightness = DISPLAY_BRIGHTNESS_LEVEL_0;
	displayState = DISPLAY_STATE_OFF;
	dataAccessMode = DATA_ACCESS_WRITE;
	dataAddressMode = DATA_ADDRESS_AUTO;
	dataOperationMode = DATA_OPERATION_NORMAL;

	numberToDisplay = 0;
	for (uint8_t i = 0; i < 8; ++i) {
		ledStatuses[i] = LED_OFF;
	}

// TODO
//	TM1638_CLK_OUTPUT();
//	TM1638_STB_OUTPUT();
//	TM1638_DIO_OUTPUT();

	TM1638_CLK_HIGH();
	TM1638_STB_HIGH();
	TM1638_DIO_HIGH();

	TM1638_SendDataCommand();
	TM1638_UpdateDisplay();
}

void TM1638_SetDisplayBrightness(TM1638_DisplayBrightness brightness) {
	displayBrightness = brightness;
	TM1638_UpdateDisplay();
}

void TM1638_SetDisplayState(TM1638_DisplayState state) {
	displayState = state;
	TM1638_UpdateDisplay();
}

void TM1638_SetNumberToDisplay(int number) {
	numberToDisplay = number;
	TM1638_UpdateDisplay();
}

void TM1638_SetLedStatus(uint8_t position, TM1638_LedStatus status) {
	if(position < 8) {
		ledStatuses[position] = status;
		TM1638_UpdateDisplay();
	}
}

void TM1638_ToggleLedStatus(uint8_t position) {
	if(position < 8)
		TM1638_SetLedStatus(position, ledStatuses[position] == LED_ON ? LED_OFF : LED_ON);
}
