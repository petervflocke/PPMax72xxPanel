#include <SPI.h>
#include <Adafruit_GFX.h>
#include <PPMax72xxPanel.h>
#include <PPmax72xxAnimate.h>

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 8;
int numberOfVerticalDisplays = 1;

#define s1 10
#define e1 32
#define s2 32
#define e2 60
#define s3  0
#define e3  9

/* ToDo:

fillScreen - think about cliping with memset or leave the GFX original part

*/

PPMax72xxPanel matrix = PPMax72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
PPmax72xxAnimate zoneText1 = PPmax72xxAnimate(&matrix);
PPmax72xxAnimate zoneText2 = PPmax72xxAnimate(&matrix);
PPmax72xxAnimate zoneText3 = PPmax72xxAnimate(&matrix);


String tape1 = "12345";
String tape2 = "1234567";
String tape3 = "12defghijklmnoprqstuvwxyz";
String tape4 = "1\n2\n3\n4\n5\n6\n7\n8\n9\n0";

int wait = 50; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

void setup() {

  Serial.begin(115200);
  delay(100);
  Serial.println("Animation started\n");

  matrix.setIntensity(4); // Use a value between 0 and 15 for brightness

  matrix.setPosition(0, 7, 0); // The first display is at <0, 0>
  matrix.setPosition(1, 6, 0); // The second display is at <1, 0>
  matrix.setPosition(2, 5, 0); // The third display is at <2, 0>
  matrix.setPosition(3, 4, 0); // And the last display is at <3, 0>
  matrix.setPosition(4, 3, 0); // And the last display is at <3, 0>
  matrix.setPosition(5, 2, 0); // And the last display is at <3, 0>
  matrix.setPosition(6, 1, 0); // And the last display is at <3, 0>
  matrix.setPosition(7, 0, 0); // And the last display is at <3, 0>

  matrix.setRotation(0, 2);
  matrix.setRotation(1, 2);
  matrix.setRotation(2, 2);
  matrix.setRotation(3, 2);
  matrix.setRotation(4, 2);
  matrix.setRotation(5, 2);
  matrix.setRotation(6, 2);
  matrix.setRotation(7, 2);

  #define ledPin 2
  pinMode(ledPin, OUTPUT); // passing seconds LED

  matrix.fillScreen(LOW);
  matrix.setCursor(0,0);
  // matrix.setTextColor(HIGH);
  matrix.setTextColor(HIGH, LOW);
  matrix.setTextWrap(false);

  zoneText1.setText(tape1, _SCROLL_LEFT, _TO_FULL, 150, s1, e1);
  zoneText2.setText(tape2, _SCROLL_RIGHT, _TO_LEFT, 50,  s2, e2);
  zoneText3.setText(tape4, _SCROLL_UP_SMOOTH, _NONE_MOD, 200,  s3, e3);

  zoneText1.Animate(true);
  zoneText1.Reset();;

}

void loop() {
  boolean updateDisplay = false;

  updateDisplay = false;
  updateDisplay |= zoneText1.Animate(false);
  updateDisplay |=zoneText2.Animate(false);
  updateDisplay |=zoneText3.Animate(false);

  if (updateDisplay) matrix.write();  // if updated needed write the buffer

  if (zoneText3.AnimateDone()) {
    zoneText3.Reset();
    delay (500);
  }
}
