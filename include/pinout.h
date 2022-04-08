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

// SCALE HX711
#define SCALE_CLK_PIN 11
#define SCALE_DT_PIN 12
#define SCALE_GAIN 1 // 1: 128 channel A, 2: 32 channel B, 3: 64 channel A

// BUZZER
#define BUZZER_PIN A5

// PUMP INTERFACE
#define PUMP_DIR_PIN 6
#define PUMP_START_PIN 7
#define PUMP_DAC_CS_PIN 4
