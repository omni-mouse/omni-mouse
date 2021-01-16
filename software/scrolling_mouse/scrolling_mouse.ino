/*
  Description: Sketch for wired tongue operated mouse.
  Date: January 2021
  Author: Naren Yenuganti
*/

#include <Mouse.h>
#include <Keyboard.h>
#include <ClickButton.h>


int horz_pin = A0;
int vert_pin = A1;
int joystick_sel_pin = 7;
int teeth_pin = 8;
int scroll_pin = 9;
int vert_zero, horz_zero;
int y_val, x_val;
int right_click_flag = 0;
int left_click_flag = 0;
int scroll_flag = 0;
int zoom_flag = 0;

// Higher sensitivity value = slower mouse, should be <= about 500
const int sensitivity = 150;

// To invert, to -1
int invert_mouse = 1;

// Set exponential acceleration flag and value of base (greater than 1)
boolean exponential_acceleration = false;
int base = 1.5;

ClickButton key(scroll_pin, LOW, CLICKBTN_PULLUP);

void setup() {
  pinMode(horz_pin, INPUT);
  pinMode(vert_pin, INPUT);
  pinMode(joystick_sel_pin, INPUT);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  digitalWrite(joystick_sel_pin, HIGH);
  delay(1000);

  vert_zero = analogRead(vert_pin);
  horz_zero = analogRead(horz_pin);

  Mouse.begin();
  key.debounceTime   = 20;   // Debounce timer in ms
  key.multiclickTime = 250;  // Time limit for multi clicks
  key.longClickTime  = 250;  // time until "held-down clicks" register
}

void loop() {
  y_val = analogRead(vert_pin) - vert_zero;
  x_val = analogRead(horz_pin) - horz_zero;

  /* debugging
  Serial.print("x value: ");
  Serial.println(x_val);
  Serial.print("y value: ");
  Serial.println(y_val);
  Serial.print("scroll flag: ");
  Serial.println(scroll_flag);
  delay(500);
  */

  key.Update();

  int scroll_click_count = key.clicks;

  // Exponential cursor acceleration
  if (exponential_acceleration) {
    if (x_val > 0) {
      x_val =  (int) pow(base,x_val);
    } else if (x_val < 0) {
      x_val = -(int) pow(base,-x_val);
    }
    if (y_val > 0) {
      y_val =  (int) pow(base,y_val);
    } else if (y_val < 0) {
      y_val = -(int) pow(base,-y_val);
    }
   }


  // Scroll Mode
  if ((scroll_click_count == -1 || scroll_flag) && key.depressed == true) {
    scroll_flag = 1;
    if (y_val <= -20) {
      Mouse.move(0, 0, -1);
      delay(100);
    } else if (y_val >= 20) {
      Mouse.move(0, 0, 1);
      delay(100);
    }
  } else {
    scroll_flag = 0;
  }

  // Zoom Mode
  if ((scroll_click_count == -2 || zoom_flag) && key.depressed == true) {
    zoom_flag = 1;
    if (y_val <= -20) {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('=');
      Keyboard.releaseAll();
      delay(500);
    } else if ( y_val >= 20) {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('-');
      Keyboard.releaseAll();
      delay(500);
    }
  } else {
    zoom_flag = 0;
  }

  // Normal Mode
  if (x_val != 0 && !scroll_flag) {
    Mouse.move(invert_mouse * (x_val / sensitivity), 0, 0);
  }
  if (y_val != 0 && !scroll_flag) {
    Mouse.move(0, invert_mouse * (y_val / sensitivity), 0);
  }

  // Detect Left and Right Clicks
  if (digitalRead(teeth_pin) == 0 && !left_click_flag) {
    left_click_flag = 1;
    Mouse.press(MOUSE_LEFT);
  } else if (digitalRead(teeth_pin) && left_click_flag) {
    left_click_flag = 0;
    Mouse.release(MOUSE_LEFT);
  }
  if (digitalRead(joystick_sel_pin) == 0 && !right_click_flag) {
    right_click_flag = 1;
    Mouse.press(MOUSE_RIGHT);
  } else if (digitalRead(joystick_sel_pin) && right_click_flag) {
    right_click_flag = 0;
    Mouse.release(MOUSE_RIGHT);
  }
}
