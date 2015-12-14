#include <SoftwareSerial.h>
#include "RTClib.h"
#include <Wire.h>

#define DEBUG
#undef DEBUG_OUTPUT

#define SSID "hamster-wifi2"
#define PASS "134679852"

SoftwareSerial esp8266(10, 11);
DateTime now;
float temp = 23.765;
float pressure = 768.4;

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

bool setup_wifi(bool full_init) {
	bool init_result = true;
	clearSerialBuffer();
	if (full_init)
		init_result &= get_response(F("AT+CWMODE=1"), F("no change"));
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
	Serial.begin(9600);
	Serial.println(">");

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
	now = DateTime(__DATE__, __TIME__);
#ifdef DEBUG	
	Serial.println(F("init done"));
#endif
}

void loop () {
	while (1) {		
		while (Serial.available() > 0) {
			esp8266.write(Serial.read());
		}
		String s = "";
		while (esp8266.available() > 0) {
			char c = esp8266.read();
			s += c;
			if (s.indexOf("HTTP/1.1") != -1) {
				break;
			}
		}		
		if (s.length()) {
			clearSerialBuffer();
			parse_request(s);
		}
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
			int end = request.indexOf(" HTTP/") - 3; // отбрасывание 3-х посл. знаков - милисек.
			String new_time = request.substring(start, end);

			Serial.println("str time = " + new_time);
			unsigned long time = new_time.toInt() + 2 * 60 * 60; // хардкодед +GMT2
			Serial.println("int time = " + String(time));
			now = DateTime(time);
		}

		if (request.indexOf("GET /") != -1) {			
			html = "<html>";
			html += "<h1>Now</h1>";			
			html += "<form name=\"f\" onsubmit=\"document.f.d.value = Date.now();\">";
			html += "<input type=hidden name=\"d\">";		
			html += "Time: " + String(now.hour()) + ":" + String(now.minute()) + " " + String(now.day()) + "." + String(now.month()) + "." + String (now.year()) +
			" <input type=\"submit\" value=\"Set\"><br>";
			html += "Temp: " + String(temp) + " C<br>";
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

void clearSerialBuffer(void) {
	while (esp8266.available() > 0) {
		esp8266.read();
	}
}