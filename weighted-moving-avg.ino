#include <WiFi.h>
int RSSI;

// Replace with your network credentials (STATION)
const char* ssid = "xxx_SSID_xxx";
const char* password = "***_PASSWORD_***";

const int RSSI_REF = -50; // Example RSSI at 1 meter
const float PATH_LOSS_EXPONENT = 2.0;

const float ALPHA = 0.3; 

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Function to calculate distance based on RSSI
float calculateDistance(int rssi) {
  float distance = pow(10, (RSSI_REF - rssi) / (10 * PATH_LOSS_EXPONENT));
  return distance;
}

// Function to calculate weighted moving average
float calculateWeightedAverageRSSI(float currentRSSI, float previousRSSI, float alpha) {
  return (alpha * currentRSSI) + ((1 - alpha) * previousRSSI);
}

void setup() {
  Serial.begin(115200);
  initWiFi();
}

void loop() {
  // Current RSSI reading
  int currentRSSI = WiFi.RSSI();
  
  // Apply Weighted Moving Average (WMA) to smooth RSSI
  static float smoothedRSSI = currentRSSI; // Initialize with the first RSSI value
  smoothedRSSI = calculateWeightedAverageRSSI(currentRSSI, smoothedRSSI, ALPHA);

  // Calculate distance using smoothed RSSI
  float distance = calculateDistance(smoothedRSSI);

  // Print smoothed RSSI and estimated distance
  Serial.print("Current RSSI: ");
  Serial.println(currentRSSI);
  Serial.print("Smoothed RSSI: ");
  Serial.println(smoothedRSSI);
  Serial.print("Estimated Distance (meters): ");
  Serial.println(distance);

  delay(2000); // Delay to update RSSI and distance every 2 seconds
}