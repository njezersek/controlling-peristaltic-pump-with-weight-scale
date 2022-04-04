#pragma once

// DISPLAY
#define CLK 10  	// E
#define CLK_PORT PORTB
#define CLK_MASK 0b01000000
#define CS 2    	// RS
#define CS_PORT PORTD
#define CS_MASK 0b10000010
#define SID 3    	// RW
#define SID_PORT PORTD
#define SID_MASK 0b00000001


// ROTARY ENCODER - KNOB
#define ENCODER_A_PIN A4
#define ENCODER_B_PIN A3
#define BUTTON_PIN A2
#define ENCODER_DIRECTION -1