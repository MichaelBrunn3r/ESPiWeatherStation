#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <espnow.h>
#define ESP_NOW_SEND_SUCCESSFULL 0
#define ESP_NOW_SEND_FAILED 1

#include <common.h>
// #define LOGGING
#include <log_utils.h>

// Settings
#define SLEEP_DURATION     5e6
#define BME280_IC2_ADDR	0x76									// The IC2 Address of the BME280 sensor
#define BME280_TEMP_OFFSET -3.0
#define BAT_ADC_RES     1024 									// Resolution of the Analog pin reading the battery level
#define BAT_MAX_V 		4.2f									// Maximum Battery Voltage
#define BAT_PIN			A0

Adafruit_BME280 bme;
uint8_t receiverAddress[] = RECEIVER_MAC;   // please update this with the MAC address of the receiver

/**
 * Enter deep sleep mode and wake up after some duration.
 * To successfully wake up, RST and D0 pins must be connected.
 *
 * @param duration_us Sleep duration in microseconds
 **/
void deepSleep(uint64_t duration_us) {
  LOGF("Deep sleep for %f seconds.", duration_us/1000000.0);

  // Turn off WiFi
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin(duration_us+1);
  delay(1);

  // Sleep
  ESP.deepSleep(duration_us);
}

/**
 * This callback is called after the sending of a ESP-Now message.
 *
 * @param receiver_mac MAC address of the device that was the targeted receiver of the message
 * @param transmissionStatus The status of the message (ESP_NOW_SEND_SUCCESSFULL | ESP_NOW_SEND_FAILED)
 **/
void onTransmissionComplete(uint8_t *receiver_mac, uint8_t transmissionStatus) {
  if(transmissionStatus == ESP_NOW_SEND_SUCCESSFULL) {
	  LOG("Data sent successfully.");
  } else {
    LOGF("Couldn't send data. Error Code: %d.\n", transmissionStatus);
  }
  delay(10);
  deepSleep(SLEEP_DURATION);
}

/**
 * Initialise the IC2 connection to the BME280 sensor
 **/
bool initBME() {
  bool status;
  status = bme.begin(BME280_IC2_ADDR);
  if(!status) {
    LOG("Could not find BME280 sensor.");
    return false;
  }
  return true;
}

/**
 * Initialise ESP-Now connection to peer
 **/
bool initESPNOW() {
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();        // we do not want to connect to a WiFi network

	if(esp_now_init() != 0) {
		LOG("Initialisation of ESP-NOW connection failed.");
		return false;
	}

	esp_now_set_self_role(STATION_ROLE);
	esp_now_register_send_cb(onTransmissionComplete);   // this function will get called once all data is sent
	esp_now_add_peer(receiverAddress, RECEIVER_ROLE, WIFI_CHANNEL, NULL, 0);

	LOG("ESP-Now connection initialized.");
  return true;
}

/**
 * Read the current battery voltage
 *
 * @returns The current battery voltage
 **/
double readBatteryVoltage() {
  float measurement = (float)analogRead(BAT_PIN);
  float voltage = measurement/(BAT_ADC_RES) * BAT_MAX_V;
  return voltage;
}

/**
 * Send a weather measurement to the current ESP-Now peer
 **/
void sendMeasurement() {
  // Create measurement packet
  MeasurementPacket mp;
  mp.temp = bme.readTemperature() + BME280_TEMP_OFFSET;
  mp.hum = bme.readHumidity();
  mp.press = bme.readPressure() / 100.0F;
  mp.batv = readBatteryVoltage();

  // Send measurement packet
  esp_now_send(receiverAddress, (uint8_t *) &mp, sizeof(mp));
}

void setup() {
  // Serial.begin(SERIAL_BAUD);
  pinMode(BAT_PIN, INPUT);

  if(initESPNOW() && initBME()) {
    sendMeasurement();
  } else {
    delay(10);
    deepSleep(SLEEP_DURATION);
  }
}

void loop() {}