#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <AM2320.h>

#define FIREBASE_HOST "https://i-hetaste-laget-40387-default-rtdb.europe-west1.firebasedatabase.app/"
#define FIREBASE_AUTH "AIzaSyA6A71DIwrLphGxwz3KI8W6aIOKiWYgdLw"

#define WIFI_SSID "TN-UI4721-2.4"
#define WIFI_PASSWORD "SipduofUdor5"

AM2320 sensor;

float SensorTemp;
float SensorHum;

FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);
  Wire.begin(14, 12);
  delay(5000);

  // WiFi-anslutning
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi!");

  // Initiera Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true); // Säkerställer återanslutning vid Wi-Fi-avbrott
}

void getTempHum() {
  if (sensor.measure()) {
    SensorTemp = sensor.getTemperature();
    SensorHum = sensor.getHumidity();

    Serial.print("Temperature: ");
    Serial.println(SensorTemp);
    Serial.print("Humidity: ");
    Serial.println(SensorHum);

    if (SensorTemp > 27) {
      Serial.println("Dålig andedräkt!!!!");
    }
  } else {
    int errorCode = sensor.getErrorCode();
    switch (errorCode) {
      case 1: Serial.println("ERR: Sensor is offline"); break;
      case 2: Serial.println("ERR: CRC validation failed."); break;
    }
  }
}

void sendToFirebase(float temp, float hum) {
  // Skicka temperatur till Firebase
  if (Firebase.setFloat(firebaseData, "/sensor/temperature", temp)) {
    Serial.println("Temperature sent to Firebase");
  } else {
    Serial.print("Failed to send temperature: ");
    Serial.println(firebaseData.errorReason());
  }

  // Skicka luftfuktighet till Firebase
  if (Firebase.setFloat(firebaseData, "/sensor/humidity", hum)) {
    Serial.println("Humidity sent to Firebase");
  } else {
    Serial.print("Failed to send humidity: ");
    Serial.println(firebaseData.errorReason());
  }
}

void loop() {
  getTempHum();
  sendToFirebase(SensorTemp, SensorHum);
  delay(20000); 
}
