#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <BlynkSimpleEsp8266.h>
#include <WidgetRTC.h>
#include <TimeLib.h>

//---------------Settings-------------------------------------------------------
#define LEDS_IN_BOX 24                            /* Number of LEDs in each box/leaf */
#define NUM_BOXES 8                               /*The number of boxes */
#define PIN 6                                     /*The pin the LED is connected to */
#define BRIGHTNESS 50                             /* Default brightness */
char ssid[] = "********";                         /* wifi ssid name */
char pass[] = "********";                         /* wifi pass */
char auth[] = "********";                         /* blynk auth code */
//------------------------------------------------------------------------------

#define BLYNK_PRINT Serial
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define TOTAL_LEDS  LEDS_IN_BOX * NUM_BOXES

//Global variables
CRGB leds[TOTAL_LEDS];
bool leds_enable = true;                //leds on/off
uint8_t effect = 2;                     // default effect
uint8_t transition = 2;                 // default transition effect
uint8_t red, green, blue, led_speed;    //main color
uint8_t red2, green2, blue2;            //secondary color
bool autoLED = false;
bool syncOnChange = false;
int delay_time = 200;
int leaf = 1;
WidgetLCD lcd(V0);

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, TOTAL_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);// global brightness
}

void loop() {
  if (syncOnChange) {
    Blynk.syncAll();
    syncOnChange = false;
  }
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
      two_colors_gradient(red, green, blue, red2, green2, blue2, delay_time);
      two_colors_gradient(red2, green2, blue2, red, green, blue, delay_time);
      break;
      case 5:
      two_colors_gradient2(red, green, blue, red2, green2, blue2, delay_time);
      two_colors_gradient2(red2, green2, blue2, red, green, blue, delay_time);
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

//---------------- Blynk inputs ------------------------------------------------
BLYNK_WRITE(V1) { // on/off
  leds_enable = param.asInt();
  if (leds_enable) {
    Blynk.virtualWrite(8, 0); //remove?
    autoLED = false; //remove?
  }
}

BLYNK_WRITE(V3) { // main color
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
}

BLYNK_WRITE(V7) { // secondary color
  red2 = param[0].asInt();
  green2 = param[1].asInt();
  blue2 = param[2].asInt();
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

BLYNK_WRITE(V9) { // effects menu
  Serial.println(param.asInt());
  effect = param.asInt();
}

BLYNK_WRITE(V4) { // transitions menu
  Serial.println(param.asInt());
  transition = param.asInt();
}

//---------------- Effects -----------------------------------------------------
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
  box(random(0, 9), red, green, blue, transition, wait); // main color
  box(random(0, 9), red2, green2, blue2, transition, wait); // secondary color
}

void two_colors_gradient2(int r, int g, int b, int r2, int g2, int b2, int delay_time){
  transition_left_to_right(48, 71, r, g, b, r2, g2, b2, delay_time);
  transition_left_to_right_2_boxes(24, 47, 72, 95, r, g, b, r2, g2, b2, delay_time);
  transition_left_to_right_2_boxes(0, 23, 96, 119, r, g, b, r2, g2, b2, delay_time);
  transition_left_to_right_2_boxes(168, 191, 120, 143, r, g, b, r2, g2, b2, delay_time);
  transition_left_to_right(144, 167, r, g, b, r2, g2, b2, delay_time);
}

void two_colors_gradient(int r, int g, int b, int r2, int g2, int b2, int delay_time){
  int count = 0;
  int value1 = 164;
  int value2 = 163;

  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    delay(delay_time);

    value1++, value2--;

    if(value1 > 167){
      value1 = 144;
    }
    count++;
  }while(count < 10);

  printf("-----------------\n");
  count = 0;
  int value3 = 188;
  int value4 = 187;
  int value5 = 140;
  int value6 = 139;
  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    leds[value3].setRGB(r, g, b);
    leds[value4].setRGB(r, g, b);
    leds[value5].setRGB(r, g, b);
    leds[value6].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    leds[value3].setRGB(r2, g2, b2);
    leds[value4].setRGB(r2, g2, b2);
    leds[value5].setRGB(r2, g2, b2);
    leds[value6].setRGB(r2, g2, b2);
    delay(delay_time);

    value1++, value2--;
    value3++, value4--;
    value5++, value6--;
    count++;
  }while(count < 2);

  printf("-----------------\n");
  count = 0;
  do{
    leds[value3].setRGB(r, g, b);
    leds[value4].setRGB(r, g, b);
    leds[value5].setRGB(r, g, b);
    leds[value6].setRGB(r, g, b);
    FastLED.show();
    leds[value3].setRGB(r2, g2, b2);
    leds[value4].setRGB(r2, g2, b2);
    leds[value5].setRGB(r2, g2, b2);
    leds[value6].setRGB(r2, g2, b2);
    delay(delay_time);
    value3++, value4--;
    value5++, value6--;

    if(value3 > 191){
      value3 = 168;
    }
    if(value5 > 143){
      value5 = 120;
    }
    count++;
  }while(count < 8);

  printf("-----------------\n");
  count = 0;
  value1 = 20;
  value2 = 19;
  int value7 = 116;
  int value8 = 115;
  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    leds[value3].setRGB(r, g, b);
    leds[value4].setRGB(r, g, b);
    leds[value5].setRGB(r, g, b);
    leds[value6].setRGB(r, g, b);
    leds[value7].setRGB(r, g, b);
    leds[value8].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    leds[value3].setRGB(r2, g2, b2);
    leds[value4].setRGB(r2, g2, b2);
    leds[value5].setRGB(r2, g2, b2);
    leds[value6].setRGB(r2, g2, b2);
    leds[value7].setRGB(r2, g2, b2);
    leds[value8].setRGB(r2, g2, b2);
    delay(delay_time);

    value1++, value2--;
    value3++, value4--;
    value5++, value6--;
    value7++, value8--;
    count++;
  }while(count < 2);

  printf("-----------------\n");
  count = 0;
  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    leds[value7].setRGB(r, g, b);
    leds[value8].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    leds[value7].setRGB(r2, g2, b2);
    leds[value8].setRGB(r2, g2, b2);
    delay(delay_time);
    value1++, value2--;
    value7++, value8--;

    if(value1 > 23){
      value1 = 0;
    }
    if(value7 > 119){
      value7 = 96;
    }
    count++;
  }while(count < 8);

  printf("-----------------\n");
  count = 0;
  value3 = 44;
  value4 = 43;
  value5 = 92;
  value6 = 91;
  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    leds[value3].setRGB(r, g, b);
    leds[value4].setRGB(r, g, b);
    leds[value5].setRGB(r, g, b);
    leds[value6].setRGB(r, g, b);
    leds[value7].setRGB(r, g, b);
    leds[value8].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    leds[value3].setRGB(r2, g2, b2);
    leds[value4].setRGB(r2, g2, b2);
    leds[value5].setRGB(r2, g2, b2);
    leds[value6].setRGB(r2, g2, b2);
    leds[value7].setRGB(r2, g2, b2);
    leds[value8].setRGB(r2, g2, b2);
    delay(delay_time);
    value1++, value2--;
    value3++, value4--;
    value5++, value6--;
    value7++, value8--;
    count++;
  }while(count < 2);

  printf("-----------------\n");
  count = 0;
  do{
    leds[value3].setRGB(r, g, b);
    leds[value4].setRGB(r, g, b);
    leds[value5].setRGB(r, g, b);
    leds[value6].setRGB(r, g, b);
    FastLED.show();
    leds[value3].setRGB(r2, g2, b2);
    leds[value4].setRGB(r2, g2, b2);
    leds[value5].setRGB(r2, g2, b2);
    leds[value6].setRGB(r2, g2, b2);
    delay(delay_time);
    value3++, value4--;
    value5++, value6--;

    if(value3 > 47){
      value3 = 24;
    }
    if(value5 > 95){
      value5 = 72;
    }
    count++;
  }while(count < 8);

  printf("-----------------\n");
  count = 0;
  value1 = 68;
  value2 = 67;
  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    leds[value3].setRGB(r, g, b);
    leds[value4].setRGB(r, g, b);
    leds[value5].setRGB(r, g, b);
    leds[value6].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    leds[value3].setRGB(r2, g2, b2);
    leds[value4].setRGB(r2, g2, b2);
    leds[value5].setRGB(r2, g2, b2);
    leds[value6].setRGB(r2, g2, b2);
    delay(delay_time);

    value1++, value2--;
    value3++, value4--;
    value5++, value6--;
    count++;
  }while(count < 2);

  printf("-----------------\n");
  count = 0;
  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    delay(delay_time);

    value1++, value2--;

    if(value1 > 71){
      value1 = 48;
    }
    count++;
  }while(count < 10);
}

