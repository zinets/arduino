#include "NYclock.h"
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <SoftwareSerial.h>

enum {modeClock, modeDate, modeTemp, modePressure, modeRemain, modeEnd};

int period = 0;
int NYremain = 0;
#define periodClock  5
#define periodDate   3
#define periodTemp   3
#define periodPressure 3
#define periodRemain 4
const int currentPeriod[] = {periodClock, periodDate, periodTemp, periodPressure, periodRemain};

const int flowModes[] = {modeClock, modeDate, modeRemain, modeClock, modeTemp, modePressure, modeEnd};
int flowStep = 0;

typedef struct FLAGS {
	unsigned char time_update: 1;
	unsigned char temp_update: 1;
	unsigned char dot_position: 3;
} FLAGS;
volatile FLAGS flags;

int digit = 0;
int d1, d2, d3, d4;
volatile int timer_count = 0;

RTC_DS1307 RTC;
Adafruit_BMP085_Unified bmp;
DateTime now;
float pressure;
float temperature;
SoftwareSerial esp8266(2,3);

#define SIGN_C	10
#define SIGN_N  11
#define SIGN_Y  12
#define SIGN_MINUS 13

const int pb_d[] = {PORTB_0, PORTB_1, PORTB_2, PORTB_3, PORTB_4, PORTB_5, PORTB_6, PORTB_7, PORTB_8, PORTB_9, PORTB_C, PORTB_N, PORTB_Y, PORTB_MIN};
const int pd_d[] = {PORTD_0, PORTD_1, PORTD_2, PORTD_3, PORTD_4, PORTD_5, PORTD_6, PORTD_7, PORTD_8, PORTD_9, PORTD_C, PORTD_N, PORTD_Y, PORTD_MIN};

void show_digit(int digit, int show_point) {
	PORTB &= ~PORTB_E;
	PORTD &= ~PORTD_E;

	PORTB |= pb_d[digit];
	PORTD |= pd_d[digit];

	if (show_point) {
		PORTD |= (1 << SEG_DP_SHIFT);
	} else {
		PORTD &= ~ (1 << SEG_DP_SHIFT);
	}
}

void show_next_digit () {
	switch (digit) {
		case 0:
		CAT4 = 1;
		CAT1 = 0;
		show_digit(d1, flags.dot_position == 1);
		break;
		case 1:
		CAT1 = HIGH;
		CAT2 = LOW;
		show_digit(d2, flags.dot_position == 2);
		break;
		case 2:
		CAT2 = HIGH;
		CAT3 = LOW;
		show_digit(d3, flags.dot_position == 3);
		break;
		case 3:
		CAT3 = HIGH;
		CAT4 = LOW;
		show_digit(d4, flags.dot_position == 4);
		break;
	}
	digit = ++digit % 4;
}

ISR(TIMER2_COMPA_vect) {
	show_next_digit();
	if (++timer_count > 500) {
		timer_count = 0;
		flags.time_update = 1;
	}
	switch (flowModes[flowStep]) {
		case modeClock:
		flags.dot_position = timer_count < 350 ? 0 : 2;
		break;
		
		case modeDate:
		flags.dot_position = 2;
		break;

		case modeTemp:
		flags.dot_position = temperature < 0 ? 0 : 2;
		break;
		
		case modePressure:
		flags.dot_position = 3;
		break;
		
		default:
		flags.dot_position = 0;
		break;
	}
}

