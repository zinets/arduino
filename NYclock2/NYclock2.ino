#include "NYclock2.h"
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <SoftwareSerial.h>
#include <SPI.h>

volatile int digit = 0;
const int digits_map[10] = {DIG_0, DIG_1, DIG_2, DIG_3, DIG_4, DIG_5, DIG_6, DIG_7, DIG_8, DIG_9};
int signs[4] = {0};
volatile int timer_count = 0;
typedef struct FLAGS {
	unsigned char time_update: 1;
	unsigned char temp_update: 1;
} FLAGS;
volatile FLAGS flags;

RTC_DS1307 RTC;
DateTime now;

Adafruit_BMP085_Unified bmp;
float pressure;
float temperature;

#undef DEBUG
#undef DEBUG_OUTPUT
#define USE_WIFI

#define SSID "hamster-wifi2"
#define PASS "134679852"

SoftwareSerial esp8266(2, 3);

// ?????????? ???????????? ?????????????? ????????????
#define periodClock  5
#define periodDate   3
#define periodTemp   3
#define periodPressure 3
#define periodRemain 4
const int currentPeriod[] = {periodClock, periodDate, periodTemp, periodPressure, periodRemain};

enum {modeClock, modeDate, modeTemp, modePressure, modeRemain, modeEnd};
const int flowModes[] = {modeClock, modeDate, modeRemain, modeClock, modeTemp, modePressure, modeEnd};
int flowStep = 0;
int period = 0;
int NYremain = 0;

//----------------------------------- wifi communication -------------------------------

bool get_response (String AT_command, String end_token = "") {
	bool res = true;
	String response = "";
#ifdef _DEBUG
	Serial.println(AT_command);
#endif
	esp8266.println(AT_command);
	
	bool found = false;
	unsigned long time = millis();
	while (1) {
		if (esp8266.available()) {
			char c = esp8266.read();
#ifdef DEBUG_OUTPUT			
			Serial.write(c);
#endif
			response += c;
		}

		if (end_token != "" && response.indexOf(end_token) > -1) {
			break;
		}
		if (response.indexOf("OK") > -1) {
			break;
		}

		if (millis() - time > 10000) {
#ifdef DEBUG			
			Serial.println("timeout!");
#endif
			res = false;			
			break;
		}
	}
#ifdef DEBUG
	Serial.println(response);
#endif	
	return res;
}

void clearSerialBuffer(void) {
	while (esp8266.available() > 0) {
		esp8266.read();
	}
}

void parse_request (String request) {
	String html = "";
	String header = "";
	int index = request.indexOf("+IPD");
	if (index != -1) {
#ifdef DEBUG
		Serial.println("\r\n\r\n-> " + request);
		Serial.println(">-"); 
		delay(100);
#endif
		String ch = request.substring(index + 5, index + 6);
		if (request.indexOf("GET /?d=") != -1) {
			// set new time
			int start = request.indexOf("GET /?d=") + 8;
			int end = request.indexOf(" HTTP/") - 3; // ???????????????????????? 3-?? ????????. ???????????? - ??????????????.
			String new_time = request.substring(start, end);

			Serial.println("str time = " + new_time);
			unsigned long time = new_time.toInt() + 2 * 60 * 60; // ?????????????????? +GMT2
			Serial.println("int time = " + String(time));
			now = DateTime(time);
			RTC.adjust(now);
		}

		if (request.indexOf("GET /") != -1) {			
			html = "<html>";
			html += "<h1>Now</h1>";			
			html += "<form name=\"f\" onsubmit=\"document.f.d.value = Date.now();\">";
			html += "<input type=hidden name=\"d\">";		
			html += "Time: " + String(now.hour()) + ":" + String(now.minute()) + " " + String(now.day()) + "." + String(now.month()) + "." + String (now.year()) +
			" <input type=\"submit\" value=\"Set\"><br>";
			html += "Temp: " + String(temperature) + " C<br>";
			html += "Pressure: " + String(pressure) + " mm<br></form>";
			html += "</html>\r\n";
			int len = html.length();
			
			header = "HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: " + String(len) + "\r\n" 
			"Connection: close\r\n\r\n";

			Serial.println("header: " + header);
			len += header.length();
			
#ifdef DEBUG			
			// Serial.print("html: ");
			// Serial.println(html);
			// Serial.println("->"); 
#endif
			esp8266.print("AT+CIPSEND=" + ch + ",");
			esp8266.println(len);

			unsigned long time = millis();
			bool found = false;
			
			while (esp8266.available() > 0) {
				char c = esp8266.read();
				Serial.write(c);
				if (c == '>') {
					found = true;
					break;
				}
			}	
			clearSerialBuffer();

			if (found) {
				esp8266.print(header);
				delay(20);
				esp8266.print(html);
			} else {
				esp8266.println("AT+CIPCLOSE");
			}
		}
	}
}

//-------------------------------- setup ----------------------------

bool setup_wifi(bool full_init) {
	bool init_result = true;
	clearSerialBuffer();
	if (full_init) {
		init_result &= get_response(F("AT+CWMODE=1"), F("no change"));
	}
	init_result &= get_response(F("AT+CWJAP=\"hamster-wifi2\",\"134679852\""));
	init_result &= get_response(F("AT+CIFSR"));
	if (full_init) {
		init_result &= get_response(F("AT+CIPMODE=0"));
		init_result &= get_response(F("AT+CIPMUX=1"));
	}
	init_result &= get_response(F("AT+CIPSERVER=1,80"), F("no change"));

	return init_result;
}

