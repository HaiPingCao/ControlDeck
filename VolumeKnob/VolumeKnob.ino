#include <HID-Project.h>
#include <HID-Settings.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pins
#define CLK 5
#define DT 4
#define SW 6
#define BTN_UP 10
#define BTN_DOWN 16
#define BTN_MODE 14
#define BTN_SCR_MODE 15
// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Global variables
int currentMode = 1;
const int maxMode = 4;
unsigned long lastButtonPress = 0;

// Function prototypes
void updateDisplay();
String modeToString(int mode);
void rotateLeft();
void rotateRight();
void pressButton();
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
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_SCR_MODE, INPUT_PULLUP);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    // If display fails to initialize, halt
    for (;;);
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
  // Handle mode buttons with debounce
  static unsigned long lastUp = 0, lastDown = 0;
  const unsigned long debounceTime = 500;
  unsigned long currentTime = millis();
  // Handle Encoder rotation
  static int lastStateCLK = digitalRead(CLK);
  int currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK) {
    if (digitalRead(DT) != currentStateCLK) {
      rotateLeft();
    } else {
      rotateRight();
    }
  }
  lastStateCLK = currentStateCLK;

  // Handle UP button - directly change mode
  if (!digitalRead(BTN_UP) && (currentTime - lastUp > debounceTime)) {
    currentMode = (currentMode == 1) ? maxMode : currentMode - 1;
    updateDisplay();
    lastUp = currentTime;
  }

  // Handle DOWN button - directly change mode
  if (!digitalRead(BTN_DOWN) && (currentTime - lastDown > debounceTime)) {
    currentMode = (currentMode % maxMode) + 1;
    updateDisplay();
    lastDown = currentTime;
  }

  // Handle encoder push button with debounce
  if (digitalRead(SW) == LOW && (currentTime - lastButtonPress > debounceTime)) {
    pressButton();
    lastButtonPress = currentTime;
  }

  if (digitalRead(BTN_SCR_MODE) == LOW && (currentTime - lastButtonPress > debounceTime)) {
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

void rotateRight() {
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

void rotateLeft() {
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

void pressButton() {
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

// Desktop management functions
void switchDesktopRight() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_RIGHT);
  delay(50);
  BootKeyboard.releaseAll();
}

void switchDesktopLeft() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_LEFT);
  delay(50);
  BootKeyboard.releaseAll();
}

void createNewDesktop() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_D);
  delay(50);
  BootKeyboard.releaseAll();
}

void goToDesktop() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_D);
  delay(50);
  BootKeyboard.releaseAll();
}

void winCtrlAltF5() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_LEFT_ALT);
  BootKeyboard.press(KEY_F5);
  delay(50);
  BootKeyboard.releaseAll();
}