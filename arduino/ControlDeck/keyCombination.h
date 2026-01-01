// Desktop management functions

// Move to next desktop
void switchDesktopRight() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_RIGHT);
  delay(50);
  BootKeyboard.releaseAll();
}

// Move to previous desktop
void switchDesktopLeft() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_LEFT);
  delay(50);
  BootKeyboard.releaseAll();
}

// Add a desktop
void createNewDesktop() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_D);
  delay(50);
  BootKeyboard.releaseAll();
}

// Minimalize all windows
void goToDesktop() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_D);
  delay(50);
  BootKeyboard.releaseAll();
}

// Discord mute button (Custom)
void winCtrlAltF5() {
  BootKeyboard.press(KEY_LEFT_GUI);
  BootKeyboard.press(KEY_LEFT_CTRL);
  BootKeyboard.press(KEY_LEFT_ALT);
  BootKeyboard.press(KEY_F5);
  delay(50);
  BootKeyboard.releaseAll();
}