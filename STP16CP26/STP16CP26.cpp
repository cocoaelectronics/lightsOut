


#include "STP16CP26.h"




STP16CP26::STP16CP26(const int OE, const int chipSelectPin):_OE(OE),_cs(chipSelectPin){

}

void STP16CP26::init(uint16_t initLEDState){
	ledState = initLEDState;
	setState(ledState);
	
	
}

void STP16CP26::setState(uint16_t state){
	SPI.beginTransaction(SPISettings(14000000,MSBFIRST,SPI_MODE0));

	digitalWrite(_OE,HIGH);
	digitalWrite(_cs,LOW);
	SPI.transfer16(state);
	digitalWrite(_cs,HIGH);
	SPI.endTransaction();
	delay(10);
	digitalWrite(_cs,LOW);

	digitalWrite(_OE,LOW);
}

void STP16CP26::on(uint8_t pin){
	
}

void STP16CP26::off(uint8_t pin){
	
}