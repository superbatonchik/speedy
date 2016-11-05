/*
 Name:    speedy.ino
 Created: 10/16/2016 2:47:14 PM
 Author:  batonchik
*/

#include <pcd8544.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS _gps;
uint64_t time = 0;
unsigned short current_row = 1;

void setup() {
	LcdInit();
	LcdContrast(0x7F);

	Serial.begin(115200);
	Serial1.begin(115200);
}

void loop() {
	
	float lat, lon, speed;
	unsigned long fix_age;
	unsigned short satts;

	while (Serial.available()) {
		char c = (char)Serial.read();
		Serial1.write(c);
		Serial.print(c);
	}

	while (Serial1.available()) {
		char c = (char)Serial1.read();
		_gps.encode(c);
		Serial.print(c);
	}
	_gps.f_get_position(&lat, &lon, &fix_age);
	speed = _gps.f_speed_kmph();
	satts = _gps.satellites();
	String sLat = "[" + String(lat, 6) + "]";
	String sLon = "[" + String(lon, 6) + "]";
	String sSpeed = "[s:" + String(speed,1) + "]";

	LcdClear();
	LcdGotoXYFont(1, 1);
	LcdStr(FONT_1X, (byte *)sLat.c_str());

	LcdGotoXYFont(1, 2);
	LcdStr(FONT_1X, (byte *)sLon.c_str());

	LcdGotoXYFont(1, 3);
	LcdStr(FONT_1X, (byte *)sSpeed.c_str());

	LcdUpdate();

	//String out = sLat + String(" ") + sLon + String(" ") + sSpeed + ";";
	//Serial.println(out);
}
