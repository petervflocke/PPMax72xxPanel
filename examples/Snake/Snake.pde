#include <SPI.h>
#include <Adafruit_GFX.h>
#include <PPMax72xxPanel.h>

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 8;
int numberOfVerticalDisplays = 1;

#define ledPin 2

#define SNs 40
#define SNe 64
#define MaxSnake (SNe-SNs)*8
#define SnakeAttempt 10
#define SankeNextRound 10

PPMax72xxPanel matrix = PPMax72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

const int pinRandom = 32;

const int wait = 100; // In milliseconds
int snakeLength = 1;

int snakeX[MaxSnake], snakeY[MaxSnake];
int ptr, nextPtr;
int snakeRound = 0;

void setup() {

    pinMode(ledPin,  OUTPUT); // passing seconds LED
    
    matrix.setIntensity(4); // Use a value between 0 and 15 for brightness
  
    matrix.setPosition(0, 7, 0); matrix.setRotation(0, 2);    
    matrix.setPosition(1, 6, 0); matrix.setRotation(1, 2);
    matrix.setPosition(2, 5, 0); matrix.setRotation(2, 2);
    matrix.setPosition(3, 4, 0); matrix.setRotation(3, 2);
    matrix.setPosition(4, 3, 0); matrix.setRotation(4, 2);
    matrix.setPosition(5, 2, 0); matrix.setRotation(5, 2);
    matrix.setPosition(6, 1, 0); matrix.setRotation(6, 2);
    matrix.setPosition(7, 0, 0); matrix.setRotation(7, 2);

  // Reset all variables
  for ( ptr = 0; ptr < snakeLength; ptr++ ) {
    snakeX[ptr] = SNs+(SNe-SNs) / 2;
    snakeY[ptr] = matrix.height() / 2;
  }
  nextPtr = 0;

  randomSeed(analogRead(pinRandom)); // Initialize random generator
}

void loop() {

  matrix.setClip(SNs,SNe,0,8);
  // Shift pointer to the next segment
  ptr = nextPtr;
  nextPtr = next(ptr);

  matrix.drawPixel(snakeX[ptr], snakeY[ptr], HIGH); // Draw the head of the snake
  matrix.write(); // Send bitmap to display

  delay(wait);

  if ( ! occupied(nextPtr) ) {
    matrix.drawPixel(snakeX[nextPtr], snakeY[nextPtr], LOW); // Remove the tail of the snake
    digitalWrite(ledPin, LOW);
  }

  int attempt;
  for ( attempt = 0; attempt < SnakeAttempt; attempt++ ) {

    // Jump at random one step up, down, left, or right
    switch ( random(4) ) {
      case 0: 
          snakeX[nextPtr] = (snakeX[ptr] + 1 >= SNe) ? SNs : snakeX[ptr] + 1;
          snakeY[nextPtr] = snakeY[ptr]; 
          break;
      case 1:
          snakeX[nextPtr] = (snakeX[ptr] - 1 < SNs) ? SNe-1 : snakeX[ptr] - 1;      
          snakeY[nextPtr] = snakeY[ptr]; 
          break;
      case 2: 
          snakeY[nextPtr] = (snakeY[ptr] + 1 > matrix.height()-1)? 0 : snakeY[ptr] + 1;
          snakeX[nextPtr] = snakeX[ptr]; break;
      case 3:
          snakeY[nextPtr] = (snakeY[ptr] - 1 < 0)? matrix.height()-1 : snakeY[ptr] - 1;
          snakeX[nextPtr] = snakeX[ptr]; break;
    }    

    if ( occupied(nextPtr) ) {
      //digitalWrite(ledPin, HIGH);
    } 
    else {
      break; // The spot is empty, break out the for loop
    }
  }
  if (attempt == SnakeAttempt) { 
    digitalWrite(ledPin, HIGH);
    matrix.fillScreen(HIGH);
    matrix.write();    
    delay(50);
    matrix.fillScreen(LOW);
    matrix.write();    
    for ( ptr = 0; ptr < snakeLength; ptr++ ) {
      snakeX[ptr] = SNs+(SNe-SNs) / 2;
      snakeY[ptr] = matrix.height() / 2;
    }
    nextPtr = 0;
    snakeLength = 1;
    snakeRound = 0;
  } else {
      snakeRound = (snakeRound +1) % SankeNextRound;
      if (snakeRound == 0) snakeLength = snakeLength + 1;
  }
}

boolean occupied(int ptrA) {
  for ( int ptrB = 0 ; ptrB < snakeLength; ptrB++ ) {
    if ( ptrA != ptrB ) {
      if ( equal(ptrA, ptrB) ) {
        return true;
      }
    }
  }
  return false;
}

int next(int ptr) {
  return (ptr + 1) % snakeLength;
}

boolean equal(int ptrA, int ptrB) {
  return snakeX[ptrA] == snakeX[ptrB] && snakeY[ptrA] == snakeY[ptrB];
}
