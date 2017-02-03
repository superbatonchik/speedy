/*
 Name:    speedy.ino
 Created: 10/16/2016 2:47:14 PM
 Author:  batonchik
*/

#include <pcd8544.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include "usefultime.h"

#define DEBUG

struct SpeedData {
	float speed; //kmph
	long elapsed; //msecs
	bool is_set;

	SpeedData() : speed(0), elapsed(0), is_set(false) {}

	String toString() {
		int elapsed_secs = elapsed / 1000;
		//sprintf(str, "%.1f kmph, %d:%d", speed, mins(elapsed_secs), secs(elapsed_secs));
		return String(speed, 1) + "kmph;" + String(mins(elapsed_secs)) + ":" + String(secs(elapsed_secs));
	}
};

TinyGPS _gps;

uint64_t _timestamp_started = 0;

float _lat = 0; 
float _lon = 0; 
float _speed = 0;
unsigned long _elapsed = 0;
unsigned long _fix_age;
unsigned short _satts;

bool _is_running = false;

SpeedData _interval0_60;
SpeedData _interval0_100;

void print_message(const char *msg, bool update = false) {
	if (strlen(msg) > 14) {
		LcdGotoXYFont(1, 1);
	}
	else {
		short x = (14 - strlen(msg)) / 2 + 1;
		LcdGotoXYFont(x, 1);
	}
	LcdStr(FONT_1X, (byte*)msg);
	if (update) {
		LcdUpdate();
	}
}

void print_message_xy(const char *msg, byte x, byte y, bool update = false) {
	LcdGotoXYFont(x, y);
	LcdStr(FONT_1X, (byte*)msg);
	if (update) {
		LcdUpdate();
	}
}

void print_debug() {
	String s = "LT" + String(_lat, 6) + "LN" + String(_lon, 6) + "SP" + String(_speed, 1) + "T" + String(_elapsed);
	Serial.println(s);
}

void setup() {
	LcdInit();
	LcdContrast(0x7F);

	Serial.begin(115200);
	Serial1.begin(115200);
}
unsigned long last_millis = 0;

void loop() {
	/*while (Serial.available()) {
		char c = (char)Serial.read();
		Serial1.write(c);
		Serial.print(c);
	}*/
	LcdClear();

	while (Serial1.available()) {
		_gps.encode(Serial1.read());
	}
	_satts = _gps.satellites();
	if (_satts == 0) {
		print_message("no satellites", true);
		return;
	}
	_gps.f_get_position(&_lat, &_lon, &_fix_age);
	_speed = _gps.f_speed_kmph();
	/*if (_speed < 0.5) {
		_speed += (millis() - last_millis) / 10000.0f;
		last_millis = millis();
	}
	else {
		_speed += (millis() - last_millis) / 330.0f;
		last_millis = millis();
	}*/
	if (_speed >= 0.5 && !_is_running) {
		_timestamp_started = millis();
		_is_running = true;
	}
	if (!_is_running && !_interval0_60.is_set && !_interval0_100.is_set) {
		print_message((String(_speed, 1) + "kmph").c_str(), true);
		return;
	}
	else {
		_elapsed = millis() - _timestamp_started;
		
		if (!_interval0_60.is_set) {
			_interval0_60.speed = _speed;
			_interval0_60.elapsed = _elapsed;
			_interval0_60.is_set = _speed >= 60.0F;
		}
		if (!_interval0_100.is_set) {
			_interval0_100.speed = _speed;
			_interval0_100.elapsed = _elapsed;
			_interval0_100.is_set = _speed >= 100.0F;
		}
		_is_running = !(_interval0_60.is_set && _interval0_100.is_set);
		String str_speed = String(_speed, 1) + "kmph;" + String(mins(_elapsed / 1000)) + ":" + String(secs(_elapsed / 1000));

		String str_0_60 = _interval0_60.toString();
		String str_0_100 = _interval0_100.toString();
		print_message_xy(("satts: " + String(_satts)).c_str(), 1, 1);
		print_message_xy(str_speed.c_str(), 1, 3);
		print_message_xy(str_0_60.c_str(), 1, 4);
		print_message_xy(str_0_100.c_str(), 1, 5, true);
	}
#ifdef DEBUG
	print_debug();
#endif // DEBUG
}
