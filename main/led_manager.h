#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include "config.h"

// Initialize LED pins
void setupLEDs() {
  pinMode(BREWING_LED_PIN, OUTPUT);
  pinMode(READY_LED_PIN, OUTPUT);
  
  // Initial state: all off
  digitalWrite(BREWING_LED_PIN, LOW);
  digitalWrite(READY_LED_PIN, LOW);
}

// Function to set specific colors
void setRed() {
  digitalWrite(BREWING_LED_PIN, HIGH);   // Red ON
  digitalWrite(READY_LED_PIN, LOW);    // Green OFF
}

void setGreen() {
  digitalWrite(BREWING_LED_PIN, LOW);  // Red OFF
  digitalWrite(READY_LED_PIN, HIGH);     // Green ON
}

void setOff() {
  digitalWrite(BREWING_LED_PIN, LOW);  // Red OFF
  digitalWrite(READY_LED_PIN, LOW);    // Green OFF
}

// Update LEDs based on brewing status
void updateLEDs(bool hasBatch, bool isReady) {
  if (!hasBatch) {
    // No active batch - All LEDs OFF
    setOff();
  } 
  else if (isReady) {
    // Ready - Green
    setGreen();
  } 
  else {
    // Brewing (not ready) - RED
    setRed();
  }
}

#endif
