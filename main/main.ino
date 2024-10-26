#include <arduino.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(64/*led count*/, 2/*strip pin*/, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Setup has begun.");
  led_strip.begin();
  led_strip.show();
  Serial.println("Setup has ended.");
}

void loop() {
  // put your main code here, to run repeatedly:
}
