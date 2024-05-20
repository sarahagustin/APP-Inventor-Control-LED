#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "SAMIA"
#define WIFI_PASSWORD "platistikputih23"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCNBK2mt0uFVyR8-Vpk0oXbQTNPdQvMyAA"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL " https://kelompo-2-iot-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//some importent variables
String sValue, sValue2;
bool signupOK = false;
int led1 = 2; //PIN D5 
int led2 = 16; //PIN D0 
void setup() {
  Serial.begin(115200);
  
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK ) {
    if (Firebase.RTDB.getString(&fbdo, "IoT2/L1")) {
      if (fbdo.dataType() == "string") {
        sValue = fbdo.stringData();
        int a = sValue.toInt();
        Serial.println(a);
        if (a == 1){
          digitalWrite(led1,HIGH);
        }else{
          digitalWrite(led1,LOW);
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    
    if (Firebase.RTDB.getString(&fbdo, "IoT2/L2")) {
      if (fbdo.dataType() == "string") {
        sValue2 = fbdo.stringData();
        int b = sValue2.toInt();
        Serial.println(b);
        if (b == 1){
          digitalWrite(led2,HIGH);
        }else{
          digitalWrite(led2,LOW);
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
  }
}
