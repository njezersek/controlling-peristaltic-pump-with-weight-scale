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

const uint16_t display_mean_n = 200;
int16_t display_mean_buffer[display_mean_n];
uint16_t display_mean_buffer_index = 0;
long display_mean = 0;
long display_mean_dynamic = 0;

long display_median_buffer[3];
uint8_t display_median_buffer_index = 0;

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

	// mean filter
	mean_buffer[mean_buffer_index] = median;
	mean_buffer_index = (mean_buffer_index + 1) % mean_n;

	filtered_raw_value = 0;
	for(int i=0; i<mean_n; i++){
		filtered_raw_value += mean_buffer[i];
	}


	weight = (float)(filtered_raw_value - settings::scale_offset) * settings::scale_factor * 10.0f;

	// limit weight to 3kg
	if(weight < -30000){
		weight = -30000;
	}
	else if(weight > 30000){
		weight = 30000;
	}

	// display mean filter
	display_mean_buffer[display_mean_buffer_index] = weight;

	display_mean = 0;
	for(uint16_t i=0; i<display_mean_n; i++){
		display_mean += display_mean_buffer[i];
	}
	display_mean /= display_mean_n;

	uint8_t look_back = min(max(20+display_mean_n - abs(display_mean - weight)*10, 1), display_mean_n); // dynamicly set the size of rolling mean
	display_mean_dynamic = 0;
	for(uint16_t i=0; i<look_back; i++){
		display_mean_dynamic += display_mean_buffer[(display_mean_buffer_index + i) % display_mean_n];
	}
	display_mean_dynamic /= look_back;

	display_mean_buffer_index = (display_mean_buffer_index + 1) % display_mean_n;



	unsigned long now = millis();
	if(now - last_display_update > 200){
		display_median_buffer[display_median_buffer_index] = (display_mean_dynamic + 5) / 10;
		display_median_buffer_index = (display_median_buffer_index + 1) % 3;
		last_display_update = now;

		if((display_median_buffer[1] <= display_median_buffer[0] && display_median_buffer[0] <= display_median_buffer[2]) || (display_median_buffer[2] <= display_median_buffer[0] && display_median_buffer[0] <= display_median_buffer[1]) ){
			weight_display = display_median_buffer[0];
		}
		else if((display_median_buffer[0] <= display_median_buffer[1] && display_median_buffer[1] <= display_median_buffer[2]) || (display_median_buffer[2] <= display_median_buffer[1] && display_median_buffer[1] <= display_median_buffer[0]) ){
			weight_display = display_median_buffer[1];
		}
		else if((display_median_buffer[0] <= display_median_buffer[2] && display_median_buffer[2] <= display_median_buffer[1]) || (display_median_buffer[1] <= display_median_buffer[2] && display_median_buffer[2] <= display_median_buffer[0]) ){
			weight_display = display_median_buffer[2];
		}
	}

	weight /= 10;

	// Serial.print("w:");
	// Serial.print(weight);
	// Serial.print(" mw:");
	// Serial.print(display_mean);
	// Serial.print(" dw:");
	// Serial.print(weight_display);
	// Serial.print(" look_back:");
	// Serial.print(look_back);
	// Serial.println();
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