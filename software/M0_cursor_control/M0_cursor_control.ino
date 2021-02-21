// Cursor movement using Trinket M0

#include <Mouse.h>

int x = A3;
int y = A4;
const int sensitivity = 20; // larger sensitivity --> slower mouse
const unsigned long loopPeriod = 15; // larger loop_period --> more delay
int yZero, xZero;
boolean debug = true;


void setup() {
  pinMode(x, INPUT);
  pinMode(y, INPUT);

  yZero = analogRead(y);
  xZero = analogRead(x);
}

void loop() {
  static unsigned long lastLoop;

  int yReading = analogRead(y);
  int xReading = analogRead(x);

  int yVal = (yReading - yZero) / sensitivity;
  int xVal = (xReading - xZero) / sensitivity;
  
  if (debug) {
    Serial.print("yZero :");
    Serial.println(yZero);
    Serial.print("xZero :");
    Serial.println(xZero);
  
    Serial.print("y analog :");
    Serial.println(yReading);
    Serial.print("x analog :");
    Serial.println(xReading);
  
    Serial.print("sensitivity :");
    Serial.println(sensitivity);
    
    Serial.print("x value: ");
    Serial.println(xVal);
    Serial.print("y value: ");
    Serial.println(yVal);
  }


  if (yVal > 1 || yVal < -1) {
      Mouse.move(0, yVal, 0);  // move mouse on y axis
    }
    if (xVal > 1 || yVal < -1) {
      Mouse.move(xVal, 0, 0);  // move mouse on x axis
    }
    while(millis() - lastLoop < loopPeriod) {
      // delay
    }
    lastLoop = millis();
}
