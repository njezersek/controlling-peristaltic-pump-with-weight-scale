#include "knob.h"
#include "controller.h"

DebounceInput encoder_A(ENCODER_A_PIN);
DebounceInput encoder_B(ENCODER_B_PIN);
DebounceInput button(BUTTON_PIN);
unsigned long last_encoder_update = 0;
int8_t prev_dir = 1;

void knob::setup(){
	encoder_A.setup();
	encoder_B.setup();
	button.setup();
}

void knob::update(){
	encoder_A.update();
	encoder_B.update();
	button.update();

	unsigned long now = millis();
	if(encoder_A.state && !encoder_A.last_state){
		int8_t dir = (encoder_B.state) ? 1 : -1;

		int8_t multiplier = min(max(1000 / (now - last_encoder_update), 1), 50);
		last_encoder_update = now;

		if(prev_dir != dir) multiplier = 1;

		prev_dir = dir;
		controller::onKnobRotate(dir * ENCODER_DIRECTION, multiplier);
	}

	if(button.state && !button.last_state){
		controller::onButtonPress();
	}
}