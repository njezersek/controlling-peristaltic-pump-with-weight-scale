#include <SPI.h>

#include "pump.h"
#include "scale.h"
#include "settings.h"
#include "pinout.h"

bool pump::running = false;
unsigned long pump::start_time = 0;

float ramp_up = 0;
uint16_t pump_speed = 0;

void pump::init(){
	pinMode(PUMP_DIR_PIN, OUTPUT);
	pinMode(PUMP_DAC_CS_PIN, OUTPUT);
	pinMode(PUMP_START_PIN, OUTPUT);
	digitalWrite(PUMP_DIR_PIN, LOW);
	digitalWrite(PUMP_DAC_CS_PIN, LOW);
	digitalWrite(PUMP_START_PIN, LOW);
	SPI.begin();
}

void pump::update(){
	// read the scale
	float weight = scale::weight;

	if(running){ // filling state

		if(millis() - start_time > 100){ // increase speed after 100ms
			ramp_up += 0.01;
			if(ramp_up > 1) ramp_up = 1;
			float f = ramp_up - max(0, max(0, weight-settings::weight+settings::stopping_weight)/settings::stopping_weight);
			// float f = ramp_up - (weight/(float)TARGET_WEIGHT);
			if(f < 0) f = 0;
			if(f > 1) f = 1;
			pump_speed = f * settings::speed * 40;
		}

		if(weight >= settings::weight - settings::drip){
			stop();
		}

	}
	setSpeed(pump_speed);
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

void pump::setDirection(bool direction){
	digitalWrite(PUMP_DIR_PIN, direction);
}

void pump::start(){
	digitalWrite(PUMP_START_PIN, HIGH);
	running = true;
	start_time = millis();
}

void pump::stop(){
	digitalWrite(PUMP_START_PIN, LOW);
	running = false;
}