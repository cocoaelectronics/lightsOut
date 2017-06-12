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
// Input:
//   - leds: Array representing LED grid
//   - x: X coordinate of LED to toggle
//   - y: Y coordinate of LED to toggle
// Side Effects: Changes values in leds array passed in
void toggleLedAndNeighbors(boolean leds[LEDARRAYHEIGHT][LEDARRAYWIDTH], int x, int y){
  // Toggle main LED
  leds[y][x] = !leds[y][x];

  // Toggle Neighboring LEDS
  // When toggling the neighboring LEDs, do nothing if the neighbor is outside the grid
  // i.e. don't wrap
  // Toggle LED above
  if( (y-1) >= 0 && (y-1) < LEDARRAYHEIGHT ){
    leds[y-1][x] = !leds[y-1][x];
  }
  // Toggle LED left
  if( (x-1) >= 0 && (x-1) < LEDARRAYWIDTH ){
    leds[y][x-1] = !leds[y][x-1];
  }
  // Toggle LED right
  if( (x+1) >= 0 && (x+1) < LEDARRAYWIDTH ){
    leds[y][x+1] = !leds[y][x+1];
  }
  // Toggle LED left
  if( (y+1) >=0 && (y+1) < LEDARRAYHEIGHT ){
    leds[y+1][x] = !leds[y+1][x];
  }
}

// Randomize the LEDs in the array, and output that to the grid
void setupGame(boolean leds[LEDARRAYHEIGHT][LEDARRAYWIDTH]){
  randomizeLedArray(leds);
  ledDriver.init(generateTransferValue(ledArray));
}

// Check if every LED is cleared
// Return a boolean indicating if the grid is cleared
// true = game over
// false = game on
boolean isCleared(boolean leds[LEDARRAYHEIGHT][LEDARRAYWIDTH]){
  boolean solved = true;
  
  for(int i = 0; i < LEDARRAYHEIGHT; i++){
    for(int k = 0; k < LEDARRAYWIDTH; k++){\
      // Check if the LED at this position should be on
      // Worth noting that the values in ledArray can differ from what is displayed on the grid
      if( leds[i][k] == true ){
        solved = false;
        break;
      }
    }
    if( solved == false ){
      break;
    }
  }

  return solved;
}

// Just here to test functionality. Not for use in the real game.
// This will light up all the leds one by one
//  display a random pattern
//  turn all the leds off one by one
//  reset the game once it is cleared
void testFunctions(boolean leds[LEDARRAYHEIGHT][LEDARRAYWIDTH] ){
  // Test on
  for( int i = 0; i < 16; i++ ){
    ledDriver.on(i);
    delay(100);
  }

  setupGame(ledArray);
  delay(1000);
  
  // Test off
  for( int i = 0; i < 16; i++ ){
    ledDriver.off(i);
    delay(100);
  }

  // off() doesn't actually effect the ledArray values, so you need to update the array to match reality
  for( int i = 0; i < LEDARRAYHEIGHT; i++ ){
    for( int k = 0; k < LEDARRAYWIDTH; k++ ){
      ledArray[i][k] = false;
    }
  }

  // Check if the game is cleared(over).
  // If so, restart
  if( isCleared(ledArray) == true ){
    ledDriver.setState(0);
    delay(250);
    ledDriver.setState(0b1111111111111111);
    delay(250);
    ledDriver.setState(0);
    delay(250);
    ledDriver.setState(0b1111111111111111);
    delay(250);
    ledDriver.setState(0);
    delay(250);
    ledDriver.setState(0b1111111111111111);
    delay(250);

    setupGame(ledArray);
  }
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

  // Enable for testing
  // testFunctions( ledArray );
  
  setupGame( ledArray );
  
  // Delay 2 seconds after setup
  delay( 2000 );

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
      delay(100); // add delay to ensure not constantly reading
      buttonSelected = true;
    }
  }


  // If a button was pressed, toggle the LED at the intersection of the button and the slider
  if( buttonSelected ){
    //ledArray[pushSelect][sliderSelect] = !ledArray[pushSelect][sliderSelect];
    toggleLedAndNeighbors(ledArray, sliderSelect, pushSelect);
    ledDriver.setState( generateTransferValue(ledArray) );
  }

  // Check if the game is cleared(over).
  // If so, restart
  if( isCleared(ledArray) ){
    ledDriver.setState(0);
    delay(250);
    ledDriver.setState(0b1111111111111111);
    delay(250);
    ledDriver.setState(0);
    delay(250);
    ledDriver.setState(0b1111111111111111);
    delay(250);
    ledDriver.setState(0);
    delay(250);
    ledDriver.setState(0b1111111111111111);
    delay(250);

    setupGame(ledArray);
  }
  
 // check status of switches
 Serial.print("SW1 = "); Serial.print(digitalRead(SW1)); Serial.print(" SW2 = "); Serial.print(digitalRead(SW2)); Serial.print(" SW3 = "); Serial.print(digitalRead(SW3)); Serial.print(" SW4 = "); Serial.print(digitalRead(SW4));Serial.println();
 Serial.print("PB1 = "); Serial.print(digitalRead(PUSH1)); Serial.print(" PB2 = "); Serial.print(digitalRead(PUSH2)); Serial.print(" PB3 = "); Serial.print(digitalRead(PUSH3)); Serial.print(" PB4 = "); Serial.print(digitalRead(PUSH4));Serial.println('\n');
}
