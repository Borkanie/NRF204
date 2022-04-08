#include "Controller.h"
#include "../Transmission/NRF204.h"

using namespace std;
using namespace Radio;

using namespace OnGroundController;
    
Reader::Reader(){
    
}

Reader::Reader(const OnGroundController::Reader &reader){
    this->aileronPin=reader.aileronPin;
    this->message=reader.message;
    this->motorPin=reader.motorPin;
    this->radioTransmitter=reader.radioTransmitter;
    this->stabilizerPin=reader.stabilizerPin;
    this->rudderPin=reader.rudderPin;
}

Reader::Reader(uint8_t rudderPin,uint8_t aileronPin,uint8_t motorPin,uint8_t stabilizerPin){
    this->rudderPin=rudderPin;
    this->aileronPin=aileronPin;
    this->motorPin=motorPin;
    this->stabilizerPin=stabilizerPin;
    this->message= NRF204AircraftControllMessage();
    this->radioTransmitter=NRF204(false,true,MonitorFrequency::Slow,8,9);
}

void Reader::readMotorPin(){
    this->message.motorValue=(analogRead(this->motorPin)>>2);
}

void Reader::readRudderPin(){
    this->message.rudderValue=(analogRead(this->rudderPin)>>2);
}

void Reader::readAileronPin(){
    this->message.aileronValue=(analogRead(this->aileronPin)>>2);
}

void Reader::readStabilizerPin(){
    this->message.stabilizerValue=(analogRead(this->stabilizerPin)>>2);
}

Radio::NRF204AircraftControllMessage Reader::getMessage(){
    this->readMotorPin();
    this->readAileronPin();
    this->readRudderPin();
    this->readStabilizerPin();
    return this->message;
}

void Reader::run(){
    Radio::NRF204AircraftControllMessage mesaj=this->getMessage();
    this->radioTransmitter.send(mesaj);
    delay(1000);
    
}

