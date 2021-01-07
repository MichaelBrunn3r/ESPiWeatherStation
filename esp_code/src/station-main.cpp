#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <espnow.h>
#define ESP_NOW_SEND_SUCCESSFULL 0
#define ESP_NOW_SEND_FAILED 1

#include <common.h>
#include <secrets.h>
// #define LOGGING
#include <log_utils.h>

// Individual sensor adjustments
#define BAT_MAX_V 		4.15f									// Maximum Battery Voltage
#define BME280_TEMP_COMPENSATION -1

// Settings
#define SLEEP_DURATION     300e6
#define BME280_IC2_ADDR	0x76									// The IC2 Address of the BME280 sensor
#define BAT_ADC_RES     1024 									// Resolution of the Analog pin reading the battery level
#define BAT_PIN			A0

// Utils
#define CELSIUS_TO_KELVIN(c) c+273.15
#define Pa_to_hPa(p) p/100.0
#define SEALEVEL_PRESSURE 1013.25

Adafruit_BME280 bme;
uint8_t receiverAddress[] = RECEIVER_MAC;   // update this with the MAC address of the receiver

/**
 * Enter deep sleep mode and wake up after some duration.
 * To successfully wake up, RST and D0 pins must be connected.
 *
 * @param duration_us Sleep duration in microseconds
 **/
void deepSleep(uint64_t duration_us) {
  LOGF("Deep sleep for %f seconds.", duration_us/1000000.0);

  // Put BME280 Sensor into sleep mode
  bme.setSampling(Adafruit_BME280::MODE_SLEEP);

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
  LOG(millis());
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

  // Adjust temperature reading. Helps for better humidity and pressure readings.
  bme.setTemperatureCompensation(BME280_TEMP_COMPENSATION);
  // Set the BME280 into Forced mode (@see datasheet chapter 3.5 'Recommended modes of operation').
  bme.setSampling(Adafruit_BME280::MODE_FORCED, Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::SAMPLING_X1, Adafruit_BME280::FILTER_OFF);

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
float readBatteryVoltage() {
  float measurement = (float)analogRead(BAT_PIN);
  float voltage = measurement/(BAT_ADC_RES) * BAT_MAX_V;
  return voltage;
}

/**
 * Send a weather measurement to the current ESP-Now peer
 **/
void sendMeasurement() {
  // Let the BME280 take one measurement.
  bme.takeForcedMeasurement();

  // Create measurement packet
  MeasurementPacket mp;
  mp.temp = bme.readTemperature();
  mp.hum = bme.readHumidity();
  mp.press = Pa_to_hPa(bme.readPressure());
  mp.batv = readBatteryVoltage();

  // Send measurement packet
  esp_now_send(receiverAddress, (uint8_t *) &mp, sizeof(mp));
}

void setup() {
  #ifdef LOGGING
  Serial.begin(SERIAL_BAUD);
  #endif

  pinMode(BAT_PIN, INPUT);

  if(initESPNOW() && initBME()) {
    sendMeasurement();
  } else {
    delay(10);
    deepSleep(SLEEP_DURATION);
  }
}

void loop() {}