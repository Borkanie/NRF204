#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

//scoped enum
enum MonitorFrequency:uint32_t{
    None=0,
    Slow=9600,
    Medium=38400,
    Fast=115200
};

namespace Radio{
    class NRF204{

        private:
        // instantiate an object for the nRF24L01 transceiver
            RF24 radio=RF24(7, 8); // using pin 7 for the CE pin, and pin 8 for the CSN pin
        // Let these addresses be used for the pair
        // It is very helpful to think of an address as a path instead of as
        // an identifying device destination
            uint8_t address[2][6]= {"1Node", "2Node"};
        // to use different addresses on a pair of radios, we need a variable to
        // uniquely identify which address this radio will use to transmit
            bool radioNumber=1;
        // Used to control whether this node is sending or receiving
            bool role=false;
        // true = TX role, false = RX role
        // For this example, we'll be using a payload containing
        // a single float number that will be incremented
        // on every successful transmission
            float payload = 0.0;

            MonitorFrequency pcCommunication;
    public:
        NRF204(bool radionumber,bool role,MonitorFrequency frequency=MonitorFrequency::None);
        void Setup();
        void Run();
    };
}

