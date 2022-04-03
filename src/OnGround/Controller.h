#include <Arduino.h>
#include <string.h>
#include "../Transmission/NRF204.h"

#pragma once

using namespace Radio;

namespace OnGroundController{

    class Reader{
        private:
            uint8_t rudderPin;
            uint8_t aileronPin;
            uint8_t motorPin;
            uint8_t stabilizerPin;
            Radio::NRF204AircraftControllMessage message;
            void readMotorPin();
            void readRudderPin();
            void readAileronPin();
            void readStabilizerPin();
            Radio::NRF204 radioTransmitter;
            Radio::NRF204AircraftControllMessage getMessage();
        public:
            Reader(uint8_t rudderPin,uint8_t aileronPin,uint8_t motorPin,uint8_t stabilizerPin);
            Reader();
            Reader(const OnGroundController::Reader &reader);
            void run();

    };
}