/*
  Description: Sketch for wired tongue operated mouse.
  Date: January 2021
  Author: Naren Yenuganti
*/

#include <Mouse.h>

int horz_pin = A0;
int vert_pin = A1;
int joystick_sel_pin = 7;
int teeth_pin = 8;
int scroll_pin = 9;
int vert_zero, horz_zero;
int y_val, x_val;
int right_click_flag = 0;
int left_click_flag = 0;
int scroll_click_flag = 0;

// Higher sensitivity value = slower mouse, should be <= about 500
const int sensitivity = 150;

// To invert, to -1
int invert_mouse = 1;

// Reaction time in ms
int reaction_time = 500;

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
}

void loop() {
  y_val = analogRead(vert_pin) - vert_zero;
  x_val = analogRead(horz_pin) - horz_zero;

  if (digitalRead(scroll_pin) == 0 && scroll_click_flag) {
    if (y_val <= -20) {
      Mouse.move(0,0,-1);
      delay(100);
    } else if ( y_val >= 20) {
      Mouse.move(0,0,1);
      delay(100);
    }
  } else if (digitalRead(scroll_pin) && scroll_click_flag) {
    scroll_click_flag = 0;
  }

  if (x_val != 0) {
    Mouse.move(invert_mouse * (x_val / sensitivity), 0, 0);    
  }
  if (y_val != 0) {
    Mouse.move(0, invert_mouse * (y_val / sensitivity), 0);    
  }

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

  if (digitalRead(scroll_pin) == 0 && !scroll_click_flag) {
    scroll_click_flag = 1;
  } 
}
