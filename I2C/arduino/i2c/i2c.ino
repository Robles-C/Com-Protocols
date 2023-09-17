#include <Wire.h>
#include "Arduino_LED_Matrix.h"

//  I2C slave address
#define SLAVE_ADDRESS 0x10

ArduinoLEDMatrix matrix;

// volatile flag and character variable for I2C data reception
volatile bool receivedFlag = false;
char receivedChar;

// Define frames for LED Matrix display
const uint32_t heart[] = {
    0x3184a444,
    0x44042081,
    0x100a0040
};
const uint32_t off[] = {
    0x00000000,
    0x00000000,
    0x00000000
};

void setup() {
  matrix.begin(); // initialize LED Matrix
  Wire.begin(SLAVE_ADDRESS); // initialize as I2C slave with address 0x10
  Wire.onReceive(receiveEvent); // register an event handler for I2C data reception
  Serial.begin(9600); // initialize for debugging
}

void loop() {
  // check if data has been received via I2C
  if (receivedFlag) {
    if(receivedChar == 'H'){
      matrix.loadFrame(heart); // display the heart shape on LED Matrix
      delay(500);
      matrix.loadFrame(off); // turn off the LED Matrix
      receivedFlag = false; // reset the received flag
    } else {
      // if the received character is not 'H', simply wait for 500ms
      delay(500);
      receivedFlag = false; // reset the received flag
    }
  }
}

// handler for I2C reception
void receiveEvent(int bytes) {
  // check if there is available data
  if (Wire.available()) {
    receivedChar = Wire.read(); // read the byte
    receivedFlag = true; // indicate that new data has arrived
  }
}