void random_colors(int wait) {
  box(random(0, 9), random(255), random(255), random(255), transition, wait);
}

void each_leaf(int wait) {
  box(leaf, red, green, blue, transition, wait);
}

//********* Transitions ***********************************************

void transition_simple(int i, int j, int r, int g, int b, int delay_time){
  for (i; i <= j ; ++i) {
    leds[i].setRGB(r, g, b);
  }
  FastLED.show();
  delay(delay_time);
}

void transition_circle(int i, int j, int r, int g, int b, int delay_time){
  for (i; i <= j ; ++i) {
    leds[i].setRGB(r, g, b);
    FastLED.show();
    delay(delay_time);
  }
}

void transition_2_sides(int i, int j, int r, int g, int b, int delay_time){
  int count = 1;

  int value1 = i + 7;
  int value2 = i + 8;
  int value3 = i + 19;
  int value4 = i + 20;

  leds[value1].setRGB(r, g, b);
  leds[value2].setRGB(r, g, b);
  leds[value3].setRGB(r, g, b);
  leds[value4].setRGB(r, g, b);
  FastLED.show();
  delay(delay_time);

  do{
    value1--, value2++, value3--, value4++;

    if(value4 > j){
      value4 = i;
    }

    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    leds[value3].setRGB(r, g, b);
    leds[value4].setRGB(r, g, b);
    FastLED.show();
    delay(delay_time);
    count++;
  }while(count < 6);
}

