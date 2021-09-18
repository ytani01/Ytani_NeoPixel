#include <Adafruit_NeoPixel.h>

const uint8_t PIN_ONBOARD_LED = 1;

const uint8_t PIN_PIXEL       = 4;
const uint8_t PIXELS_N        = 16;
const int     BRIGHTNESS      = 32; // 0-255
const int     STEP            = BRIGHTNESS / 4;

const int     LOOP_DELAY      = 100;  // ms

const uint8_t COLOR_LIST[][3] =
  {
   {BRIGHTNESS, 0, 0},
   {BRIGHTNESS, int(1*BRIGHTNESS/6), 0},
   {BRIGHTNESS, BRIGHTNESS, 0},
   {int(3*BRIGHTNESS/8), BRIGHTNESS, 0},
   {0, BRIGHTNESS, 0},
   {0, BRIGHTNESS, int(1*BRIGHTNESS/8)},
   {0, BRIGHTNESS, BRIGHTNESS},
   {0, int(1*BRIGHTNESS/4), BRIGHTNESS},
   {0, 0, BRIGHTNESS},
   {int(3*BRIGHTNESS/12), 0, BRIGHTNESS},
   {BRIGHTNESS, 0, BRIGHTNESS},
   {BRIGHTNESS, 0, int(1*BRIGHTNESS/8)}
  };

const int COLOR_N = 12;

/**
 *
 */
class NeoPixel {
public:
  NeoPixel(uint8_t n, uint8_t pin) {
    this->_pixels = Adafruit_NeoPixel(n, pin, NEO_GRB + NEO_KHZ800);
    this->_pixels.begin();
    this->_pixels.setBrightness(255);
  }

  void clear() {
    this->_pixels.clear();
  }

  void show() {
    this->_pixels.show();
  }

  void setColor(int i, uint32_t col) {
    this->_pixels.setPixelColor(i, col);
  }

  uint32_t getColor(int i) {
    return this->_pixels.getPixelColor(i);
  }

  uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
    return this->_pixels.Color(r, g, b);
  }

private:
  Adafruit_NeoPixel _pixels;
  uint8_t pixels_n = 0;
  uint8_t pin;
};

NeoPixel pixels(PIXELS_N, PIN_PIXEL);

int color_i;

/**
 *
 */
void setup() {
  uint32_t col;

  pinMode(PIN_ONBOARD_LED, OUTPUT);
  digitalWrite(PIN_ONBOARD_LED, HIGH);

  pixels.clear();

  for (int i=0; i < PIXELS_N; i++) {
    col = pixels.color(COLOR_LIST[i % COLOR_N][0],
                       COLOR_LIST[i % COLOR_N][1],
                       COLOR_LIST[i % COLOR_N][2]);
    pixels.setColor(i, col);
    pixels.show();
    delay(LOOP_DELAY);
  }

  //delay(60000);
  color_i = 0;
}

/**
 *
 */
void loop() {
  digitalWrite(PIN_ONBOARD_LED, LOW);

  int color_i2 = color_i;
  
  for (int i=0; i < PIXELS_N; i++) {
    uint32_t col = pixels.color(COLOR_LIST[color_i2][0],
                                COLOR_LIST[color_i2][1],
                                COLOR_LIST[color_i2][2]);
    pixels.setColor(i, col);
    color_i2 = (color_i2 + 1) % COLOR_N;
  }
  pixels.show();

  color_i = (color_i + COLOR_N - 1) % COLOR_N;

  delay(LOOP_DELAY / 2);
  digitalWrite(PIN_ONBOARD_LED, HIGH);
  delay(LOOP_DELAY / 2);
}
