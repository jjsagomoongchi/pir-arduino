// firebase global
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"
#define API_KEY "API_KEY"
#define DATABASE_URL "https://DATABASE_NAME.firebaseio.com"
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
float temp = 0;

// pir global
int LED = 2;
int sensor = 7;
int value = 0;

void setup() {
  Serial.begin(115200);

  // firebase setup 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
  
  // pir setup
  pinMode (sensor, INPUT);
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    value = digitalRead(sensor);

    if(value == HIGH) {
      Serial.printf("Set value... %s\n", Firebase.setFloat(fbdo, "/product/human", 1) ? "ok" : fbdo.errorReason().c_str());
    }

    else {
      Serial.printf("Set value... %s\n", Firebase.setFloat(fbdo, "/product/human", 0) ? "ok" : fbdo.errorReason().c_str());
    }
  }
}
