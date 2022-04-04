#include "knob.h"
#include "menu.h"

DebounceInput encoder_A(ENCODER_A_PIN);
DebounceInput encoder_B(ENCODER_B_PIN);
DebounceInput button(BUTTON_PIN);
unsigned long last_encoder_update = 0;

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
		int multiplier = 1;
		if(now - last_encoder_update < 25){
			multiplier = 20;
		}
		else if(now - last_encoder_update < 50){
			multiplier = 10;
		}
		last_encoder_update = now;

		if(encoder_B.state){
			menu::onKnobRotate(1 * multiplier * ENCODER_DIRECTION);
		}
		else{
			menu::onKnobRotate(-1 * multiplier * ENCODER_DIRECTION);
		}
	}

	if(button.state && !button.last_state){
		menu::onButtonPress();
	}
}