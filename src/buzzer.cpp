#include "buzzer.h"
#include "pinout.h"
#include "settings.h"

unsigned long buzzer::start_time = 0;
uint8_t buzzer::beep_duration = 0;
uint8_t buzzer::beep_patern = 0;

void buzzer::init(){
	pinMode(BUZZER_PIN, OUTPUT);
}

void buzzer::beep(uint8_t duration, uint8_t patern){
	start_time = millis();
	beep_duration = duration;
	beep_patern = patern;
}

void buzzer::update(){
	unsigned long t = (millis() - start_time) / beep_duration;
	if(t < 8 && settings::sound){
		digitalWrite(BUZZER_PIN, beep_patern & (0x80 >> t));
	}
	else{
		digitalWrite(BUZZER_PIN, LOW);
	}
}