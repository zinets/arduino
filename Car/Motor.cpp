#include "Motor.h"
	Motor::Motor(int trottle, int input1, int input2) {
		trottlePin = trottle;
		motorPin1 = input1;
		motorPin2 = input2;

		pinMode(trottlePin, OUTPUT);
		pinMode(motorPin1, OUTPUT);
		pinMode(motorPin1, OUTPUT);
	}

	byte powerToHex(int power) {
		return (byte)power * 2.5;
	}

	void Motor::forward(int power) {
		intState = MotorStateRunForward;
		
		digitalWrite(motorPin1, HIGH);
		digitalWrite(motorPin2, LOW);
		analogWrite(trottlePin, powerToHex(power));
	}

	void Motor::backward(int power) {
		intState = MotorStateRunBackward;
	
		digitalWrite(motorPin1, LOW);
		digitalWrite(motorPin2, HIGH);
		analogWrite(trottlePin, powerToHex(power));
	}

	void Motor::stop() {
		intState = MotorStateStopped;
		analogWrite(trottlePin, 0);
		digitalWrite(motorPin1, LOW);
		digitalWrite(motorPin2, LOW);
	}