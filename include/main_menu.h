#include <Arduino.h>

namespace main_menu{
	extern uint8_t selected_item;
	extern bool selected;
	
	void onKnobRotate(int8_t direction, int8_t multiplier);
	void onButtonPress();
	void render();
}