// HAL
#define START_BUTTON 	12 // дальше 11, 10, 9...
#define NUM_COLS		3  // количество "столбцов" кнопок
#define PULL_LINE 		2  // и дальше (если будет) 2, 3..
#define LED_PLUS		8
#define LED_MINUS		9
#define SERVO_INPUT		13

#define DEBUG

#include <Servo.h>

// states
#define STATE_WAIT           0
#define STATE_PRESSED        STATE_WAIT + 1

// variables
Servo lock_gear;
int curButton = START_BUTTON;
int pressedButton;
int curState = STATE_WAIT;

void setup() {
	// setup servo
	lock_gear.attach(SERVO_INPUT);

	// setup button line selector
	pinMode(PULL_LINE, OUTPUT);
	digitalWrite(PULL_LINE, LOW);

	// setup pins for buttons
	for (int x = 0; x < NUM_COLS; x++) {
		pinMode(START_BUTTON - x, INPUT);
		digitalWrite(START_BUTTON - x, HIGH);
	}

	#ifdef DEBUG
	Serial.begin(115200);
	#endif
}

void loop() {
	switch (curState) {
		case STATE_WAIT: {
			for (int x = 0; x < NUM_COLS; x++) {
				if (digitalRead(x) == LOW) {
					curState = STATE_PRESSED;
					pressedButton = x;
#ifdef DEBUG
					Serial.print("Pressed..");
#endif
					break;
				}
			}
		} break;
		case STATE_PRESSED: {
			if (digitalRead(pressedButton) == HIGH) {
				curButton = pressedButton;
				curState = STATE_WAIT;
				pressedButton = 0;
#ifdef DEBUG
				Serial.println(curButton, HEX);
#endif
			}
		} break;
	}
}