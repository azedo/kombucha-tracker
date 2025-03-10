#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <DHT.h>
#include "config.h"

// Define DHT type
#define DHTTYPE DHT11 // DHT11 sensor

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

float temperature = 0.0;
float humidity = 0.0;
unsigned long lastDHTReadTime = 0;
const long dhtReadInterval = 30000; // Read every 30 seconds

void setupDHT() {
  dht.begin();
  Serial.println("DHT11 sensor initialized");
}

// Read temperature and humidity
void updateDHTReadings() {
  unsigned long currentMillis = millis();
  
  // Only read every 30 seconds (DHT11 is slow)
  if (currentMillis - lastDHTReadTime >= dhtReadInterval) {
    lastDHTReadTime = currentMillis;
    
    // Reading temperature or humidity takes about 250 milliseconds
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    
    // Check if readings failed
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }
}

// Get temperature status message
String getTemperatureStatus() {
  if (temperature < 18) {
    return "Too cold for optimal fermentation";
  } else if (temperature > 30) {
    return "Too warm for optimal fermentation";
  } else {
    return "Optimal temperature range";
  }
}

#endif
