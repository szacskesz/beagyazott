// Includes
#include "HX711.h"

// Global variables
GPIO_TypeDef* HX711_SCK_PORT;
uint16_t HX711_SCK_PIN;
GPIO_TypeDef* HX711_DT_PORT;
uint16_t HX711_DT_PIN;

HX711_GainLevel gainLevel = GAIN_128;
double shift = 0;;
double coeff = 1;

// Private function
GPIO_PinState HX711_SCK_GET() {
	return HAL_GPIO_ReadPin(HX711_SCK_PORT, HX711_SCK_PIN);
}

void HX711_SCK_SET(GPIO_PinState state) {
	HAL_GPIO_WritePin(HX711_SCK_PORT, HX711_SCK_PIN, state);
}

void HX711_SCK_HIGH() {
	HX711_SCK_SET(GPIO_PIN_SET);
}

void HX711_SCK_LOW() {
	HX711_SCK_SET(GPIO_PIN_RESET);
}

GPIO_PinState HX711_DT_GET() {
	return HAL_GPIO_ReadPin(HX711_DT_PORT, HX711_DT_PIN);
}

void HX711_WaitUntilDataReady() {
	while(HX711_DT_GET() != GPIO_PIN_RESET);
}

void HX711_PerformGainPulse() {
	uint8_t times;
	switch(gainLevel) {
		case GAIN_128:
			times = 1;
			break;
		case GAIN_64:
			times = 3;
			break;
		case GAIN_32:
			times = 2;
			break;
	}

	while(times-- > 0) {
		HX711_SCK_HIGH();
		HX711_SCK_LOW();
	}
}

long HX711_ReadRawValue() {
	HX711_WaitUntilDataReady();

	long value = 0;
	for (unsigned short i = 0; i < 24; i++) {
		HX711_SCK_HIGH();
		value = value << 1;
		HX711_SCK_LOW();
		if(HX711_DT_GET() == GPIO_PIN_SET) ++value;
	}

	HX711_PerformGainPulse();

	//HX711 range: lower half is from 0x800000 to 0xFFFFFF, upper half is from 0x000000 to 7FFFFF
	//so it rolls over at 0xFFFFFF, so we need to flip the 24th bit
	return value ^ 0x800000;
}

// Public functions
void HX711_Init(
	GPIO_TypeDef* SCK_PORT,
	uint16_t SCK_PIN,
	GPIO_TypeDef* DT_PORT,
	uint16_t DT_PIN
) {
	HX711_SCK_PORT= SCK_PORT;
	HX711_SCK_PIN = SCK_PIN;
	HX711_DT_PORT = DT_PORT;
	HX711_DT_PIN = DT_PIN;

	HX711_PowerDown();
}

void HX711_PowerDown() {
	HX711_SCK_HIGH();
	HAL_Delay(1);
}

void HX711_PowerUp() {
	HX711_SCK_LOW();
	HAL_Delay(1);

	HX711_ReadRawValue(); // to set the gain level;
}

boolean HX711_IsPoweredUp() {
	return HX711_SCK_GET() == GPIO_PIN_RESET;
}

void HX711_SetGainLevel(HX711_GainLevel _gainLevel) {
	gainLevel = _gainLevel;

	if(HX711_IsPoweredUp()) HX711_ReadRawValue();
}

void HX711_Calibrate(double _shift, double _coeff) {
	shift = _shift;
	coeff = _coeff;
}

double HX711_ReadValue() {
	return ((double)HX711_ReadRawValue() - shift) * coeff;
}

double HX711_ReadAverageValue(int times) {
	double value = 0;

	for(int i = 0; i < times; ++i) {
		value += HX711_ReadValue() / (double)times;
	}

	return value;
}
