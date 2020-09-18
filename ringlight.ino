#include <Adafruit_NeoPixel.h>

// PINS
#define PIN_BRIGHT A1
#define PIN_TEMP A2
#define PIN_LEDS 8
#define NUM_PIXELS 12
#define DELAY 100

// GLOBALS
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_LEDS, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear(); 
  
  int brightness = map(analogRead(PIN_BRIGHT), 0, 1023, 0, 255);
  pixels.setBrightness(brightness);

  long t_pot = analogRead(PIN_TEMP);
  unsigned long t = map(t_pot, 0L, 1023L, 1000L, 40000L);
  int r, g, b;
  tempToRGB(t, r, g, b);
  for(int i = 0; i< NUM_PIXELS; i++) {
    pixels.setPixelColor(i, 255, 255, 255);
  }
  pixels.setPixelColor(4, 255, 255, 255);
  pixels.show(); 

  debug(r, g, b, brightness, t);
  
  delay(DELAY); // Pause before next pass through loop
}

// Based on:
// https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html 
void tempToRGB(int t, int &r, int &g, int &b) {
  double kelvin = t;
  double calc;

  if (kelvin < 1000) kelvin = 1000;
  if (kelvin > 40000) kelvin = 40000;

  kelvin = kelvin / 100;

  // red
  if (kelvin <= 66) {
    r = 255;
  } else {
    calc = kelvin - 60;
    calc = 329.698727446 * pow(calc, -0.1332047592);
    r = calc;
    if (r < 0) r = 0;
    if (r > 255) r = 255;
  }

  // green
  if (kelvin <= 66) {
    g = 99.4708025861 * log(kelvin) - 161.1195681661;
    if (g < 0) g = 0;
    if (g > 255) g = 255;
  } else {
    calc = kelvin - 60;
    g = 288.1221695283 * pow(calc, -0.0755148492);
    if (g < 0) g = 0;
    if (g > 255) g = 255;
  }

  // blue
  if (kelvin >= 66) {
    b = 255;
  } else if (kelvin <= 19) {
    b = 0;
  } else {
    calc = kelvin - 10;
    b = 138.5177312231 * log(calc) - 305.0447927307;
    if (b < 0) b = 0;
    if (b > 255) b = 255;
  }
}

void debug(int r, int g, int b, int bright, int temp) {
  Serial.print("RGB: (");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.print(b);
  Serial.print("), Brightness: ");
  Serial.print(bright);
  Serial.print(", Temp: ");
  Serial.println(temp);
}
