#include <Arduino.h>
#include "../Transmission/NRF204.h"

#pragma once

namespace OnBoardController{
    class AircraftController{
        private:
            void listen();
            void act(Radio::NRF204AircraftControllMessage message);
        public:
            AircraftController();
            AircraftController(const OnBoardController::AircraftController &aircraftController);
            AircraftController(uint8_t aileronPin1,uint8_t aileronPin2,uint8_t rudderPin,uint8_t stabilizerPin1,uint8_t stabilizerPin2,uint8_t motorPin);
            void run();
            int8_t aileronPin1;
            uint8_t aileronPin2;
            uint8_t rudderPin;
            uint8_t stabilizerPin1;
            uint8_t stabilizerPin2;
            uint8_t motorPin;
            Radio::NRF204 radioReciever;

    };
}