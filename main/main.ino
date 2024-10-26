#include <arduino.h>
#include <Adafruit_NeoPixel.h>

//pins
const uint8_t start_button_pin = 2;
const uint8_t led_strip_pin = 3;
const uint8_t reed_switch_pins[8][8] = {0};

//other variables
volatile Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(64, led_strip_pin, NEO_GRB + NEO_KHZ800);
volatile int8_t chess_board[8][8] = {//Initialize chess board
  /*
  Chess board encoding
  Negative: Black
  Positive: White
  0, 1, 2, 3, 4, 5, 6 = Blank, Pawn, Rook, Bishop, Queen, King
  */
  //A. B. C. D. E. F. G. H. 
  { 2, 3, 4, 5, 6, 4, 3, 2,},//1.   White side
  { 1, 1, 1, 1, 1, 1, 1, 1,},//2.   Whites pawns
  { 0, 0, 0, 0, 0, 0, 0, 0,},//3
  { 0, 0, 0, 0, 0, 0, 0, 0,},//4
  { 0, 0, 0, 0, 0, 0, 0, 0,},//5
  { 0, 0, 0, 0, 0, 0, 0, 0,},//6
  {-1,-1,-1,-1,-1,-1,-1,-1,},//7.   Black pawns
  {-2,-3,-4,-5,-6,-4,-3,-2,},//8.   White side
};

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
