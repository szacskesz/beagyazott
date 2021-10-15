// Includes
#include <stdbool.h>
#include <string.h>
#include "SIM900.h"
#include "SIM900.config.h"


// Defines
#define BUFFER_SIZE 100
#define TIMEOUT_MS 5000 //TODO lower timeout value

#define LINE_ENDING 					"\r\n"

#define CMD_TEST 						"AT" LINE_ENDING
#define RESP_TEST_OK		 			CMD_TEST LINE_ENDING "OK" LINE_ENDING

#define CMD_CHECK_SIM_INSERTED			"AT+CSMINS?" LINE_ENDING
#define RESP_CHECK_SIM_INSERTED_OK		CMD_CHECK_SIM_INSERTED LINE_ENDING "+CSMINS: 0,1" LINE_ENDING LINE_ENDING "OK" LINE_ENDING

#define CMD_CHECK_NEED_PIN				"AT+CPIN?" LINE_ENDING
#define RESP_CHECK_NEED_PIN_OK			CMD_CHECK_NEED_PIN LINE_ENDING "+CPIN: READY" LINE_ENDING LINE_ENDING "OK" LINE_ENDING

#define CMD_UNLOCK_PIN(code)			"AT+CPIN=\"" #code "\"" LINE_ENDING

#define CMD_CHECK_ATTACHED				"AT+CGATT?" LINE_ENDING
#define RESP_CHECK_ATTACHED_OK			CMD_CHECK_ATTACHED LINE_ENDING "+CGATT: 1" LINE_ENDING LINE_ENDING "OK" LINE_ENDING

#define CMD_CHECK_REGISTERED			"AT+CREG?" LINE_ENDING
#define RESP_CHECK_REGISTERED_OK_1		CMD_CHECK_REGISTERED LINE_ENDING "+CREG: " "1"
#define RESP_CHECK_REGISTERED_OK_2		CMD_CHECK_REGISTERED LINE_ENDING "+CREG: " "5"
#define RESP_CHECK_REGISTERED_OK_3		CMD_CHECK_REGISTERED LINE_ENDING "+CREG: " "6"
#define RESP_CHECK_REGISTERED_OK_4		CMD_CHECK_REGISTERED LINE_ENDING "+CREG: " "7"

#define CMD_SET_TEXT_MODE				"AT+CMGF=1" LINE_ENDING
#define RESP_SET_TEXT_MODE_OK			CMD_SET_TEXT_MODE LINE_ENDING "OK" LINE_ENDING

#define CMD_MSG1_P1						"AT+CMGS=\""
#define CMD_MSG1_P3						"\"" LINE_ENDING
#define RESP_MSG1_OK_P2					LINE_ENDING ">"

#define CMD_MSG2_P2						"\x1A" LINE_ENDING
#define RESP_MSG2_OK_CONTAINS			"OK"



// Global variables
UART_HandleTypeDef SIM900_HUART;
char buffer[BUFFER_SIZE] = "";


// Private functions
bool isStrStartsWith(const char *str, const char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

bool isStrContains(const char *str, const char * sub) {
	return strstr(str, sub) != NULL;
}

void resetBuffer() {
	memset(buffer, 0, BUFFER_SIZE);
}

void uartSend(const char *cmd) {
	HAL_UART_Transmit(&SIM900_HUART, (uint8_t *)cmd, strlen(cmd), TIMEOUT_MS);
}

void uartReceive() {
	HAL_UART_Receive(&SIM900_HUART, (uint8_t *)buffer, BUFFER_SIZE, TIMEOUT_MS);
}

void uartSendAndReceive(const char *cmd) {
	resetBuffer();

	uartSend(cmd);
	uartReceive();
}

bool isSerialConnectionWorking() {
	uartSendAndReceive(CMD_TEST);

	return isStrStartsWith(buffer, RESP_TEST_OK);
}

bool isSimInserted() {
	uartSendAndReceive(CMD_CHECK_SIM_INSERTED);

	return isStrStartsWith(buffer, RESP_CHECK_SIM_INSERTED_OK);
}

bool isSimLocked() {
	uartSendAndReceive(CMD_CHECK_NEED_PIN);

	return !isStrStartsWith(buffer, RESP_CHECK_NEED_PIN_OK);
}

void tryUnlockSim() {
//	uartSendAndReceive(CMD_UNLOCK_PIN(PIN_CODE)); //TODO uncomment
}

bool isAttachedToNetwork() {
	uartSendAndReceive(CMD_CHECK_ATTACHED);

	return isStrStartsWith(buffer, RESP_CHECK_ATTACHED_OK);
}

bool isRegisteredToNetwork() {
	uartSendAndReceive(CMD_CHECK_REGISTERED);

	return isStrStartsWith(buffer, RESP_CHECK_REGISTERED_OK_1)
		|| isStrStartsWith(buffer, RESP_CHECK_REGISTERED_OK_2)
		|| isStrStartsWith(buffer, RESP_CHECK_REGISTERED_OK_3)
		|| isStrStartsWith(buffer, RESP_CHECK_REGISTERED_OK_4);
}

bool setTextMode() {
	uartSendAndReceive(CMD_SET_TEXT_MODE);

	return isStrStartsWith(buffer, RESP_SET_TEXT_MODE_OK);
}


bool sendSMS(char *phoneNumber, char *msg) {
	uint32_t msg1_size = strlen(CMD_MSG1_P1) + strlen(phoneNumber) + strlen(CMD_MSG1_P3) + 1;
	char msg1[msg1_size];
	memset(msg1, 0, msg1_size*sizeof(char));
	strcat(msg1, CMD_MSG1_P1);
	strcat(msg1, phoneNumber);
	strcat(msg1, CMD_MSG1_P3);

	uartSendAndReceive(msg1);

	uint32_t msg1_resp_size = strlen(msg1) + strlen(RESP_MSG1_OK_P2) + 1;
	char msg1_resp[msg1_resp_size];
	memset(msg1_resp, 0, msg1_resp_size*sizeof(char));
	strcat(msg1_resp, msg1);
	strcat(msg1_resp, RESP_MSG1_OK_P2);

	if(!isStrStartsWith(buffer, msg1_resp)) return false;

	uint32_t msg2_size = strlen(msg) + strlen(CMD_MSG2_P2) + 1;
	char msg2[msg2_size];
	memset(msg2, 0, msg2_size*sizeof(char));
	strcat(msg2, msg);
	strcat(msg2, CMD_MSG2_P2);

	uartSendAndReceive(msg2);

	return isStrContains(buffer, RESP_MSG2_OK_CONTAINS);
}


// Public functions
uint8_t SIM900_Init(UART_HandleTypeDef HUART, uint32_t timeout) {
	SIM900_HUART = HUART;

	uint32_t timeoutTickTime = HAL_GetTick() + timeout;

	while(!isSerialConnectionWorking()) {
		if(HAL_GetTick() > timeoutTickTime) return 1;
	}

	if(!isSimInserted()) return 2;

	if(isSimLocked()) tryUnlockSim();
	if(isSimLocked()) return 3;

	while(!isAttachedToNetwork()) {
		if(HAL_GetTick() > timeoutTickTime) return 4;
	}

	while(!isRegisteredToNetwork()) {
		if(HAL_GetTick() > timeoutTickTime) return 5;
	}

	return 0;
}

uint8_t SIM900_TrySendSMS(char *msg) {
	if(!setTextMode()) return 1;
	if(!sendSMS(PHONE_NUMBER, msg)) return 2;

	return 0;
}


