#include <EEPROM.h>
#include "settings.h"

int8_t settings::speed = 100;
int16_t settings::weight = 250;
int8_t settings::drip = 0;
int16_t settings::stopping_weight = 100;
int8_t settings::start_acceleration = 100;
int8_t settings::sound = 1;
long settings::scale_offset = -537943;
float settings::scale_factor = 0.0001;

void settings::load(){
	// Load settings from EEPROM
}

void settings::save(){
	// Save settings to EEPROM
}