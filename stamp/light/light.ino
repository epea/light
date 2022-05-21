#include "Arduino.h"
#include <FastLED.h>
#include <WiFi.h>
#include <HTTPClient.h>

// stamp picoの三色LED
#define NUM_LEDS 1 
#define DATA_PIN 27

// LEDテープ用MOSFETのBASE
#define OUT_PIN 25

// 店のwifi(公開してるやつなのでそのままpublicレポジトリへ)
#define SSID "rocher_guest"
#define PASS "sd5ks86vn5nti"

// API
#define SERVER_URL "http://192.168.1.13:8080/?data="

// PICO LED
CRGB leds[NUM_LEDS];

HTTPClient http;

void setup() {
  Serial.begin(115200);
  Serial.println("start");
  delay(10);

  FastLED.addLeds<SK6812, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  pinMode(36,INPUT);
  delay(10);

  ledcSetup(0,12800,8);
  ledcAttachPin(OUT_PIN,0);

  startLight();
  
  WiFi.begin(SSID, PASS);
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    connectingLight();
  }
}

void loop() {
  Serial.println("LOOP Start");
  setLEDColor(CRGB::Black);
  delay(100);
  
  int valueBefore = analogRead(36);
  String getURL = SERVER_URL + String(valueBefore);
  Serial.println(getURL);
  http.begin( getURL );
  int httpCode = http.GET();
  Serial.printf("getResult:%d\n",httpCode);
  if(httpCode != HTTP_CODE_OK){
    setLEDColor(CRGB::Red);
  } else {
      String payload = http.getString();
      setLEDColor(( payload.equals("ON") ) ? CRGB::Green : CRGB::Blue);
      ledcWrite(0,(( payload.equals("ON") ) ? 256 : 0));
      Serial.printf("payload:%s\n",payload);
  }
  http.end();
  delay(1000);
  Serial.println("LOOP Fin");
}

void setLEDColor(int grb){
  leds[0] = grb;
  delay(10);
  FastLED.show();
}

void startLight(){
  for(int i=0;i<5;i++){
    setLEDColor(CRGB::Red);
    delay(100);
    setLEDColor(CRGB::Black);
    delay(100);
  }
}

void connectingLight(){
  for(int i=0;i<5;i++){
    setLEDColor(CRGB::Orange);
    delay(75);
    setLEDColor(CRGB::Black);
    delay(25);
  }
}
