#include <Arduino.h>
#include "scale.h"
#include "pinout.h"
#include "settings.h"

long median_buffer[3];
uint8_t median_buffer_index = 0;
long median = 0;

const uint8_t mean_n = 5;
long mean_buffer[mean_n];
uint8_t mean_buffer_index = 0;
long mean = 0;

unsigned long last_display_update = 0;

long scale::weight = 0;
long scale::weight_display = 0;
long scale::filtered_raw_value = 0;

void scale::init(){
	pinMode(SCALE_CLK_PIN, OUTPUT);
	pinMode(SCALE_DT_PIN, INPUT);
}

void scale::update(){
	long reading = read_raw();

	// median filter
	median_buffer[median_buffer_index] = reading;
	median_buffer_index = (median_buffer_index + 1) % 3;

	if((median_buffer[1] <= median_buffer[0] && median_buffer[0] <= median_buffer[2]) || (median_buffer[2] <= median_buffer[0] && median_buffer[0] <= median_buffer[1]) ){
		median = median_buffer[0];
	}
	else if((median_buffer[0] <= median_buffer[1] && median_buffer[1] <= median_buffer[2]) || (median_buffer[2] <= median_buffer[1] && median_buffer[1] <= median_buffer[0]) ){
		median = median_buffer[1];
	}
	else if((median_buffer[0] <= median_buffer[2] && median_buffer[2] <= median_buffer[1]) || (median_buffer[1] <= median_buffer[2] && median_buffer[2] <= median_buffer[0]) ){
		median = median_buffer[2];
	}
	else{
		median = 123;
	}

	// mean filter
	mean_buffer[mean_buffer_index] = median;
	mean_buffer_index = (mean_buffer_index + 1) % mean_n;

	filtered_raw_value = 0;
	for(int i=0; i<mean_n; i++){
		filtered_raw_value += mean_buffer[i];
	}

	weight = (filtered_raw_value - settings::scale_offset) * settings::scale_factor;

	unsigned long now = millis();
	if(now - last_display_update > 200){
		last_display_update = now;
		weight_display = weight;
		Serial.println(weight_display);
	}
}

bool scale::is_ready(){
	return digitalRead(SCALE_DT_PIN) == LOW;
}

void scale::wait_ready(){
	while(!is_ready()){};
}

long scale::read_raw(){
	wait_ready();

	// Define structures for reading data into.
	unsigned long value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;

	noInterrupts();

	// Pulse the clock pin 24 times to read the data.
	data[2] = shiftIn(SCALE_DT_PIN, SCALE_CLK_PIN, MSBFIRST);
	data[1] = shiftIn(SCALE_DT_PIN, SCALE_CLK_PIN, MSBFIRST);
	data[0] = shiftIn(SCALE_DT_PIN, SCALE_CLK_PIN, MSBFIRST);

	// Set the channel and the gain factor for the next reading using the clock pin.
	for (unsigned int i = 0; i < SCALE_GAIN; i++) {
		digitalWrite(SCALE_CLK_PIN, HIGH);
		digitalWrite(SCALE_CLK_PIN, LOW);
	}

	interrupts();

	// Replicate the most significant bit to pad out a 32-bit signed integer
	if (data[2] & 0x80) {
		filler = 0xFF;
	} else {
		filler = 0x00;
	}

	// Construct a 32-bit signed integer
	value = ( static_cast<unsigned long>(filler) << 24
			| static_cast<unsigned long>(data[2]) << 16
			| static_cast<unsigned long>(data[1]) << 8
			| static_cast<unsigned long>(data[0]) );

	return static_cast<long>(value);
}