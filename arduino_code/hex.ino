#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <BlynkSimpleEsp8266.h>
#include <WidgetRTC.h>
#include <TimeLib.h>

#define BLYNK_PRINT Serial

#define LEDS_IN_BOX 24 /* Number of LEDs in each box/leaf */
#define NUM_BOXES 8 /*The number of boxes */
#define TOTAL_LEDS  LEDS_IN_BOX * NUM_BOXES
#define PIN 6 /*The pin the LED is connected to */
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 50

CRGB leds[TOTAL_LEDS];

char ssid[] = "ZyXELE5E9F4";
char pass[] = "B959124795FF";
char auth[] = "0034273f9bbe4e91a4072440f7a932ce";

//Global variables
int wifiStatus;
bool leds_enable = true;
bool blynk_connection;
bool change;
uint8_t red, green, blue, led_speed;
uint8_t effect = 2; 
uint8_t red2, green2, blue2;
uint8_t red3, green3, blue3;
bool autoLED = false;
bool autoTime = false;
bool syncOnChange = false;
int delay_time = 200;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int leaf = 1;

WidgetLCD lcd(V0);

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, TOTAL_LEDS ).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);// global brightness
}

void loop() {
  if (syncOnChange) {
    Blynk.syncAll();
    syncOnChange = false;
  }
  change = false;
  Blynk.run();
  if (leds_enable) {
    switch (effect) {
      case 0:
        break;
      case 1:
        leds_off();
        break;
      case 2:
        one_color();
        break;
      case 3:
        two_colors(delay_time);
        break;
      case 4:
        
        break;
      case 5:

        break;
      case 6:
        random_colors(delay_time);
        break;
      case 7:
        each_leaf(delay_time);
        break;   
    }
  }
  else {
    leds_off();
  }
}
//---------------- Blynk inputs ------------------------
BLYNK_WRITE(V1) { // on/off
  Serial.println(param.asInt());
  leds_enable = param.asInt();
  Serial.println(leds_enable);
  change = true;
  if (leds_enable) {
    Blynk.virtualWrite(8, 0);
    autoLED = false;
  }
}

BLYNK_WRITE(V3) { // main color
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
  change = true;
}

BLYNK_WRITE(V7) { // secondary color
  red2 = param[0].asInt();
  green2 = param[1].asInt();
  blue2 = param[2].asInt();
  change = true;
}

BLYNK_WRITE(V4) { // secondary color
  red3 = param[0].asInt();
  green3 = param[1].asInt();
  blue3 = param[2].asInt();
  change = true;
}

BLYNK_WRITE(V2) { // brightness
  FastLED.setBrightness(param.asInt());
}

BLYNK_WRITE(V6) { // delay
  delay_time = param.asInt();
}

BLYNK_WRITE(V5) { // leaf
  leaf = param.asInt();
}

BLYNK_WRITE(V9) { // menu
  Serial.println(param.asInt());
  effect = param.asInt();
  change = true;
}



//---------------- Effects ------------------------
void leds_off() {
  for (int i = 0; i < TOTAL_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void one_color() {
  for (int i = 0; i < TOTAL_LEDS; i++) {
    leds[i].setRGB(red, green, blue);
  }
  FastLED.show();
}


void rainbow(int wait) {

}

void two_colors(int wait) {
  box(random(0,9), red, green, blue); // main color
  delay(wait);
  box(random(0,9), red2, green2, blue2); // secondary color
  delay(wait);
}

void two_colors_gradient(int wait) {

}

void random_colors(int wait) {
  box(random(0,9), random(255), random(255), random(255));
  delay(wait);
}

void each_leaf(int wait) {
  box(leaf, red3, green3, blue3);
  delay(wait);
}

//********* Help method ***********************************************
CRGB wheel(int WheelPos, int dim) {
  CRGB color;
  if (85 > WheelPos) {
    color.r = 0;
    color.g = WheelPos * 3 / dim;
    color.b = (255 - WheelPos * 3) / dim;;
  }
  else if (170 > WheelPos) {
    color.r = WheelPos * 3 / dim;
    color.g = (255 - WheelPos * 3) / dim;
    color.b = 0;
  }
  else {
    color.r = (255 - WheelPos * 3) / dim;
    color.g = 0;
    color.b = WheelPos * 3 / dim;
  }
  return color;
}

void box(int nr, int r, int g, int b){
  int i = 0;
  int j = 0;
  
  if(nr == 1){
    i = 0;
    j = 24;
  }
  else if(nr == 2){
    i = 25;
    j = 48;
  }
  else if(nr == 3){
    i = 49;
    j = 72;
  } 
  else if(nr == 4){
    i = 73;
    j = 96;
  } 
  else if(nr == 5){
    i = 97;
    j = 120;
  } 
  else if(nr == 6){
    i = 121;
    j = 144;
  } 
  else if(nr == 7){
    i = 145;
    j = 168;
  } 
  else if(nr == 8){
    i = 169;
    j = 192;
  } 
 
  for (i; i < j ; ++i) { 
    leds[i].setRGB(r, g, b);
  }
  FastLED.show();
}
