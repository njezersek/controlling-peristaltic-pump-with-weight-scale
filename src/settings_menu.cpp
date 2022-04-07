#include "settings_menu.h"
#include "controller.h"
#include "display.h"
#include "settings.h"

uint8_t settings_menu::selected_item = 0;
bool settings_menu::selected = false;

void settings_menu::onButtonPress(){
	if(selected_item < 2){
		selected = !selected;
	}
	else if(selected_item == 2){
		settings::sound = !settings::sound;
	}
	else if(selected_item == 3){
		// open calibration menu
	}
	else{
		controller::selected_menu = 0;
		selected_item = 0;
		selected = false;
	}
}

void settings_menu::onKnobRotate(int8_t direction, int8_t multiplier){
	if(selected){
		if(selected_item == 0){
			settings::stopping_weight += direction * multiplier;
			if(settings::stopping_weight > 250) settings::stopping_weight = 250;
			else if(settings::stopping_weight < 1) settings::stopping_weight = 1;
		}
		else if(selected_item == 1){
			settings::start_acceleration += direction * multiplier;
			if(settings::start_acceleration > 100)	settings::start_acceleration = 100;
			else if(settings::start_acceleration < 1) settings::start_acceleration = 1;
		}
	}
	else{
		int8_t d = direction / abs(direction);
		selected_item = (selected_item + d + 6) % 6;
	}
}


void settings_menu::render(){
	display::clear();
	display::draw_mode = 2;

	display::print("SETTINGS", 3, 1);
	display::horizontalLine(0, 7, 128);
	display::verticalLine(0, 0, 8);
	display::verticalLine(127, 0, 8);

	// cancle
	if(selected_item == 4){
		display::rectangle(110, 0, 9, 7);
	}
	display::printBitmap(display::cancle_icon, 112, 1, 5, 5, false);

	// confirm
	if(selected_item == 5){
		display::rectangle(118, 0, 9, 7);
	}
	display::printBitmap(display::confirm_icon, 120, 1, 6, 5, false);

	// pointer
	if(selected_item < 4){
		display::horizontalLine(0, selected_item * 13 + 13, 1);
		display::horizontalLine(0, selected_item * 13 + 13 + 1, 2);
		display::horizontalLine(0, selected_item * 13 + 13 + 2, 3);
		display::horizontalLine(0, selected_item * 13 + 13 + 3, 2);
		display::horizontalLine(0, selected_item * 13 + 13 + 4, 1);

		if(selected){
			display::rectangle(0, selected_item * 13 + 13 - 4, 128, 13);
		}
	}

	// settings text
	display::print("STOPPING WEIGHT", 5, 13 + 13*0);
	display::printNumber(settings::stopping_weight, 119, 13 + 13*0 -3);
	display::printBitmap(display::small_gram_icon, 120, 13 + 13*0, 4, 8, false);
	display::print("START ACCELERATION", 5, 13 + 13*1);
	display::printNumber(settings::start_acceleration, 119, 13 + 13*1 -3);
	display::printBitmap(display::small_percent_icon, 120, 13 + 13*1, 4, 8, false);
	display::print("SOUND", 5, 13 + 13*2);
	display::printBitmap(display::speaker_icon, 107, 13 + 13*2 - 2, 6, 9, false);
	if(settings::sound){
		display::printBitmap(display::sound_icon, 114, 13 + 13*2 - 3, 5, 11, false);
	}
	else{
		display::printBitmap(display::cancle_icon, 114, 13 + 13*2, 5, 5, false);
	}

	display::print("CALIBRATION", 5, 13 + 13*3);
}