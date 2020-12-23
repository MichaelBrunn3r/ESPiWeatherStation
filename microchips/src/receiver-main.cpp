#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include <common.h>
// #define LOGGING
#include <log_utils.h>

struct __attribute__((packed)) dataPacket {
  int sensor1;
  int sensor2;
  float sensor3;
};

void onDataReceived(uint8_t* senderMac, uint8_t* data, uint8_t dataLen) {
	char macStr[18];
	MeasurementPacket packet;

	snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", senderMac[0], senderMac[1], senderMac[2], senderMac[3], senderMac[4], senderMac[5]);

	LOGF("Received data from: %s", macStr);
	memcpy(&packet, data, sizeof(packet));

	Serial.printf("{Temp: %f, Hum: %f, Press: %f}\n", packet.temp, packet.hum, packet.press);
}

void initESPNOW() {
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();

	if(esp_now_init() != 0) {
		LOG("ESP-NOW initialization failed");
    	return;
	}

	esp_now_register_recv_cb(onDataReceived);
	LOG("Initialized");
}

void setup() {
	Serial.begin(SERIAL_BAUD);
	initESPNOW();
}

void loop() {}