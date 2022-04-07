#include <Arduino.h>

namespace calibration_menu{
	extern uint8_t reading_state;
	extern uint8_t selected_item;
	extern bool selected;
	extern uint16_t known_weight;
	extern long empty_reading;
	extern long full_reading;
	extern uint8_t reading_count;
	extern bool empty_reading_set;
	extern bool full_reading_set;

	void onButtonPress();
	void onKnobRotate(int8_t direction, int8_t multiplier);
	void render();
	void init();
	void setCalibrationFactor();
}