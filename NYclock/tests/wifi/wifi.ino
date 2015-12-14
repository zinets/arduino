#include <SoftwareSerial.h>

#define DEBUG true
#define SSID "hamster-wifi2"
#define PASS "134679852"

SoftwareSerial esp8266(10, 11);
int incomingByte = 0;
float temp = 25.4;
float pressure = 768.2;

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];

void receive(){
  delay(2000);
  while (esp8266.available()) {
    incomingByte = esp8266.read();
    Serial.write(incomingByte);
  }
  Serial.println();
}

String get_response(String AT_Command, int delay_value){
  String tmpData;
  #ifdef DEBUG
  Serial.println(AT_Command);
  #endif
  esp8266.println(AT_Command);
  delay(delay_value);
  while (esp8266.available() > 0)  {
    char c = esp8266.read();
    tmpData += c;
    
    if (tmpData.indexOf(AT_Command) > -1)         
      tmpData = "";
    else
      tmpData.trim();       
          
   }
   return tmpData;
}

void setup_wifi() {
	// esp8266.println(F("AT+RST"));
	// receive();

	// esp8266.println(F("AT+CWMODE=1"));
	// receive();

	// esp8266.println("AT+CWJAP=\"hamster-wifi2\",\"134679852\"");
	// delay(5000);
	// receive();

	// esp8266.println("AT+CIFSR");
	// receive();

	// esp8266.println("AT+CIPMODE=0");
	// receive();

	// esp8266.println("AT+CIPMUX=1");
	// receive();

	// esp8266.println("AT+CIPSERVER=1,80");
	// receive();

	clearSerialBuffer();
	
	Serial.println(get_response(F("AT+CWMODE=1"), 500));
	Serial.println(get_response(F("AT+CWJAP=\"hamster-wifi2\",\"134679852\""), 5000));
	Serial.println(get_response(F("AT+CIFSR"), 100));
	Serial.println(get_response(F("AT+CIPMODE=0"), 100));
	Serial.println(get_response(F("AT+CIPMUX=1"), 100));
	Serial.println(get_response(F("AT+CIPSERVER=1,80"), 100));

}

void setup() {
	esp8266.begin(9600);
	Serial.begin(9600);
	Serial.println("Starting");
	setup_wifi();
}

void loop() {
	// delay(500);
	
	// if (esp8266.available()) {
	// 	if (esp8266.find("/time")) {
	// 		String txt = "<h1>Hello</h1>";
	// 		esp8266.println("AT+CIPSEND=0,16");		
	// 		esp8266.println(txt);	
	// 	}


	// 	// while(esp8266.available()) {
	// 	// 	incomingByte = esp8266.read();
	// 	// 	Serial.write(incomingByte);
	// 	// }
	// 	// Serial.println("");
		
	// 	// String txt = "<h1>Hello</h1>";
	// 	// esp8266.println("AT+CIPSEND=0,16");		
	// 	// esp8266.println(txt);
	// 	// receive();


	// 	esp8266.println("AT+CIPCLOSE=0");
	// 	receive();
	// }
}

void clearSerialBuffer(void) {
	while (esp8266.available() > 0) {
		esp8266.read();
	}
}

void clearBuffer(void) {
	for (int i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
}