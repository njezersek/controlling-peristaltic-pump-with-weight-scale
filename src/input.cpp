#include "input.h"
#include "controller.h"

DebounceInput encoder_A(ENCODER_A_PIN);
DebounceInput encoder_B(ENCODER_B_PIN);
DebounceInput button(BUTTON_PIN);
DebounceInput pedal(PEDAL_PIN);
unsigned long last_encoder_update = 0;
int8_t prev_dir = 1;

void input::init(){
	encoder_A.setup();
	encoder_B.setup();
	button.setup();
	pedal.setup();
}

void input::update(){
	encoder_A.update();
	encoder_B.update();
	button.update();
	pedal.update();

	unsigned long now = millis();
	if(encoder_A.state && !encoder_A.last_state){
		int8_t dir = (encoder_B.state) ? 1 : -1;

		int8_t multiplier = min(max(30 - (int)(now - last_encoder_update), 1), 20);
		last_encoder_update = now;

		if(prev_dir != dir) multiplier = 1;

		prev_dir = dir;
		controller::onKnobRotate(dir * ENCODER_DIRECTION, multiplier);
	}

	if(button.state && !button.last_state){
		controller::onButtonPress();
	}

	if(pedal.state && !pedal.last_state){
		controller::onPedalPress();
	}
}