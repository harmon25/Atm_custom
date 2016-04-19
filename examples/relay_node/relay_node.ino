#include <Automaton.h>
#include <Atm_button.h>
#include "Atm_relay.h"
#include <Atm_RF24Network.h>

Atm_button btn1,btn2;    // An Atm_button machine
Atm_relay relay1, relay2; // Custom Atm_relay machine

// initalize RF24 Radio & network
RF24 radio(9, 10);

// initalize RF24Network Machine
Atm_RF24Network atm_net(radio);

// message callback declaration
void msgcb();

// button callback declaration
void btn_change( int press, int idx );

// And finally a factory to house them in
Factory factory; 

void setup() {
  Serial.begin( 9600 );
  // init relays: relay pin, led pin
  relay1.begin( 3, 5 ).label( "RELAY_R" );
  relay2.begin( 4, 6 ).label( "RELAY_G" );
  // ensure relays start OFF, could add an idle state to indicate not on or off, which would be off..
  relay1.state(relay1.OFF);
  relay2.state(relay2.OFF);

  btn1.begin(1).onPress(btn_change, 1 );
  btn2.begin(2).onPress(btn_change, 2 );

  btn1.begin(1).onPress(btn_change, 1 );
  btn2.begin(2).onPress(btn_change, 2 );

  // register custom message handler
  atm_net.onMsg(msgcb);
  // begin RF24Network within machine, pass nodes address
  atm_net.begin(115, 01);

  factory
  .add(atm_net)
  .add(relay1)
  .add(relay2)
  .add(btn1)
  .add(btn2);
}

void loop() {
  factory.cycle();
  // put your main code here, to run repeatedly:
}



// button callback
void btn_change( int press, int idx )
{
  if ( press ) {
     if ( idx == 1 )
     {
      relay1.trigger(relay1.EVT_TOGGLE);
      Serial.println(relay1.inst_label);
      Serial.println(relay1.state());
      char pl[] = "test payload";
      atm_net.send(pl, sizeof(pl), 'A');
     }

     if(idx == 2)
     {
      relay2.trigger(relay2.EVT_TOGGLE);
      Serial.println(relay2.inst_label);
      Serial.println(relay2.state());
      char pl[] = "test payload";
      atm_net.send(pl, sizeof(pl), 'A');
     }
  }
}


// message callback definition
void msgcb()
{
  RF24NetworkHeader header;
  char payload[65];
  size_t full_len = atm_net._network.read(header, &payload, sizeof(payload));
  payload[full_len] = '\0';
  Serial.println(payload);
  Serial.println("CUSTOM HANDLER");
}
