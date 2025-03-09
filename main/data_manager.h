#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

// External references
extern String startDate;
extern int brewDays;

// Save brewing data to file
void saveBrewingData() {
  File file = LittleFS.open("/brewing.txt", "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  
  file.println(startDate);
  file.println(brewDays);
  file.close();
}

// Load brewing data from file
void loadBrewingData() {
  if (LittleFS.exists("/brewing.txt")) {
    File file = LittleFS.open("/brewing.txt", "r");
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }
    
    startDate = file.readStringUntil('\n');
    startDate.trim();
    String daysStr = file.readStringUntil('\n');
    brewDays = daysStr.toInt();
    file.close();
  }
}

#endif
