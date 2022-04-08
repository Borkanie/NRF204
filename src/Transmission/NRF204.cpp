#include "NRF204.h" 
#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>

using namespace Radio;

NRF204AircraftControllMessage::NRF204AircraftControllMessage(){
    this->motorValue=0;
    this->rudderValue=0;
    this->stabilizerValue=0;
    this->aileronValue=0;
}

NRF204AircraftControllMessage::NRF204AircraftControllMessage(const Radio::NRF204AircraftControllMessage &message){
     this->motorValue=message.motorValue;
    this->rudderValue=message.rudderValue;
    this->stabilizerValue=message.stabilizerValue;
    this->aileronValue=message.aileronValue;
}

NRF204AircraftControllMessage::NRF204AircraftControllMessage(uint8_t values[4]){
    this->motorValue=values[0];
    this->rudderValue=values[1];
    this->stabilizerValue=values[2];
    this->aileronValue=values[3];
}

String NRF204AircraftControllMessage::toString(){
    String message="\nMotor value: "+String(this->motorValue)+"\n";
    message+="Rudder value: "+String(this->rudderValue)+"\n";
    message+="Stabilizer value: "+String(this->stabilizerValue)+"\n";
    message+="Aileron value: "+String(this->aileronValue)+"\n";
    return message;
}

NRF204::NRF204(){
  //this is uselesss
}

NRF204::NRF204(const NRF204 &nRF204){
  this->cEPin=nRF204.cEPin;
  this->cSNPin=nRF204.cSNPin;
  this->radioNumber=nRF204.radioNumber;
  this->role=nRF204.role;
  this->pcCommunication=nRF204.pcCommunication;
}

NRF204::NRF204(bool radio_number,bool role_to_be_assigned,MonitorFrequency frequency,int CEPin,int CSNPin){
  this->cEPin=CEPin;
  this->cSNPin=CSNPin;
  this->radioNumber=radio_number;
  this->role=role_to_be_assigned;
  this->pcCommunication=frequency;
  //this->reader=OnGroundController::Reader(A2,A1,A0,A3);
  this->setup();
}


void NRF204::setup()
{
  if(this->pcCommunication!=MonitorFrequency::None)
  {
    Serial.begin(pcCommunication);
    while (!Serial) {
      // some boards need to wait to ensure access to serial over USB
    }
    Serial.println("Serial started");
    radio=RF24(this->cEPin, this->cSNPin);
    // initialize the transceiver on the SPI bus
    Serial.println("Started RF24");
    if (!radio.begin()) {
      Serial.println("radio hardware is not responding!!");
      while (1) {} // hold in infinite loop
    }
    radio.setPayloadSize(sizeof(NRF204AircraftControllMessage));
    radio.setPALevel(RF24_PA_LOW); 
    Serial.println("Radio hardware is working:"+this->radioNumber);
  }

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

  // additional setup specific to the node's role
  if (role) {
    radio.stopListening();  // put radio in TX mode
  } else {
    radio.startListening(); // put radio in RX mode
  }
  

  // For debugging info
  // printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data
}

void NRF204::send(NRF204AircraftControllMessage message){
   // This device is a TX node

    unsigned long start_timer = micros();                    // start the timer
    //=this->reader.getMessage();
    bool report = radio.write(&message, sizeof(message));      // transmit & save the report
    unsigned long end_timer = micros();                      // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println(message.toString());                      // print payload sent
      payload += 0.01;                                       // increment float payload
    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }

    // to make this example readable in the serial monitor
    //delay(1000); 
     // slow transmissions down by 1 second

}

void NRF204::recieve(uint8_t (&values)[4]){
    Serial.println("Recieve");
    uint8_t pipe;
    Serial.println("Waiting for payload");
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      Serial.println("Expecting: "+bytes);
      radio.read(&values, bytes);            // fetch payload from FIFO
      Serial.print(F("Received "));
      Serial.print(bytes);                    // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);                     // print the pipe number
      Serial.print(F(": "));
      Serial.println(payload);                // print the payload's value
    }
    else{
      values[0]=0;
      values[1]=0;
      values[2]=0;
      values[3]=0;
    }
  }


