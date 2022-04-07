#pragma once
#include <Arduino.h>

#define MAIN_MENU 0
#define SETTINGS_MENU 1
#define CALIBRATION_MENU 2

namespace controller{
	extern uint8_t selected_menu;

	void onKnobRotate(int8_t direction, int8_t multiplier);
	void onButtonPress();
	void render();
	void openMenu(uint8_t menu);
}