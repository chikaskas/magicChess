//pins
const uint8_t start_button_pin = 2;
const uint8_t led_strip_pin = 3;
const uint32_t led_count = 121;
const uint16_t led_positions[8][8] = {0};
const uint8_t reed_switch_pins[8][8] = {0};

//colors
volatile Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(led_count, led_strip_pin, NEO_GRB + NEO_KHZ800);
const uint32_t color_black = led_strip.Color(0, 0, 0);
const uint32_t color_red = led_strip.Color(255, 0, 0);
const uint32_t color_green = led_strip.Color(0, 255, 0);
const uint32_t color_blue = led_strip.Color(0, 0, 255);
const uint32_t color_white = led_strip.Color(255, 255, 255);