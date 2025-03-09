#ifndef TIME_UTILS_H
#define TIME_UTILS_H

// Calculate days passed since start date
int getDaysPassed(String start) {
  if (start == "") return 0;
  
  int year, month, day;
  sscanf(start.c_str(), "%d-%d-%d", &year, &month, &day);
  
  // Get current time
  time_t now;
  time(&now);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  
  // Create time structure for start date
  struct tm start_tm;
  memset(&start_tm, 0, sizeof(struct tm));
  start_tm.tm_year = year - 1900;
  start_tm.tm_mon = month - 1;
  start_tm.tm_mday = day;
  start_tm.tm_hour = 0;
  start_tm.tm_min = 0;
  start_tm.tm_sec = 0;
  time_t startTime = mktime(&start_tm);
  
  // Calculate difference in days
  double diff = difftime(now, startTime);
  return (int)(diff / 86400.0); // 86400 seconds in a day
}

// Calculate ready date based on start date and brewing days
String getReadyDate(String start, int days) {
  if (start == "") return "";
  
  int year, month, day;
  sscanf(start.c_str(), "%d-%d-%d", &year, &month, &day);
  
  // Create time structure
  struct tm timeinfo;
  memset(&timeinfo, 0, sizeof(struct tm));
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;
  timeinfo.tm_hour = 0;
  timeinfo.tm_min = 0;
  timeinfo.tm_sec = 0;
  
  // Add brewing days
  time_t startTime = mktime(&timeinfo);
  time_t readyTime = startTime + (days * 86400);
  
  // Convert back to string
  struct tm readyTimeinfo;
  localtime_r(&readyTime, &readyTimeinfo);
  
  char buffer[11];
  sprintf(buffer, "%d-%02d-%02d", readyTimeinfo.tm_year + 1900, readyTimeinfo.tm_mon + 1, readyTimeinfo.tm_mday);
  return String(buffer);
}

// Debug function to verify time is working
String getCurrentTimeString() {
  time_t now;
  time(&now);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  
  char buffer[30];
  sprintf(buffer, "%d-%02d-%02d %02d:%02d:%02d", 
          timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
          timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buffer);
}

#endif
