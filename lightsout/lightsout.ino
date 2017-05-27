// Include the SPI library
#include <SPI.h>

const int OE = 9;
const int chipSelectPin = 10;



const short ledArrayHeight = 4;
const short ledArrayWidth  = 4;
boolean ledArray[ledArrayHeight][ledArrayWidth];

SPISettings spiSettings(14000000,LSBFIRST,SPI_MODE2); // instantiate spiSettings
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
//  SPI.begin();
  pinMode(chipSelectPin,OUTPUT);
  pinMode(13, OUTPUT); // SCK set to output
  pinMode(11,OUTPUT);  // MOSI set to output
  Serial.begin(9600); // initialize serial at 9600 baud
  Serial.println("Setup...");
  pinMode(OE, OUTPUT );


  SPI.beginTransaction(spiSettings);
  digitalWrite(OE, HIGH);
  digitalWrite(chipSelectPin, LOW);
  
  // Initialize SPI


  // Initialize array to random bool values
  //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  randomizeLedArray(ledArray);

  
  // Set up two bytes to transfer
  // Each bit represents one led
  byte b0 = B10011100;
  byte b1 = B01000010;
  uint16_t b = b1 << 8;
  b += b0;
  b = 0xA5A5; // random for testing 
  // Take the chipSelectPin pin low to select the chip
  

  //SPI.transfer( b1 );
  SPI.transfer16(b);

  // Take the chipSelectPin pin high to deselect the pin
  digitalWrite(chipSelectPin, HIGH);
  delay(50);
  digitalWrite(chipSelectPin, LOW);

  digitalWrite(OE, LOW);
  delay(50);                                                                 
  digitalWrite(OE, HIGH);

  Serial.println("Setup complete...");
}

void loop() {
  digitalWrite(chipSelectPin,LOW);
  SPI.transfer16(0xA5A5);
  digitalWrite(chipSelectPin,HIGH);

}
