#include <HID-Project.h>
#include <HID-Settings.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===================== Pin Definitions =====================
#define CLK 5
#define DT 4
#define SW 6
#define BTN_UP 10
#define BTN_DOWN 16
#define BTN_MODE 14
#define BTN_SCR_MODE 15

// ===================== OLED Config =====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===================== Global Variables =====================
int currentMode = 1;
const int maxMode = 4;
unsigned long lastButtonPress = 0;

// ===================== Function Prototypes =====================
void updateDisplay();
String modeToString(int mode);
void rotateLeft();
void rotateRight();
void pressButton();
void switchDesktopRight();
void switchDesktopLeft();
void createNewDesktop();
void goToDesktop();
void winCtrlAltF5();

// ===================== Setup =====================
void setup() {
  // Input configuration
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_SCR_MODE, INPUT_PULLUP);

  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;); // halt if display fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  updateDisplay();

  // HID initialization
  Consumer.begin();
  Keyboard.begin();
  BootMouse.begin();
  BootKeyboard.begin();
}

// ===================== Main Loop =====================
void loop() {
  static unsigned long lastUp = 0, lastDown = 0, lastScrMode = 0;
  const unsigned long debounceTime = 200;
  unsigned long currentTime = millis();

  // Encoder rotation
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

  // BTN_UP – previous mode
  if (!digitalRead(BTN_UP) && (currentTime - lastUp > debounceTime)) {
    currentMode = (currentMode == 1) ? maxMode : currentMode - 1;
    updateDisplay();
    lastUp = currentTime;
  }

  // BTN_DOWN – next mode
  if (!digitalRead(BTN_DOWN) && (currentTime - lastDown > debounceTime)) {
    currentMode = (currentMode % maxMode) + 1;
    updateDisplay();
    lastDown = currentTime;
  }

  // Encoder push button
  if (digitalRead(SW) == LOW && (currentTime - lastButtonPress > debounceTime)) {
    pressButton();
    lastButtonPress = currentTime;
  }

  // BTN_SCR_MODE – trigger Win+Ctrl+Alt+F5
  if (!digitalRead(BTN_SCR_MODE) && (currentTime - lastScrMode > debounceTime)) {
    winCtrlAltF5();

    // Feedback on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Hotkey Triggered:");
    display.println("Win + Ctrl + Alt + F5");
    display.display();
    delay(400);
    updateDisplay();

    lastScrMode = currentTime;
  }

  delay(1);
}

// ===================== Display =====================
void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("CURRENT MODE:");
  display.println(modeToString(currentMode));
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

// ===================== Encoder Actions =====================
void rotateRight() {
  switch (currentMode) {
    case 1:
      Consumer.write(MEDIA_VOLUME_UP);
      break;
    case 2:
      Consumer.write(MEDIA_NEXT);
      break;
    case 3:
      BootMouse.move(0, 0, -1); // scroll down
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
      BootMouse.move(0, 0, 1); // scroll up
      break;
    case 4:
      switchDesktopLeft();
      break;
  }
}

// ===================== Button Actions =====================
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

// ===================== Desktop Management =====================
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

// ===================== Hotkey Function =====================
void winCtrlAltF5() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_LEFT_ALT);
  BootKeyboard.press(KEY_F5);
  delay(50);
  BootKeyboard.releaseAll();
}
