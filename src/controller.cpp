#include "controller.h"
#include "main_menu.h"
#include "settings_menu.h"
#include "calibration_menu.h"
#include "display.h"
#include "scale.h"

uint8_t controller::selected_menu = 2;

void controller::onButtonPress(){
	if(selected_menu == 0){
		main_menu::onButtonPress();
	}
	else if(selected_menu == 1){
		settings_menu::onButtonPress();
	}
	else if(selected_menu == 2){
		calibration_menu::onButtonPress();
	}
}

void controller::onKnobRotate(int8_t direction, int8_t multiplier){
	if(selected_menu == 0){
		main_menu::onKnobRotate(direction, multiplier);
	}
	else if(selected_menu == 1){
		settings_menu::onKnobRotate(direction, multiplier);
	}
	else if(selected_menu == 2){
		calibration_menu::onKnobRotate(direction, multiplier);
	}
}


void controller::openMenu(uint8_t menu){
	selected_menu = menu;

	if(menu == MAIN_MENU){
		main_menu::init();
	}
	else if(menu == SETTINGS_MENU){
		settings_menu::init();
	}
	else if(menu == CALIBRATION_MENU){
		calibration_menu::init();
	}
}

void controller::render(){
	if(selected_menu == 0){
		main_menu::render();
	}
	else if(selected_menu == 1) {
		settings_menu::render();
	}
	else if(selected_menu == 2) {
		calibration_menu::render();
	}
}