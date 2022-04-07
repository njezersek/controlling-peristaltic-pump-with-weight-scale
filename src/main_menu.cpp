#include "main_menu.h"
#include "controller.h"
#include "display.h"
#include "scale.h"
#include "settings.h"

uint8_t main_menu::selected_item = 0;
bool main_menu::selected = false;

void main_menu::onButtonPress(){
	if(selected_item < 3){
		selected = !selected;
	}
	else{
		selected_item = 0;
		selected = false;
		controller::selected_menu = 1;
	}
}

void main_menu::onKnobRotate(int8_t direction, int8_t multiplier){
	if(selected){
		switch(selected_item){
			case 0:
				settings::speed += direction * multiplier;
				if(settings::speed > 100) settings::speed = 100;
				else if(settings::speed < 1) settings::speed = 1;
				break;
			case 1:
				settings::weight += direction * multiplier;
				if(settings::weight > 1000)	settings::weight = 1000;
				else if(settings::weight < 1) settings::weight = 1;
				break;
			case 2:
				settings::drip += direction * multiplier;
				if(settings::drip > 100) settings::drip = 100;
				else if(settings::drip < 0) settings::drip = 0;
				break;
		}
	}
	else{
		selected_item = (selected_item + direction + 4) % 4;
	}
}

#define SETTINGS_Y 42
#define SETTINGS_H 22
#define ITEM_W 34

void main_menu::render(){
	display::clear();

	display::draw_mode = 2;

	// weight display
	long w = abs(scale::weight_display);
	uint8_t i;
	for(i=0; i<4; i++){
		display::print7Segment(w % 10, 43+19*(3-i), 3);
		w /= 10;
		if(w == 0) break;
	}
	if(scale::weight_display < 0){
		display::horizontalLine(43+19*(3-i)-10, 17, 7);
		display::horizontalLine(43+19*(3-i)-11, 18, 9);
		display::horizontalLine(43+19*(3-i)-10, 19, 7);
	}

	// gram icon
	display::printBitmap(display::gram_icon, 119, 26, 7, 12, false);

	// filling indicator
	display::print("FILLING", 8, 9);
	display::printNumber(123, 26, 17);
	display::printChar('S', 28, 23);


	// settings box
	display::horizontalLine(0, SETTINGS_Y, 128);
	display::verticalLine(0, SETTINGS_Y, SETTINGS_H);
	display::verticalLine(ITEM_W, SETTINGS_Y, SETTINGS_H);
	display::verticalLine(ITEM_W*2, SETTINGS_Y, SETTINGS_H);
	display::verticalLine(ITEM_W*3, SETTINGS_Y, SETTINGS_H);
	display::verticalLine(127, SETTINGS_Y, SETTINGS_H);

	// settings highlight
	if(selected){
		if(selected_item < 3){
			display::rectangle(ITEM_W*selected_item+1, SETTINGS_Y+1, ITEM_W-1, SETTINGS_H-1);
		}
	}

	// settings cursor
	if(selected_item < 3){
		display::cursor(ITEM_W*selected_item+1+16, SETTINGS_Y-1, false);
	}
	else{
		display::cursor(ITEM_W*selected_item+1+11, SETTINGS_Y-1, false);
	}

	// settings text
	display::print("SPEED", 2, SETTINGS_Y+2);
	display::print("WEIGHT", 2 + ITEM_W, SETTINGS_Y+2);
	display::print("DRIP", 2 + ITEM_W*2, SETTINGS_Y+2);
	display::printNumber(settings::speed, ITEM_W*1 - 8, SETTINGS_Y+10);
	display::printNumber(settings::weight, ITEM_W*2 - 8, SETTINGS_Y+10);
	display::printNumber(settings::drip, ITEM_W*3 - 8, SETTINGS_Y+10);
	// setting item icons
	display::printBitmap(display::small_percent_icon, ITEM_W*1 - 8 + 1, SETTINGS_Y+13, 6, 8, false);
	display::printBitmap(display::small_gram_icon, ITEM_W*2 - 8 + 1, SETTINGS_Y+13, 6, 8, false);
	display::printBitmap(display::small_gram_icon, ITEM_W*3 - 8 + 1, SETTINGS_Y+13, 6, 8, false);


	// settings icon
	display::printBitmap(display::settings_icon, 106, SETTINGS_Y+3, 18, 9, false);
	display::printBitmap(display::settings_icon, 106, SETTINGS_Y+3+9, 18, 9, true);
}