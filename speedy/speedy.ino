/*
 Name:    speedy.ino
 Created: 10/16/2016 2:47:14 PM
 Author:  batonchik
*/

#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS _gps;

void setup() {
	Serial.begin(9600);
	Serial1.begin(115200);
}
unsigned long time = 0;

void loop() {
	float lat, lon, speed;
	unsigned long fix_age;
	unsigned short satts;

	while (Serial1.available()) {
		char c = (char)Serial1.read();
		_gps.encode(c);
	}
	_gps.f_get_position(&lat, &lon, &fix_age);
	speed = _gps.f_speed_kmph();
	satts = _gps.satellites();

	String out = "[lat: " + String(lat,6) + "] [long: " + String(lon,6) + "] [speed: " + String(speed,3) + "] [satts: " + String(satts) + "]";
	Serial.println(out);
}