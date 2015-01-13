#include "NYclock.h"
#include <Wire.h>
#include "RTClib.h"

enum {modeClock, modeDate, modeNYRemain};
int mode = modeClock;
int period = 0;
#define periodClock  5
#define periodDate   3
const int currentPeriod[] = {periodClock, periodDate};

typedef struct FLAGS {
  unsigned char time_update: 1;
} FLAGS;
volatile FLAGS flags;

int digit = 0;
int d1, d2, d3, d4, dp;
volatile int timer_count = 0;

RTC_DS1307 RTC;

const int pb_d[] = {PORTB_0, PORTB_1, PORTB_2, PORTB_3, PORTB_4, PORTB_5, PORTB_6, PORTB_7, PORTB_8, PORTB_9};
const int pd_d[] = {PORTD_0, PORTD_1, PORTD_2, PORTD_3, PORTD_4, PORTD_5, PORTD_6, PORTD_7, PORTD_8, PORTD_9};

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
      show_digit(d1, 0);
      break;
    case 1:
      CAT1 = HIGH;
      CAT2 = LOW;
      show_digit(d2, dp);
      break;
    case 2:
      CAT2 = HIGH;
      CAT3 = LOW;
      show_digit(d3, 0);
      break;
    case 3:
      CAT3 = HIGH;
      CAT4 = LOW;
      show_digit(d4, 0);
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
  switch (mode) {
    case modeClock:
      dp = timer_count < 350;
      break;
    case modeDate:
      dp = 1;
      break;
    default:
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
}

void nextState() {
  if (++period >= currentPeriod[mode]) {
    switch (mode) {
      case modeClock:
        mode = modeDate;
        break;
      case modeDate:
        mode = modeClock;
        break;
    }
    period = 0;
  }
}

void loop() {
  if (flags.time_update) {
    flags.time_update = 0;
    DateTime now = RTC.now();
    int remain = 0;
    if (now.month() == 11) {
      remain = 31 + 30 - now.day();
    } else if (now.month() == 12) {
      remain = 31 - now.day();
    }
    switch (mode) {
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
      default:
        break;
    }
    nextState();
  }
}

