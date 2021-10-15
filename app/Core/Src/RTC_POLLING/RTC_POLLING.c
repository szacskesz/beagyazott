// Includes
#include "RTC_POLLING.h"
#include <stdbool.h>


// Global variables
RTC_HandleTypeDef HRTC;

RTC_TimeTypeDef currentTime = {0};
RTC_DateTypeDef currentDate = {0};

uint8_t nextDay;

// Private functions
void updateDateTime() {
	HAL_RTC_GetTime(&HRTC, &currentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&HRTC, &currentDate, RTC_FORMAT_BIN);
}


// Public functions
void RTC_POLLING_InitEveryDay(RTC_HandleTypeDef hrtc) {
	HRTC = hrtc;

	updateDateTime();
	nextDay = currentDate.WeekDay;
}

bool RTC_POLLING_CheckEveryDay() {
	updateDateTime();
	HAL_RTC_GetTime(&HRTC, &currentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&HRTC, &currentDate, RTC_FORMAT_BIN);
	if(currentDate.WeekDay == nextDay) {
		nextDay++;
		if(nextDay >= 7) nextDay = 0;

		return true;
	}

	return false;
}


