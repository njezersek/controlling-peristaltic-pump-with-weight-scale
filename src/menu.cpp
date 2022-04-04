#include "menu.h"
#include "display.h"

uint8_t menu::selected_item = 0;
bool menu::selected = false;

int8_t menu::speed = 0;
int16_t menu::weight = 0;
int8_t menu::drip = 0;

void menu::onButtonPress(){
	if(selected_item < 3){
		selected = !selected;
	}
}

void menu::onKnobRotate(int direction){
	if(selected){
		switch(selected_item){
			case 0:
				speed += direction;
				if(speed > 100)	speed = 100;
				else if(speed < 0) speed = 0;
				break;
			case 1:
				weight += direction;
				if(weight > 1000)	weight = 1000;
				else if(weight < 0) weight = 0;
				break;
			case 2:
				drip += direction;
				if(drip > 100)	drip = 100;
				else if(drip < 0) drip = 0;
				break;
		}
	}
	else{
		uint8_t d = direction / abs(direction);
		selected_item = (selected_item + d + 4) % 4;
	}
}

#define SETTINGS_Y 42
#define SETTINGS_H 22
#define ITEM_W 34

void menu::render(){
	renderMain();
}


void menu::renderMain(){
	display::clear();

	// display::print("HELLO WORLD", 0, 0);

	display::draw_mode = 2;

	// weight display
	display::print7Segment(selected_item, 43, 3);
	display::print7Segment(selected_item, 43+19, 3);
	display::print7Segment(selected_item, 43+19*2, 3);
	display::print7Segment(selected_item, 43+19*3, 3);

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
	display::printNumber(speed, ITEM_W*1 - 8, SETTINGS_Y+10);
	display::printNumber(weight, ITEM_W*2 - 8, SETTINGS_Y+10);
	display::printNumber(drip, ITEM_W*3 - 8, SETTINGS_Y+10);
	// setting item icons
	display::printBitmap(display::small_percent_icon, ITEM_W*1 - 8 + 1, SETTINGS_Y+13, 6, 8, false);
	display::printBitmap(display::small_gram_icon, ITEM_W*2 - 8 + 1, SETTINGS_Y+13, 6, 8, false);
	display::printBitmap(display::small_gram_icon, ITEM_W*3 - 8 + 1, SETTINGS_Y+13, 6, 8, false);


	// settings icon
	display::printBitmap(display::settings_icon, 106, SETTINGS_Y+3, 18, 9, false);
	display::printBitmap(display::settings_icon, 106, SETTINGS_Y+3+9, 18, 9, true);
}