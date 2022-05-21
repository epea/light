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

  FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  pinMode(36,INPUT);
  delay(10);

  ledcSetup(0,12800,8);
  ledcAttachPin(OUT_PIN,0);
  
  WiFi.begin(SSID, PASS);
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  Serial.println("LOOP");

  int valueBefore = analogRead(36);
  String getURL = SERVER_URL + String(valueBefore);
  Serial.println(getURL);
  http.begin( getURL );
  int httpCode = http.GET();

  String payload = http.getString();
  setLEDColor(( payload.equals("ON") ) ? 0xf00000 : 0x00f000);
  ledcWrite(0,(( payload.equals("ON") ) ? 256 : 0));
  Serial.printf("payload:%s\n",payload);
  http.end();
  Serial.printf("getResult:%d\n",httpCode);
  delay(1000);
}

void setLEDColor(int rgb){
  leds[0] = rgb;
  delay(10);
  FastLED.show();
}
