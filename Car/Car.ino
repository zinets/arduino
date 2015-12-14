#include "Motor.h"

Motor motorLeft(3, 2, 4);
Motor motorRight(5, 6, 7);
char incomingByte; 

void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
  	incomingByte = Serial.read();
  	switch(incomingByte) {
  		case 'F':
  			motorLeft.forward(90);
  			motorRight.forward(90);
  		break;
  		case 'S':
  			motorLeft.stop();
  			motorRight.stop();
  		break;
  		case 'L': // left
  			motorRight.forward(80);
  			motorLeft.stop();
  		break;
  		case 'R': // right
  			motorRight.stop();
  			motorLeft.forward(80);
  		break;
      case 'B':
        motorLeft.backward(70);
        motorRight.backward(70);
      break;
      case 'G': // f-l
        motorLeft.forward(70);
        motorRight.forward(90);
      break;
      case 'I': // f-r
        motorLeft.forward(90);
        motorRight.forward(70);
      break;
  	}
  }
}
