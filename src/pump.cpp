#include <SPI.h>

#include "pump.h"
#include "pinout.h"

void pump::init(){
	pinMode(PUMP_DIR_PIN, OUTPUT);
	pinMode(PUMP_DAC_CS_PIN, OUTPUT);
	pinMode(PUMP_START_PIN, OUTPUT);
	digitalWrite(PUMP_DIR_PIN, LOW);
	digitalWrite(PUMP_DAC_CS_PIN, LOW);
	digitalWrite(PUMP_START_PIN, LOW);
	SPI.begin();
}

// max speed: 4096
void pump::setSpeed(uint16_t speed){
	uint16_t data = 0x3000 | speed;
	digitalWrite(PUMP_DAC_CS_PIN, LOW);
	SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
	SPI.transfer((uint8_t)(data >> 8));
	SPI.transfer((uint8_t)(data & 0xFF));
	SPI.endTransaction();
	digitalWrite(PUMP_DAC_CS_PIN, HIGH);
}