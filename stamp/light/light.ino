/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with STAMP-PICO sample source code
*                          配套  STAMP-PICO 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/stamp_pico
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/stamp_pico
*
* describe: FastLED.
* date：2021/9/19
*******************************************************************************
*/
#include "Arduino.h"
#include <FastLED.h>
#include <WiFi.h>
#include <HTTPClient.h>

// How many leds in your strip?
#define NUM_LEDS 1
#define DATA_PIN 27

#define SSID "rocher_guest"
#define PASS "sd5ks86vn5nti"

#define SERVER_URL "http://192.168.1.13:8080/?data="

// Define the array of leds
CRGB leds[NUM_LEDS];

HTTPClient http;

/* After STAMP-PICO is started or reset
  the program in the setUp () function will be run, and this part will only be run once.
  在 STAMP-PICO 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  Serial.begin(115200);
  Serial.println("start");
  delay(10);

  FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  
  pinMode(36,INPUT);
  delay(10);

  WiFi.begin(SSID, PASS);
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  Serial.println("LOOP");

  int valueBefore = analogRead(36);
  Serial.printf("analogRead:%d\n",valueBefore);

  String hoge = SERVER_URL + String(valueBefore);
  Serial.println(hoge);
  http.begin( hoge );
  int httpCode = http.GET();

  String payload = http.getString();
  setLEDColor(( payload.equals("ON") ) ? 0xf00000 : 0x00f000);
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
