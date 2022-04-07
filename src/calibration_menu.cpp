#include "calibration_menu.h"
#include "controller.h"
#include "display.h"
#include "settings.h"
#include "scale.h"

uint8_t calibration_menu::reading_state = 0;
uint8_t calibration_menu::reading_count = 0;
uint8_t calibration_menu::selected_item = 0;
bool calibration_menu::selected = 0;
uint16_t calibration_menu::known_weight = 1000;
long calibration_menu::empty_reading = 0;
long calibration_menu::full_reading = 1000;
bool calibration_menu::empty_reading_set = false;
bool calibration_menu::full_reading_set = false;

void calibration_menu::init(){
	reading_state = 0;
	reading_count = 0;
	selected_item = 0;
	selected = 0;
	// known_weight = 1000;
	empty_reading = 0;
	full_reading = 1000;
	empty_reading_set = false;
	full_reading_set = false;
}

void calibration_menu::setCalibrationFactor(){
	settings::scale_factor = (float)known_weight / (float)(full_reading - empty_reading);
	settings::scale_offset = empty_reading;
}

void calibration_menu::onButtonPress(){
	if(reading_state != 0) return;

	if(selected_item == 3 || selected_item == 4){
		controller::openMenu(SETTINGS_MENU);

		if(selected_item == 4){
			setCalibrationFactor();
		}
	}
	else if(selected_item == 1){
		selected = !selected;
	}
	else if(selected_item == 0){ // measure empty
		reading_state = 1;
		reading_count = 0;
		empty_reading = 0;
	}
	else if(selected_item == 2){ // measure full
		reading_state = 2;
		reading_count = 0;
		full_reading = 0;
	}
}

void calibration_menu::onKnobRotate(int8_t direction, int8_t multiplier){
	if(reading_state != 0) return;

	if(selected){
		if(selected_item == 1){
			known_weight += direction * multiplier;
			if(known_weight < 100){
				known_weight = 100;
			}
			else if(known_weight > 5000){
				known_weight = 5000;
			}
		}
	}
	else{
		uint8_t n = (4 + (empty_reading_set && full_reading_set));
		selected_item = (selected_item + direction + n) % n;
	}
}

void calibration_menu::render(){
	// reading
	if(reading_state == 1){
		empty_reading += scale::filtered_raw_value;
		reading_count++;
		Serial.println(empty_reading);
		if(reading_count >= 110){
			empty_reading /= 110;
			reading_state = 0;
			reading_count = 0;
			empty_reading_set = true;
		}
	}
	if(reading_state == 2){
		full_reading += scale::filtered_raw_value;
		reading_count++;
		if(reading_count >= 110){
			full_reading /= 110;
			reading_state = 0;
			reading_count = 0;
			full_reading_set = true;
		}
	}

	display::clear();
	display::draw_mode = 2;

	// cancle
	if(selected_item == 3){
		display::rectangle(110 + 8 - 8*(empty_reading_set && full_reading_set) , 0, 9, 7);
	}
	display::printBitmap(display::cancle_icon, 112 + 8 - 8*(empty_reading_set && full_reading_set), 1, 5, 5, false);

	// confirm
	if(empty_reading_set && full_reading_set){
		if(selected_item == 4){
			display::rectangle(118, 0, 9, 7);
		}
		display::printBitmap(display::confirm_icon, 120, 1, 6, 5, false);
	}


	// title
	display::print("CALIBRATION", 3, 1);
	display::horizontalLine(0, 7, 128);
	display::verticalLine(0, 0, 8);
	display::verticalLine(127, 0, 8);

	display::print("EMPTY SCALE ", 7, 13);
	display::printNumberChar(0, 26, 23);
	display::printBitmap(display::small_gram_icon, 33, 26, 5, 8, false);

	display::horizontalLine(12, 39, 38);
	display::horizontalLine(12, 49, 38);
	display::horizontalLine(13, 50, 38);
	display::verticalLine(12, 40, 9);
	display::verticalLine(49, 40, 9);
	display::verticalLine(50, 41, 9);
	if(selected_item == 0){
		display::rectangle(13, 40, 36, 9);
	}
	display::print("MEASURE", 15, 42);
	if(empty_reading_set) display::printBitmap(display::confirm_icon, 53, 42, 6, 5, false);

	display::print("KNOWN WEIGHT ", 69, 13);
	display::printNumber(known_weight, 104, 23);
	display::printBitmap(display::small_gram_icon, 106, 26, 5, 8, false);
	if(selected_item == 1){
		display::horizontalLine(77, 21, 38);
		display::horizontalLine(77, 21+14, 38);
		display::verticalLine(77, 22, 13);
		display::verticalLine(77+37, 22, 13);
		if(selected){
			display::rectangle(78, 22, 36, 13);
		}
	}

	display::horizontalLine(65+12, 39, 38);
	display::horizontalLine(65+12, 49, 38);
	display::horizontalLine(65+13, 50, 38);
	display::verticalLine(65+12, 40, 9);
	display::verticalLine(65+49, 40, 9);
	display::verticalLine(65+50, 41, 9);
	if(selected_item == 2){
		display::rectangle(65+13, 40, 36, 9);
	}
	display::print("MEASURE", 80, 42);
	if(full_reading_set) display::printBitmap(display::confirm_icon, 118, 42, 6, 5, false);

	// progress bar
	if(reading_state != 0){
		display::horizontalLine(12, 55, 104);
		display::horizontalLine(12, 55+5, 104);
		display::verticalLine(12, 55, 6);
		display::verticalLine(12+103, 55, 6);

		uint8_t progress = min(102, reading_count);
		display::dither = 0xaa;
		display::horizontalLine(13, 56, progress);
		display::horizontalLine(13, 56+2, progress);
		display::dither = 0x55;
		display::horizontalLine(13, 56+1, progress);
		display::horizontalLine(13, 56+3, progress);
		display::dither = 0xff;
	}


}