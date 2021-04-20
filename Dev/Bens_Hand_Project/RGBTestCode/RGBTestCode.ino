#define LED_RED 9
#define LED_GREEN 10
#define LED_BLUE 11


// The Colors are valued such that the bits are
// 0b {RED} {GREEN} {BLUE}
enum LEDColor {
  RED = 0b100,
  GREEN = 0b010,
  BLUE = 0b001,
  YELLOW = 0b110,
  MAGENTA = 0b101,
  CYAN = 0b011,
  WHITE = 0b111,
  OFF = 0b000
};

byte currentColor = OFF;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  setLEDColor((LEDColor) currentColor);
  currentColor++;
  delay(1000);
}

// The LED is common annode meaning a 0 will turn the light on
void setLEDColor(LEDColor col) {
  digitalWrite(LED_RED, !((col & 0x4) >> 2));
  digitalWrite(LED_GREEN, !((col & 0x2) >> 1));
  digitalWrite(LED_BLUE, !((col & 0x1) >> 0));
}
