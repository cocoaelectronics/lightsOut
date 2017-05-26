#include <SPI.h>

const int OE = 9;
const int slaveSelectPin = 10;

const short ledArrayHeight = 4;
const short ledArrayWidth  = 4;
boolean ledArray[ledArrayHeight][ledArrayWidth];

/* 
 * Set each LED in the array to be either high or low
 * Input:
 *  leds:   Pointer two dimensional boolean to array to be randomized
 *  height: Height of array
 *  width:  Width of array
 * Side Effects: Assigns the array entries to be 1 or 0 randomly
 * Does not actually turn the LEDs on or off
 * Call UpdateLedArray afterwards to actually turn on or off the LEDS
 */
void randomizeLedArray(boolean leds[ledArrayHeight][ledArrayWidth]){
  for(short i = 0; i < ledArrayHeight; i++){
    for(short k = 0; k < ledArrayWidth; k++){
      //leds[i][k] = random(10)%2;
      leds[i][k] = 1; // TESTING ONLY
    }
  }
}

// Update LEDs to match array
void updateLedArray(boolean leds, const short height, const short width){

}

void setup() {
  pinMode( OE, OUTPUT );
  // Set the slaveSelectPin as output
  pinMode( slaveSelectPin, OUTPUT );

  delay(1000);

  digitalWrite(OE, HIGH);
  digitalWrite(slaveSelectPin, LOW);
  
  // Initialize SPI
  SPI.beginTransaction(SPISettings(30000000, LSBFIRST, SPI_MODE2));
  //SPI.begin();
  // Initialize array to random bool values
  //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  randomizeLedArray(ledArray);

  
  // Set up two bytes to transfer
  // Each bit represents one led
  byte b0 = B10011100;
  byte b1 = B01000010;
  short b = b1 << 8;
  b += b0;
  b = 16; 
  // Take the SS pin low to select the chip
  

  //SPI.transfer( b0 );
  //SPI.transfer( b1 );
  SPI.transfer16(b);

  // Take the ss pin high to deselect the pin
  digitalWrite(slaveSelectPin, HIGH);
  //delay(50);
  //digitalWrite(slaveSelectPin, LOW);

  digitalWrite(OE, LOW);
  delay(50);                                                                 
  digitalWrite(OE, HIGH);
  
}

void loop() {
  
}
