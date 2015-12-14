#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
typedef enum {
	MotorStateStopped,
	MotorStateRunForward,
	MotorStateRunBackward,
} MotorState;

class Motor {
	byte trottlePin;
	byte motorPin1, motorPin2;
	MotorState intState;
public:
	Motor(int trottle, int input1, int input2);
	// power - 0-100
	void forward(int power);
	void backward(int power);
	void stop();
};

#endif