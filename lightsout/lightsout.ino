// Include the SPI library
#include <SPI.h>

// Include library for using the LED driver
#include "STP16CP26.h" 

// Pins for slider switch at top
#define SW1 5
#define SW2 4
#define SW3 3
#define SW4 2

// Pins for push buttons along side
#define PUSH1 A4
#define PUSH2 A3
#define PUSH3 A2
#define PUSH4 A1

#define OE 9
#define CHIPSELECTPIN 10

#define LEDARRAYHEIGHT  4
#define LEDARRAYWIDTH 4

//Variable to hold the state of each LED in the grid
boolean ledArray[LEDARRAYHEIGHT][LEDARRAYWIDTH];
int sliderArray[4] = {SW1, SW2, SW3, SW4};
int pushArray[4]   = {PUSH1,  PUSH2,  PUSH3,  PUSH4};
// Variable to save which button/slider value has been selected each iteration
int sliderSelect = 0;
int pushSelect   = 0;

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
  // Set up random number generator with a new seed
  // If this doesn't happen, it gives the same pattern every time you turn it on
  randomSeed(analogRead(0));
  
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

// Toggle selected LEDs, and the adjacent LEDs as well
void toggleLedAndNeighbors(boolean leds[LEDARRAYHEIGHT][LEDARRAYWIDTH]){
  
}



//------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600); // initialize serial at 9600 baud
  Serial.println("Setup...");

  
  pinMode(CHIPSELECTPIN,OUTPUT);
  pinMode(OE, OUTPUT );
  pinMode(13, OUTPUT); // SCK set to output
  pinMode(11,OUTPUT);  // MOSI set to output

  // Set slider pinmodes
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(SW3,INPUT);
  pinMode(SW4,INPUT);

  // Set pushbutton pinmodes
  pinMode(PUSH1,INPUT);
  pinMode(PUSH2,INPUT);
  pinMode(PUSH3,INPUT);
  pinMode(PUSH4,INPUT);

  
  Serial.println("Setup complete...");
  randomizeLedArray(ledArray);
  ledDriver.init(generateTransferValue(ledArray));

  
  // Delay 2 seconds after setup
  delay(2000);

}

void loop() {

  boolean buttonSelected = false;
  
  // See which position the slider is in
  for(int i = 0; i < LEDARRAYWIDTH; i++){
    if( digitalRead(sliderArray[i]) == LOW){
      sliderSelect = i;
      break;
    }
  }

  // See which push button is selected, if any
  for(int i = 0; i < LEDARRAYHEIGHT; i++){
    if( digitalRead(pushArray[i]) == LOW ){
      pushSelect = i;
      buttonSelected = true;
    }
  }


  // If a button was pressed, toggle the LED at the intersection of the button and the slider
  if( buttonSelected ){
    ledArray[pushSelect][sliderSelect] = !ledArray[pushSelect][sliderSelect];
    ledDriver.setState( generateTransferValue(ledArray) );
  }
  
 // check status of switches
 Serial.print("SW1 = "); Serial.print(digitalRead(SW1)); Serial.print(" SW2 = "); Serial.print(digitalRead(SW2)); Serial.print(" SW3 = "); Serial.print(digitalRead(SW3)); Serial.print(" SW4 = "); Serial.print(digitalRead(SW4));Serial.println();
 Serial.print("PB1 = "); Serial.print(digitalRead(PUSH1)); Serial.print(" PB2 = "); Serial.print(digitalRead(PUSH2)); Serial.print(" PB3 = "); Serial.print(digitalRead(PUSH3)); Serial.print(" PB4 = "); Serial.print(digitalRead(PUSH4));Serial.println('\n');
}