void setup() {
//{{AVR_IO_DESIGNER_INIT
	// C:\projects\arduino\NYclock2\NYclock2.aid

	//
	// Project Options - NYclock2
	//
	// Device Name = ATmega328P
	// CPU Frequency (F_CPU) = 16,0 MHz
	// IC Package = TQFP
	// Sort Pins by = Pin Number
	// Output Code File = NYclock2.ino
	// Output Header File = NYclock2.h
	// Assume Default State = True
	// Write Unmodified Bits = True
	// Set Unused PORT Pins = Input

	//
	// PORTB variable initializations
	//
	// PORTB2 for 1: LATCH as Output - 
	DDRB = 0x04;

	//
	// PORTC variable initializations
	//
	// PORTC0 for 1: CAT1 as Output - Reserved for pin PC0
	// PORTC1 for 1: CAT2 as Output - 
	// PORTC2 for 1: CAT3 as Output - 
	// PORTC3 for 1: CAT4 as Output - 
	DDRC = 0x0F;

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
	TIMSK2 = 0x02;	// (1<<OCIE2A)
	TCCR2A = 0x02;	// (1<<WGM21)|(0<<WGM20)
	TCCR2B = 0x05;	// (0<<WGM22)|(1<<CS22)|(0<<CS21)|(1<<CS20)
	OCR2A = 0xF9;
//}}AVR_IO_DESIGNER_INIT

	LATCH = 0; 
	SPI.begin();
	SPI.transfer(0);
	LATCH = 0; 

	Wire.begin();
	RTC.begin();
	flags.time_update = 1;

	bmp = Adafruit_BMP085_Unified(12345);
	if (!bmp.begin()) {
		while(1);
	}
#ifdef DEBUG
	Serial.begin(9600);
	Serial.println(">");
#endif

#ifdef USE_WIFI
	esp8266.begin(9600);

	bool full_init = false;
	while (!setup_wifi(full_init)) {
		get_response(F("AT+RST"));
		delay(5000);
		full_init = true;
#ifdef DEBUG
		Serial.println("RESTARTING..");
#endif		
	}
#endif

#ifdef DEBUG	
	Serial.println(F("init done"));
#endif
}

//-------------------------------- interrupts ---------------------------------

ISR(TIMER2_COMPA_vect) {
	// digitalWrite(latchPin, LOW); 
	LATCH = 0;  
	// shiftOut(dataPin, clockPin, MSBFIRST, signs[digit]);  
	SPI.transfer(signs[digit]);
	// digitalWrite(latchPin, HIGH); 
	LATCH = 1;

	switch (digit++) {
		case 0: 
		CAT4 = 1;
		CAT1 = 0;
		break;
		case 1:
		CAT1 = 1;
		CAT2 = 0;
		break;
		case 2: 
		CAT2 = 1;
		CAT3 = 0;
		break;
		case 3:
		CAT3 = 1;
		CAT4 = 0;
		break;
	}

	digit %= 4;

	if (++timer_count > 500) {
		timer_count = 0;
		flags.time_update = 1;
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

//----------------------------------- main loop ----------------------------------------

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
	}

	switch (flowModes[flowStep]) {
		case modeClock:
		signs[0] = digits_map [now.hour() / 10];
		signs[1] = digits_map [now.hour() % 10] | (timer_count < 350 ? DIG_DP : 0);
		signs[2] = digits_map [now.minute() / 10];
		signs[3] = digits_map [now.minute() % 10];
		break;

		case modeDate:
		signs[0] = digits_map [now.day() / 10];
		signs[1] = digits_map [now.day() % 10] | DIG_DP;
		signs[2] = digits_map [now.month() / 10];
		signs[3] = digits_map [now.month() % 10];
		break;

		case modeTemp:
		if (temperature < 0) {	
			signs[0] = SIGN_MINUS;
			signs[1] = digits_map [-(int)temperature / 10];
			signs[2] = digits_map [-(int)temperature % 10];
			signs[3] = SIGN_C;			
		} else {
			signs[0] = digits_map [(int)temperature / 10];
			signs[1] = digits_map [(int)temperature % 10] | DIG_DP;
			signs[2] = digits_map [(int)(temperature * 10) % 10];
			signs[3] = SIGN_C;			
		}
		break;

		case modePressure: 
		signs[0] = digits_map [(int)(pressure / 100) % 10];
		signs[1] = digits_map [(int)(pressure / 10) % 10]; 
		signs[2] = digits_map [(int)pressure % 10] | DIG_DP;
		signs[3] = digits_map [(int)(pressure * 10) % 10];			
		break;

		case modeRemain:
		signs[0] = SIGN_N;
		signs[1] = SIGN_Y;
		signs[2] = digits_map[NYremain / 10];
		signs[3] = digits_map[NYremain % 10];
		break;

		default:
		break;
	}	

#ifdef DEBUG
	while (Serial.available() > 0) {
		esp8266.write(Serial.read());
	}
#endif

#ifdef USE_WIFI	
	String s = "";
	while (esp8266.available() > 0) {
		char c = esp8266.read();
		s += c;
		if (s.indexOf("HTTP/1.1") != -1) {
			clearSerialBuffer();
			parse_request(s);
			break;
		}
	}		
#endif
}
