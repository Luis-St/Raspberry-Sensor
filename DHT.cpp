#include "DHT.hpp"

DHT::DHT() {
	wiringPiSetup();
}

int DHT::readSensor(const int pin, int wakeupDelay) {
	int mask = 0x80;
	int index = 0;
	int i;
	for (i = 0; i < 5; i++) {
		bits[i] = 0;
	}
	pinMode(pin, OUTPUT);
	digitalWrite(pin,HIGH);
	delay(500);
	digitalWrite(pin, LOW);
	delay(wakeupDelay);
	digitalWrite(pin, HIGH);
	pinMode(pin,INPUT);

	int32_t loopCnt = DHTLIB_TIMEOUT;
	int32_t t = micros();
	while (true) {
		if (digitalRead(pin) == LOW) {
			break;
		}
		if ((micros() - t) > loopCnt) {
			return DHTLIB_ERROR_TIMEOUT;
		}
	}

	loopCnt = DHTLIB_TIMEOUT;
	t = micros();
	while (digitalRead(pin) == LOW) {
		if ((micros() - t) > loopCnt) {
			return DHTLIB_ERROR_TIMEOUT;
		}
	}

	loopCnt = DHTLIB_TIMEOUT;
	t = micros();
	while (digitalRead(pin) == HIGH) {
		if ((micros() - t) > loopCnt) {
			return DHTLIB_ERROR_TIMEOUT;
		}
	}
	for (i = 0; i < 40; i++) {
		loopCnt = DHTLIB_TIMEOUT;
		t = micros();
		while (digitalRead(pin) == LOW) {
			if ((micros() - t) > loopCnt)
				return DHTLIB_ERROR_TIMEOUT;
		}
		t = micros();
		loopCnt = DHTLIB_TIMEOUT;
		while (digitalRead(pin) == HIGH) {
			if ((micros() - t) > loopCnt) {
				return DHTLIB_ERROR_TIMEOUT;
			}
		}
		if ((micros() - t) > 60) {
			bits[index] |= mask;
		}
		mask >>= 1;
		if (mask == 0) {
			mask = 0x80;
			index++;
		}
	}
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	return DHTLIB_OK;
}

int DHT::readDHT11Once(const int pin) {
	if (const int rv = readSensor(pin, DHTLIB_DHT11_WAKEUP); rv != DHTLIB_OK) {
		humidity = DHTLIB_INVALID_VALUE;
		temperature = DHTLIB_INVALID_VALUE;
		return rv;
	}
	humidity = bits[0];
	temperature = bits[2] + bits[3] * 0.1;
	if (const uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3]; bits[4] != sum) {
		return DHTLIB_ERROR_CHECKSUM;
	}
	return DHTLIB_OK;
}

int DHT::readDHT11(const int pin) {
	int chk = DHTLIB_INVALID_VALUE;
	for (int i = 0; i < 15; i++) {
		chk = readDHT11Once(pin);
		if (chk == DHTLIB_OK) {
			return DHTLIB_OK;
		}
		delay(100);
	}
	return chk;
}
