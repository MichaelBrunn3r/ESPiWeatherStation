#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <common.h>
#define LOGGING
#include <log_utils.h>

Adafruit_BME280 bme;
uint8_t receiverAddress[] = RECEIVER_MAC;   // please update this with the MAC address of the receiver

void deepSleep(uint64_t duration) {
  LOGF("Deep sleep for %f seconds.", duration/1000000.0);

  // Turn off WiFi
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin(duration+1);
  delay(1);

  // Sleep
  ESP.deepSleep(duration);
}

void onTransmissionComplete(uint8_t *receiver_mac, uint8_t transmissionStatus) {
  if(transmissionStatus == 0) {
	  LOG("Data sent successfully");
  } else {
    LOGF("Couldn't send data. Error Code: %d\n", transmissionStatus);
  }
  delay(10);
  deepSleep(SLEEP_DURATION);
}

bool init_bme() {
  bool status;
  status = bme.begin(BME280_IC2_ADDR);
  if(!status) {
    LOG("Could not find BME280!!");
    return false;
  }
  return true;
}

void initESPNOW() {
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();        // we do not want to connect to a WiFi network

	if(esp_now_init() != 0) {
		LOG("ESP-NOW initialization failed");
		return;
	}

	esp_now_set_self_role(STATION_ROLE);
	esp_now_register_send_cb(onTransmissionComplete);   // this function will get called once all data is sent
	esp_now_add_peer(receiverAddress, RECEIVER_ROLE, WIFI_CHANNEL, NULL, 0);

	LOG("Initialized.");
}

double readBatteryVoltage() {
  float measurement = (float)analogRead(A0);
  float voltage = measurement/(BAT_ADC_RES) * BAT_MAX_V;
  return voltage;
}

void sendMeasurement() {
  MeasurementPacket mp;
  mp.temp = bme.readTemperature();
  mp.hum = bme.readHumidity();
  mp.press = bme.readPressure() / 100.0F;
  mp.batv = readBatteryVoltage();

  LOG(bme.readTemperature() + BME280_TEMP_OFFSET);

  esp_now_send(receiverAddress, (uint8_t *) &mp, sizeof(mp));
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(BAT_PIN, INPUT);

  initESPNOW();
  if(init_bme()) {
    sendMeasurement();
  } else {
    delay(10);
    deepSleep(SLEEP_DURATION);
  }
}

void loop() {}