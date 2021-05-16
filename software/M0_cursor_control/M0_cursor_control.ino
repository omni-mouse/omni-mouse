/*
  Description: Adaptive hands-free computer mouse that can be used by people with quadriplegia. 
  Supports left click, right click, scroll, and zoom on the hardware-level.
  Date: May 2021
  Authors: Naren Yenuganti and Dylan Chow      
*/

#include <Mouse.h>
#include <Keyboard.h>
#include <ClickButton.h>

// Pins
int x = A4;
int y = A3;
int air_sensor = A0;
int joystick_sel_pin = 7;

// Mouse Movement Controls
const int sensitivity = 23; // larger sensitivity --> slower mouse
const unsigned long loop_period = 15; // larger loop_period --> more delay
int y_zero, x_zero;
boolean invert = true;

// Click Flags
int right_click_flag = 0;
int left_click_flag = 0;
int scroll_flag = 0;
int zoom_flag = 0;

// Debug Constants
boolean debug_cursor = false;
boolean debug_air_sensor = false;

// Multi Click Constants
int debounce_time = 20;   // Debounce timer in ms
int multi_click_time = 250;  // Time limit for multi clicks
int long_click_time = 250;  // time until "held-down clicks" register

// Air Sensor Constants
int rest = 361;
int max_sip = 13;
int max_puff = 648;

// Double Click Detection 
ClickButton key(joystick_sel_pin, LOW, CLICKBTN_PULLUP);

void setup() {
  pinMode(x, INPUT);
  pinMode(y, INPUT);
  pinMode(air_sensor, INPUT);
  pinMode(joystick_sel_pin, INPUT_PULLUP);
  delay(1000);
  
  Mouse.begin();
  key.debounceTime = debounce_time;
  key.multiclickTime = multi_click_time;
  key.longClickTime = long_click_time;
  
  y_zero = analogRead(y);
  x_zero = analogRead(x);
}

void loop() {
  static unsigned long lastLoop;

  int x_reading = analogRead(x);
  int y_reading = analogRead(y);
  int air_reading = analogRead(air_sensor);

  int y_val = (y_reading - y_zero) / sensitivity;
  int x_val = (x_reading - x_zero) / sensitivity;

  if (invert) {
    x_val *= -1;
    y_val *= -1;
  }

  if (debug_cursor) {
    Serial.print("y_zero :");
    Serial.println(y_zero);
    Serial.print("x_zero :");
    Serial.println(x_zero);

    Serial.print("y analog :");
    Serial.println(y_reading);
    Serial.print("x analog :");
    Serial.println(x_reading);

    Serial.print("sensitivity :");
    Serial.println(sensitivity);

    Serial.print("x value: ");
    Serial.println(x_val);
    Serial.print("y value: ");
    Serial.println(y_val);
  }

  if (debug_air_sensor) {
    Serial.println(air_reading);
  }

  // Move Mouse
  if (!scroll_flag && !zoom_flag) {
    if (y_val > 1 || y_val < -1) {
      Mouse.move(0, y_val, 0);  // move mouse on y axis
    }
    if (x_val > 1 || x_val < -1) {
      Mouse.move(x_val, 0, 0);  // move mouse on x axis
    }
    while (millis() - lastLoop < loop_period) {
    // delay
    }
    lastLoop = millis(); 
  }

  key.Update();
  int scroll_click_count = key.clicks;
  if (scroll_click_count != 0) {
    Serial.println(scroll_click_count);
  }

  // Scroll Mode
  if ((scroll_click_count == -1 || scroll_flag) && key.depressed == true) {
    scroll_flag = 1;
    if (y_val <= -5) {
      Mouse.move(0, 0, -1);
      delay(100);
    } else if (y_val >= 5) {
      Mouse.move(0, 0, 1);
      delay(100);
    }
  } else {
    scroll_flag = 0;
  }

  // Zoom Mode
  if ((scroll_click_count == -2 || zoom_flag) && key.depressed == true) {

    zoom_flag = 1;
    if (y_val <= -5) {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('='); 
      Keyboard.releaseAll();
      delay(500);
    } else if ( y_val >= 5) {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('-');
      Keyboard.releaseAll();
      delay(500);
    }
  } else {
    zoom_flag = 0;
  }
  Serial.println(rest - air_reading);


  // Detect Left and Right Clicks
  if (air_reading - rest < -100 && !left_click_flag) {
    left_click_flag = 1;
    Mouse.press(MOUSE_LEFT);
  } else if (air_reading - rest > -100 && air_reading - rest < 100 && left_click_flag) {
    left_click_flag = 0;
    Mouse.release(MOUSE_LEFT);
  }
  if (air_reading - rest > 100 && !right_click_flag) {
    right_click_flag = 1;
    Mouse.press(MOUSE_RIGHT);
  } else if (air_reading - rest > -100 && air_reading - rest < 100 && right_click_flag) {
    right_click_flag = 0;
    Mouse.release(MOUSE_RIGHT);
  }

  // TODO: Detect Palette Command
}
