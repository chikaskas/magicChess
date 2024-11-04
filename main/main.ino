#include <arduino.h>
#include <Adafruit_NeoPixel.h>

//pins
extern const uint8_t start_button_pin;
extern const uint8_t led_strip_pin;
extern const uint8_t reed_switch_pins[8][8];

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
  Serial.begin(9600);
  Serial.println("Setup has begun.");

  //setup led strip
  led_strip.begin();
  led_strip.show();

  //setup pins
  loop_through_squares(
    [](uint8_t row, uint8_t column) {
      pinMode(reed_switch_pins[row][column], INPUT_PULLUP);
    }
  );

  Serial.println("Setup has ended.");
}

volatile bool prev_readings[8][8] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
};

void loop() {
  delay(100);

  volatile bool cur_readings[8][8] = {0};

  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t column = 0; column < 8; column++) {

      volatile bool prev_read = digitalRead(reed_switch_pins[row][column]);
      volatile bool cur_read = digitalRead(reed_switch_pins[row][column]);

      if (prev_read && !cur_read) {
        //piece lifted
      }//piece lifted

      if (cur_read && !prev_read) {
        //piece placed
      }//piece placed

    }
  }

}

void loop_through_squares(void (*func)(uint8_t row, uint8_t column)) {
  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t column = 0; column < 8; column++) {
      func(row, column);
    }
  }
}