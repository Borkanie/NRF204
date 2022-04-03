#include <Arduino.h>
#include "Airplane.h"

using namespace std;
using namespace Radio;

using namespace OnBoardController;
    void AircraftController::listen(){
        
    }
    void AircraftController::act(Radio::NRF204AircraftControllMessage message){
        analogWrite(this->aileronPin1,message.aileronValue);
        analogWrite(this->aileronPin2,-message.aileronValue);
        analogWrite(this->motorPin,message.motorValue);
        analogWrite(this->stabilizerPin1,message.stabilizerValue);
        analogWrite(this->stabilizerPin2,message.stabilizerValue);
        analogWrite(this->rudderPin,message.rudderValue);
        
    }

    AircraftController::AircraftController(const AircraftController &aircraftController){
        this->aileronPin1=aircraftController.aileronPin1;
        this->aileronPin2=aircraftController.aileronPin1;
        this->rudderPin=aircraftController.rudderPin;
        this->stabilizerPin1=aircraftController.stabilizerPin1;
        this->stabilizerPin2=aircraftController.stabilizerPin2;
        this->motorPin=aircraftController.motorPin;
        this->radioReciever=aircraftController.radioReciever;
    }

    AircraftController::AircraftController(){
        this->radioReciever=Radio::NRF204(false,true,MonitorFrequency::Slow,8,9);
    }
    AircraftController::AircraftController(uint8_t aileronPin1,uint8_t aileronPin2,uint8_t rudderPin,uint8_t stabilizerPin1,uint8_t stabilizerPin2,uint8_t motorPin){
            this->aileronPin1=aileronPin1;
            this->aileronPin2=aileronPin2;
            this->rudderPin=rudderPin;
            this->motorPin=motorPin;
            this->stabilizerPin1=stabilizerPin1;
            this->stabilizerPin2=stabilizerPin2;
            this->radioReciever=NRF204(true,false,MonitorFrequency::Slow,24,25);
    }
    void AircraftController::run(){
        Serial.println("Run");
        uint8_t values[4];
        this->radioReciever.recieve(values);
        NRF204AircraftControllMessage message(values);
        this->act(message);
    }

  