void transition_6_corners(int i, int j, int r, int g, int b, int delay_time){
  int count = 1;

  int value1 = i;
  int value2 = i + 4;
  int value3 = i + 8;
  int value4 = i + 12;
  int value5 = i + 16;
  int value6 = i + 20;

  leds[value1].setRGB(r, g, b);
  leds[value2].setRGB(r, g, b);
  leds[value3].setRGB(r, g, b);
  leds[value4].setRGB(r, g, b);
  leds[value5].setRGB(r, g, b);
  leds[value6].setRGB(r, g, b);
  FastLED.show();
  delay(delay_time);

  do{
    value1++, value2++, value3++, value4++, value5++, value6++;

    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    leds[value3].setRGB(r, g, b);
    leds[value4].setRGB(r, g, b);
    leds[value5].setRGB(r, g, b);
    leds[value6].setRGB(r, g, b);
    FastLED.show();
    delay(delay_time);
    count++;
  }while(count < 4);
}

void transition_left_to_right(int i, int j, int r, int g, int b, int r2, int g2, int b2, int delay_time){
  int count = 0;
  int value1 = i+20;
  int value2 = i+19;

  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    delay(delay_time);

    value1++, value2--;

    if(value1 > j){
      value1 = i;
    }
    count++;
  }while(count < 12);
}

void transition_left_to_right_2_boxes(int i, int j, int i2, int j2, int r, int g, int b, int r2, int g2, int b2, int delay_time){
  int count = 0;
  int value1 = i+20;
  int value2 = i+19;
  int value3 = i2+20;
  int value4 = i2+19;

  do{
    leds[value1].setRGB(r, g, b);
    leds[value2].setRGB(r, g, b);
    FastLED.show();
    leds[value1].setRGB(r2, g2, b2);
    leds[value2].setRGB(r2, g2, b2);
    delay(delay_time);

    value1++, value2--;
    value3++, value4--;

    if(value1 > j){
      value1 = i;
    }
    if(value3 > j2){
      value3 = i2;
    }
    count++;
  }while(count < 12);
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

void box(int nr, int r, int g, int b, uint8_t transition, int delay_time) {
  int i = 0;
  int j = 0;

  switch (nr) {
    case 1:
    i = 0;
    j = 23;
    break;
    case 2:
    i = 24;
    j = 47;
    break;
    case 3:
    i = 48;
    j = 71;
    break;
    case 4:
    i = 72;
    j = 95;
    break;
    case 5:
    i = 96;
    j = 119;
    break;
    case 6:
    i = 120;
    j = 143;
    break;
    case 7:
    i = 144;
    j = 167;
    break;
    case 8:
    i = 168;
    j = 191;
    break;
  }

  switch (transition) {
    case 0:
    break;
    case 1:
    transition_simple(i, j, r, g, b, delay_time);
    break;
    case 2:
    transition_circle(i, j, r, g, b, delay_time);
    break;
    case 3:
    transition_2_sides(i, j, r, g, b, delay_time);
    break;
    case 4:
    transition_6_corners(i, j, r, g, b, delay_time);
    break;
  }

}
