#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include <common.h>
#include <secrets.h>
// #define LOGGING
#include <log_utils.h>

/**
 * This callback is called when this device receives some data via ESP-Now
 *
 * @param senderMac The MAC address of the device that sent the data
 * @param data An array of bytes that were received
 * @param dataLen The number of bytes that were received
 **/
void onDataReceived(uint8_t* senderMac, uint8_t* data, uint8_t dataLen) {
	char macStr[18];
	MeasurementPacket packet;

	// Convert MAC address to string
	snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", senderMac[0], senderMac[1], senderMac[2], senderMac[3], senderMac[4], senderMac[5]);
	LOGF("Received data from: %s", macStr);

	memcpy(&packet, data, sizeof(packet));

	// Send Json data via Serial to connected device (e.g. RaspberryPi)
	Serial.printf("{\"Mac\":\"%s\",\"Temp\":%f,\"Hum\":%f,\"Press\":%f,\"BatV\":%f}\n", macStr, packet.temp, packet.hum, packet.press, packet.batv);
}

/**
 * Initialise ESP-Now
 **/
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