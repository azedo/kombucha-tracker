#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "config.h"

// Global variable to store light level
int lightLevel = 0;     // 0-100 percent
unsigned long lastLightReadTime = 0;
const int lightReadInterval = 5000;  // Read every 5 seconds

void setupLightSensor() {
  // For ESP8266, no specific setup needed for analog input
  Serial.println("TEMT6000 light sensor initialized");
}

void updateLightReading() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastLightReadTime >= lightReadInterval) {
    lastLightReadTime = currentMillis;
    
    // Read the analog value (0-1023)
    int rawValue = analogRead(LIGHT_SENSOR_PIN);
    
    // Convert to percentage (0-100%)
    lightLevel = map(rawValue, 0, 1023, 0, 100);
    
    Serial.print("Light level: ");
    Serial.print(lightLevel);
    Serial.println("%");
  }
}

String getLightStatusText() {
  if (lightLevel < LIGHT_LOW_THRESHOLD) {
    return "Good (low light)";
  } else if (lightLevel < LIGHT_HIGH_THRESHOLD) {
    return "Moderate (consider reducing)";
  } else {
    return "Too bright (protect your kombucha)";
  }
}

String getLightStatusColor() {
  if (lightLevel < LIGHT_LOW_THRESHOLD) {
    return "green";
  } else if (lightLevel < LIGHT_HIGH_THRESHOLD) {
    return "orange";
  } else {
    return "red";
  }
}

#endif