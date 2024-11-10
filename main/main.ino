#include <arduino.h>
#include <Adafruit_NeoPixel.h>

//get pins
extern volatile Adafruit_NeoPixel led_strip;
extern const uint16_t led_positions[8][8];
extern const uint8_t reed_switch_pins[8][8];

//get colors
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

//picked piece
volatile bool piece_up = false;
volatile uint8_t picked_piece_row = 0;
volatile uint8_t picked_piece_column = 0;

//last move
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

      if (!prev_read && cur_read && !piece_up) {
        piece_up = true;
        picked_piece_row = row;
        picked_piece_column = column;
        light_square(row, column, color_blue);

        {
          volatile uint8_t piece_type = abs(chess_board[row][column]);
          volatile bool lifted_is_white = chess_board[row][column] > 0;

          if (piece_type == 1) {} //pawn

          else if (piece_type == 2) {

            for (int8_t test_row = row+1; test_row < 8; test_row++) {
              if (chess_board[test_row][column] == 0) {
                light_square(test_row, column, color_green);
              } else {
                if (is_white(test_row, column) == lifted_is_white) {
                  break;
                }//same color
                light_square(test_row, column, color_red);
                break;//dif color
              }
            }//up

            for (int8_t test_row = row-1; test_row > -1; test_row--) {
              if (chess_board[test_row][column] == 0) {
                light_square(test_row, column, color_green);
              } else {
                if (is_white(test_row, column) == lifted_is_white) {
                  break;
                }//same color
                light_square(test_row, column, color_red);
                break;//dif color
              }
            }//down

            for (int8_t test_column = column+1; test_column < 8; test_column++) {
              if (chess_board[row][test_column] == 0) {
                light_square(row, test_column, color_green);
              } else {
                if (is_white(row, test_column) == lifted_is_white) {
                  break;
                }//same color
                light_square(row, test_column, color_red);
                break;//dif color
              }
            }//right

            for (int8_t test_column = column-1; test_column > -1; test_column--) {
              if (chess_board[row][test_column] == 0) {
                light_square(row, test_column, color_green);
              } else {
                if (is_white(row, test_column) == lifted_is_white) {
                  break;
                }//same color
                light_square(row, test_column, color_red);
                break;//dif color
              }
            }//left

          } //rook

        }//calc and display moves
        return;
      }//piece lifted

      if (prev_read && !cur_read) {

        piece_up = false;

        light_square(last_move_from_row, last_move_from_column, color_black);
        light_square(last_move_to_row, last_move_to_column, color_black);
        light_square(row, column, color_blue);

        last_move_from_row = picked_piece_row;
        last_move_from_column = picked_piece_column;
        last_move_to_row = row;
        last_move_to_column = column;

        chess_board[row][column] = chess_board[picked_piece_row][picked_piece_column];
        chess_board[picked_piece_row][picked_piece_column] = 0;
        return;
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

void light_square(uint8_t row, uint8_t column, uint32_t color) {led_strip.setPixelColor(led_positions[row][column], color);}

bool is_white(uint8_t row, uint8_t column) {return (chess_board[row][column] > 0);}