void setup() {
	noInterrupts();
	//{{AVR_IO_DESIGNER_INIT
	// C:\Users\Zinetz Victor\Documents\Arduino\NYclock\NYclock.aid

	//
	// Project Options - NYclock
	//
	// Device Name = ATmega328P
	// CPU Frequency (F_CPU) = 16,0 MHz
	// IC Package = TQFP
	// Sort Pins by = Pin Number
	// Output Code File = NYclock.ino
	// Output Header File = NYclock.h
	// Assume Default State = True
	// Write Unmodified Bits = True
	// Set Unused PORT Pins = Input

	//
	// PORTB variable initializations
	//
	// PORTB0 for 1: SEG_B as Output - 
	// PORTB1 for 1: SEG_A as Output - 
	// PORTB2 for 1: CAT1 as Output - 
	// PORTB3 for 1: CAT2 as Output - 
	// PORTB4 for 1: CAT3 as Output - 
	// PORTB5 for 1: CAT4 as Output - 
	PORTB = 0x3C;
	DDRB = 0x3F;

	//
	// PORTD variable initializations
	//
	// PORTD2 for 1: SEG_DP as Output - 
	// PORTD3 for 1: SEG_G as Output - 
	// PORTD4 for 1: SEG_F as Output - 
	// PORTD5 for 1: SEG_E as Output - 
	// PORTD6 for 1: SEG_D as Output - 
	// PORTD7 for 1: SEG_C as Output - 
	DDRD = 0xFC;

	//
	// TIMER_COUNTER_2 -  - 
	//
	// Waveform Generation Mode = Clear Timer on Compare Match TOP=OCRA
	// Clock Select = clk / 128 (From prescaler)
	// Output Compare Register A = 249
	// Output Compare Match Interrupt Enable A = Enabled
	// Counter Frequency = 500,0 Hz
	// Counter Period = 2,0 msec
	// Seconds per Count = 8,0 usec
	// Counts per Second = 125,0 kHz
	// Output Compare Register A Time = 1,992 msec
	TIMSK2 = 0x02;	// (1<<OCIE2A)|(0<<TOIE2)
	TCCR2A = 0x02;	// (1<<WGM21)|(0<<WGM20)
	TCCR2B = 0x05;	// (0<<WGM22)|(1<<CS22)|(0<<CS21)|(1<<CS20)
	OCR2A = 0xF9;
	//}}AVR_IO_DESIGNER_INIT

	interrupts();

	Serial.begin(57600);
	Wire.begin();
	RTC.begin();
	flags.time_update = 1;
	flags.temp_update = 1;

	bmp = Adafruit_BMP085_Unified(12345);
	if (!bmp.begin()) {
		Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
		while(1);
	}
}

void nextState() {
	if (++period >= currentPeriod[flowModes[flowStep]]) {
		flowStep++;
		if (flowModes[flowStep] == modeRemain && NYremain == 0) {
			flowStep++;
		}
		if (flowModes[flowStep] == modeEnd) {
			flowStep = 0;
		}
		flags.temp_update = flowModes[flowStep] == modeDate;
		period = 0;
	}
}

void loop() {	
	if (flags.temp_update) {
		flags.temp_update = 0;		
		
		sensors_event_t event;
		bmp.getEvent(&event);
		pressure = 0.75 * event.pressure;		
		bmp.getTemperature(&temperature);	
	}

	if (flags.time_update) {
		flags.time_update = 0;

		now = RTC.now();	

		if (now.month() == 11) {
			NYremain = 31 + 30 - now.day() + 1;
		} else if (now.month() == 12) {
			NYremain = 31 - now.day() + 1;
		} else {
			NYremain = 0;
		}		

		nextState();		
		switch (flowModes[flowStep]) {
			case modeClock:
			d1 = now.hour() / 10;
			d2 = now.hour() % 10;
			d3 = now.minute() / 10;
			d4 = now.minute() % 10;
			break;

			case modeDate:
			d1 = now.day() / 10;
			d2 = now.day() % 10;
			d3 = now.month() / 10;
			d4 = now.month() % 10;
			break;

			case modeTemp:
			if (temperature < 0) {
				d1 = SIGN_MINUS;
				d2 = -(int)temperature / 10;
				d3 = -(int)temperature % 10;
				d4 = SIGN_C;			
			} else {
				d1 = (int)temperature / 10;
				d2 = (int)temperature % 10;
				d3 = (int)(temperature * 10) % 10;
				d4 = SIGN_C;			
			}
			break;

			case modePressure: 
			d1 = (int)(pressure / 100) % 10;
			d2 = (int)(pressure / 10) % 10; 
			d3 = (int)pressure % 10;
			d4 = (int)(pressure * 10) % 10;			
			break;
			
			case modeRemain:
			d1 = SIGN_N;
			d2 = SIGN_Y;
			d3 = NYremain / 10;
			d4 = NYremain % 10;
			break;

			default:
			break;
		}
	}
}