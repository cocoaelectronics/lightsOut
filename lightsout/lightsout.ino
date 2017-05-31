// Include the SPI library
#include <SPI.h>

// Include library for using the LED driver
#include "STP16CP26.h" 

// Pins for slider switch at top
#define SW1 2
#define SW2 3
#define SW3 4
#define SW4 5

// Pins for push buttons along side
#define push1 A1
#define push2 A2
#define push3 A3
#define push4 A4

#define OE 9
#define CHIPSELECTPIN 10

#define LEDARRAYHEIGHT  4
#define LEDARRAYWIDTH 4

boolean ledArray[LEDARRAYHEIGHT][LEDARRAYWIDTH];

SPISettings spiSettings(14000000,MSBFIRST,SPI_MODE0); // instantiate spiSettings

STP16CP26 ledDriver(OE,CHIPSELECTPIN); // instantiate object ledDrive of class STP16CP26
/* 
 * Set each LED in the array to be either high or low
 * Input:
 *  leds:   Pointer two dimensional boolean to array to be randomized
 *  height: Height of array
 *  width:  Width of array
 * Side Effects: AchipSelectPinigns the array entries to be 1 or 0 randomly
 * Does not actually turn the LEDs on or off
 * Call UpdateLedArray afterwards to actually turn on or off the LEDS
 */
void randomizeLedArray(boolean leds[LEDARRAYHEIGHT][LEDARRAYWIDTH]){
  for(short i = 0; i < LEDARRAYHEIGHT; i++){
    for(short k = 0; k < LEDARRAYWIDTH; k++){
      leds[i][k] = random(10)%2;
    }
  }
}

// Generate a transfer value from the array.
// Returns transfer value
uint16_t generateTransferValue(boolean leds[LEDARRAYHEIGHT][LEDARRAYWIDTH]){
  // The controller sees the leds as a linear row
  // count denotes which led we're at
  // Then we can shift the value for that LED into the proper position in the transfer value
  short count = 0;
  uint16_t transferValue = 0;

  // Build transfer value
  // A (1) in a position indicates that led should be turned on
  for(short height = 0; height < 4; height++){
    for(short width = 0; width < 4; width++){
      // If we want the led to be turned on
      // Shift a 1 into that position in the transfer value
      if(leds[height][width] == 1){
        transferValue += (1 << count);
      }

      count++;
    }
  }

  return transferValue;
}


//------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600); // initialize serial at 9600 baud
  Serial.println("Setup...");
  
  pinMode(CHIPSELECTPIN,OUTPUT);
  pinMode(OE, OUTPUT );
  pinMode(13, OUTPUT); // SCK set to output
  pinMode(11,OUTPUT);  // MOSI set to output
  
  Serial.println("Setup complete...");
  ledDriver.init(0b0101010101010101);

  
  // Delay 2 seconds after setup
  delay(2000);
}

void loop() {

  // Test code. Flashes a random pattern and then a test pattern.
  randomizeLedArray(ledArray);
  ledDriver.setState(generateTransferValue(ledArray));
  delay(1000);
  ledDriver.setState(0b1010101010101010);
  delay(1000);
  

 // check status of switches
 Serial.print("SW1 = "); Serial.print(digitalRead(SW1)); Serial.print(" SW2 = "); Serial.print(digitalRead(SW2)); Serial.print(" SW3 = "); Serial.print(digitalRead(SW3)); Serial.print(" SW4 = "); Serial.print(digitalRead(SW4));Serial.println();
 Serial.print("PB1 = "); Serial.print(digitalRead(push1)); Serial.print(" PB2 = "); Serial.print(digitalRead(push2)); Serial.print(" PB3 = "); Serial.print(digitalRead(push3)); Serial.print(" PB4 = "); Serial.print(digitalRead(push4));Serial.println('\n');
  


/*
  


  

  //SPI.transfer( b1 );
  SPI.transfer16(0xA5A5);

  // Take the chipSelectPin pin high to deselect the pin
  digitalWrite(chipSelectPin, HIGH);
  delay(50);
  digitalWrite(chipSelectPin, LOW);

  digitalWrite(OE, LOW);
*/

}
