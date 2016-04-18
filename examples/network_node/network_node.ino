#include <Automaton.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "Atm_RF24Network.h"


// initalize RF24 Radio & network
RF24 radio(9, 10);
RF24Network network(radio);

// initalize RF24Network Machine
Atm_RF24Network atm_net(network);

// message callback declaration
void msgcb();


void setup()
{
  Serial.begin(9600);

  // start rf24 network stuff
  SPI.begin();
  radio.begin();
  // can configure RF24 radio, channel, PAlevel etc
  radio.setChannel(115);
  radio.setPALevel(RF24_PA_MAX);
  
  // register custom message handler
  atm_net.onMsg(msgcb);
  // begin RF24Network within machine, pass nodes address
  atm_net.begin(04);
  delay(250);
  char pl[] = "test payload"
  atm_net.send(pl, sizeof(pl));
  // debugging
  atm_net.onSwitch(atm_serial_debug::onSwitch);
}


void loop()
{
  // calls network.update() ON_LOOP to ensure network is up.
  atm_net.cycle();

}

// message callback definition
void msgcb()
{
  RF24NetworkHeader header;
  char payload[65];
  size_t full_len = network.read(header, &payload, sizeof(payload));
  payload[full_len] = '\0';
  Serial.println(payload);
  Serial.println("CUSTOM HANDLER");
}