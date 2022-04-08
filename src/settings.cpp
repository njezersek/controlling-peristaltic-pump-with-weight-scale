#include <EEPROM.h>
#include "settings.h"

int8_t settings::speed = 100;
int8_t settings::drip = 0;
int8_t settings::start_acceleration = 100;
int8_t settings::sound = 1;
int16_t settings::weight = 250;
int16_t settings::stopping_weight = 100;
long settings::scale_offset = -537943;
float settings::scale_factor = 0.0001;

void settings::load(bool all){
	if(all) EEPROM.get(0, speed);
	if(all) EEPROM.get(1, drip);
	EEPROM.get(2, start_acceleration);
	EEPROM.get(3, sound);
	if(all) EEPROM.get(4, weight);
	EEPROM.get(6, stopping_weight);
	EEPROM.get(8, scale_offset);
	EEPROM.get(12, scale_factor);
}

void settings::save(){
	EEPROM.put(0, speed);
	EEPROM.put(1, drip);
	EEPROM.put(2, start_acceleration);
	EEPROM.put(3, sound);
	EEPROM.put(4, weight);
	EEPROM.put(6, stopping_weight);
	EEPROM.put(8, scale_offset);
	EEPROM.put(12, scale_factor);
}