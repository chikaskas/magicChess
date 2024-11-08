#include <arduino.h>
#include <Adafruit_NeoPixel.h>

//pins
extern const uint8_t start_button_pin;
extern const uint8_t led_strip_pin;
extern const uint32_t led_count;
extern const uint8_t led_positions[8][8];
extern const uint8_t reed_switch_pins[8][8];

//other variables
extern volatile Adafruit_NeoPixel led_strip;
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

//colors
extern const uint32_t color_black;
extern const uint32_t color_red;
extern const uint32_t color_green;
extern const uint32_t color_blue;
extern const uint32_t color_white;

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

volatile bool piece_up = false;
volatile uint8_t picked_piece_row = 0;
volatile uint8_t picked_piece_column = 0;

volatile uint8_t last_move_from_row = 0;
volatile uint8_t last_move_from_column = 0;
volatile uint8_t last_move_to_row = 0;
volatile uint8_t last_move_to_column = 0;

void loop() {
  delay(100);

  volatile bool cur_readings[8][8] = {0};

  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t column = 0; column < 8; column++) {

      volatile bool prev_read = digitalRead(reed_switch_pins[row][column]);
      volatile bool cur_read = digitalRead(reed_switch_pins[row][column]);

      if (prev_read && !cur_read && !piece_up) {
        piece_up = true;
        picked_piece_row = row;
        picked_piece_column = column;
        led_strip.setPixelColor(led_positions[row][column], color_blue);
        {}//calc and display moves
        //piece lifted
      }//piece lifted

      if (cur_read && !prev_read) {

        piece_up = false;

        led_strip.setPixelColor(led_positions[last_move_from_row][last_move_from_column], color_black);
        led_strip.setPixelColor(led_positions[last_move_to_row][last_move_to_column], color_black);
        led_strip.setPixelColor(led_positions[row][column], color_blue);

        last_move_from_row = picked_piece_row;
        last_move_from_column = picked_piece_column;
        last_move_to_row = row;
        last_move_to_column = column;

        //piece placed
      }//piece placed

    }
  }

  led_strip.show();
}

void loop_through_squares(void (*func)(uint8_t row, uint8_t column)) {
  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t column = 0; column < 8; column++) {
      func(row, column);
    }
  }
}