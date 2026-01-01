#include <HID-Project.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "keyCombination.h"

// Encoder rotary pins
#define CLK 5
#define DT 4
#define SW 6
// Control button pins
#define BTN_UP 10
#define BTN_DOWN 16
#define BTN_NUMBER_SIGN 14
#define BTN_ASTERISK 15
// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

// Global variables
int currentMode = 1;              // Current operating mode (1-4)
const int maxMode = 4;            // Total number of modes
unsigned long lastButtonPress = 0; // For debouncing

// Init display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Function prototypes
void updateDisplay();
String modeToString(int mode);
void rotateERLeft();
void rotateERRight();
void pressERButton();
void switchDesktopRight();
void switchDesktopLeft();
void createNewDesktop();
void goToDesktop();

void setup() {
  // Initialize inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_NUMBER_SIGN, INPUT_PULLUP);
  pinMode(BTN_ASTERISK, INPUT_PULLUP);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;); // Halt if display fails
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  updateDisplay();

  // Initialize HID
  Consumer.begin();
  Keyboard.begin();
  BootMouse.begin();
  BootKeyboard.begin();
}

void loop() {
  // Debounce timing
  const unsigned long debounceTime = 500;
  unsigned long currentTime = millis();
  
  // Handle encoder rotation
  static int lastStateCLK = digitalRead(CLK);
  int currentStateCLK = digitalRead(CLK);

  // Check if encoder rotated
  if (currentStateCLK != lastStateCLK) {
    if (digitalRead(DT) != currentStateCLK) {
      rotateERLeft();  // Counter-clockwise
    } else {
      rotateERRight(); // Clockwise
    }
  }
  lastStateCLK = currentStateCLK;

  // Handle encoder button press
  if (digitalRead(SW) == LOW && (currentTime - lastButtonPress > debounceTime)) {
    pressERButton();
    lastButtonPress = currentTime;
  }

  // Handle UP button - previous mode
  if (!digitalRead(BTN_UP) && (currentTime - lastButtonPress > debounceTime)) {
    currentMode = (currentMode == 1) ? maxMode : currentMode - 1;
    updateDisplay();
    lastButtonPress = currentTime;
  }

  // Handle DOWN button - next mode
  if (!digitalRead(BTN_DOWN) && (currentTime - lastButtonPress > debounceTime)) {
    currentMode = (currentMode % maxMode) + 1;
    updateDisplay();
    lastButtonPress = currentTime;
  }

  // Handle * button - special hotkey
  if (digitalRead(BTN_ASTERISK) == LOW && (currentTime - lastButtonPress > debounceTime)) {
    winCtrlAltF5();
    lastButtonPress = currentTime;
  }
  
  delay(1);
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);

  // Show current mode
  display.println("CURRENT MODE:");
  display.println(modeToString(currentMode));
  display.println("\n---------------------\n");
  display.println("#:Not bounce");
  display.println("*:Win+Ctrl+Alt+F5");
  display.display();
}

String modeToString(int mode) {
  switch (mode) {
    case 1: return "Volume Control";
    case 2: return "Media Control";
    case 3: return "Mouse Scroll";
    case 4: return "Desktop Mgmt";
    default: return "Unknown";
  }
}

void rotateERRight() {
  switch (currentMode) {
    case 1:
      Consumer.write(MEDIA_VOLUME_UP);
      break;
    case 2:
      Consumer.write(MEDIA_NEXT);
      break;
    case 3:
      BootMouse.move(0, 0, -1); // Scroll down
      break;
    case 4:
      switchDesktopRight();
      break;
  }
}

void rotateERLeft() {
  switch (currentMode) {
    case 1:
      Consumer.write(MEDIA_VOLUME_DOWN);
      break;
    case 2:
      Consumer.write(MEDIA_PREVIOUS);
      break;
    case 3:
      BootMouse.move(0, 0, 1); // Scroll up
      break;
    case 4:
      switchDesktopLeft();
      break;
  }
}

void pressERButton() {
  switch (currentMode) {
    case 1:
      Consumer.write(MEDIA_VOLUME_MUTE);
      break;
    case 2:
      Consumer.write(MEDIA_PLAY_PAUSE);
      break;
    case 3:
      goToDesktop();
      break;
    case 4:
      createNewDesktop();
      break;
  }
}