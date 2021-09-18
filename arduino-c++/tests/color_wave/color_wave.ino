#include <Adafruit_NeoPixel.h>

const uint8_t PIN_ONBOARD_LED = 5;

const uint8_t PIN_PIXEL       = 4;
const uint8_t PIXELS_N        = 16;
const int     BRIGHTNESS      = 16; // 0-255
const int     STEP            = BRIGHTNESS / 4;

const int     LOOP_DELAY      = 100;  // ms

class NeoPixel {
public:
  NeoPixel(uint8_t n, uint8_t pin) {
    this->_pixels = Adafruit_NeoPixel(n, pin, NEO_GRB + NEO_KHZ800);
  }
  
private:
  Adafruit_NeoPixel _pixels;
  uint8_t pixels_n = 0;
  uint8_t pin;
};

Adafruit_NeoPixel pixels(PIXELS_N, PIN_PIXEL, NEO_GRB + NEO_KHZ800);

uint32_t next_color(uint32_t col, int step=1) {
  uint32_t rgb[3];
  int j;
  
  rgb[0] = col >> 16 & 0xff;
  rgb[1] = col >> 8 & 0xff;
  rgb[2] = col & 0xff;

  for (int i=0; i < 3; i++) {
    int j1 = (i - 1 + 3) % 3;
    int j2 = (i + 1) % 3;
    
    if (rgb[i] != BRIGHTNESS) {
      continue;
    }

    if (rgb[j2] == BRIGHTNESS) {
      continue;
    }

    if (rgb[j1] != 0) {
      rgb[j1] -= step;
      if (rgb[j1] > BRIGHTNESS) {
        rgb[j1] = 0;
      }
      break;
    } else {
      rgb[j2] += step;
      if (rgb[j2] > BRIGHTNESS) {
        rgb[j2] = BRIGHTNESS;
      }
      break;
    }
  } // for(i)

  col = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
  return col;
}

void setup() {
  //Serial.begin(115200);

  pixels.begin();
  pixels.clear();

  pinMode(PIN_ONBOARD_LED, OUTPUT);
  digitalWrite(PIN_ONBOARD_LED, HIGH);

  //pixels.setPixelColor(0, pixels.Color(BRIGHTNESS, 0, 0));
  pixels.setPixelColor(PIXELS_N - 1,
                       pixels.Color(BRIGHTNESS, 0, 0));
  pixels.show();

  delay(2000);
}

void loop() {
  digitalWrite(PIN_ONBOARD_LED, LOW);

  //uint32_t col = pixels.getPixelColor(0);
  uint32_t col = pixels.getPixelColor(PIXELS_N - 1);

  //for (int i=0; i < PIXELS_N; i++) {
  for (int i=PIXELS_N - 1; i >= 0; i--) {
    col = next_color(col, STEP);
    //Serial.printf("%06X ", col);
    pixels.setPixelColor(i, col);
  }
  pixels.show();
  //Serial.printf("\n");

  delay(LOOP_DELAY/2);
  digitalWrite(PIN_ONBOARD_LED, HIGH);
  delay(LOOP_DELAY/2);
}
