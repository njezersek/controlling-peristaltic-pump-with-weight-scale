#include "controller.h"
#include "settings_menu.h"
#include "main_menu.h"
#include "display.h"
#include "scale.h"

uint8_t controller::selected_menu = 0;

void controller::onButtonPress(){
	if(selected_menu == 0){
		main_menu::onButtonPress();
	}
	else if(selected_menu == 1){
		settings_menu::onButtonPress();
	}
}

void controller::onKnobRotate(int8_t direction, int8_t multiplier){
	if(selected_menu == 0){
		main_menu::onKnobRotate(direction, multiplier);
	}
	else if(selected_menu == 1){
		settings_menu::onKnobRotate(direction, multiplier);
	}
}


void controller::render(){
	if(selected_menu == 0){
		main_menu::render();
	}
	else {
		settings_menu::render();
	}
}