#include <Automaton.h>
#include <Atm_button.h>
#include "Att_relay.h"
#include <Atm_RF24Network.h>

Att_button btn1,btn2;    // An Atm_button machine
Att_relay relay1;
Att_relay relay2; // Custom Atm_relay machine

// initalize RF24 Radio & network
RF24 radio(9, 10);

// initalize RF24Network Machine
Att_RF24Network att_net(radio);

// message callback declaration
void msgcb();

// button callback declaration
void btn_change( int press, int idx );

// And finally a factory to house them in
TinyFactory factory; 

void setup() {
  Serial.begin( 9600 );
  // init relays: relay pin, led pin
  relay1.begin( 3, 5 );
  relay2.begin( 4, 6 );

  btn1.begin(1).onPress(btn_change1);
  btn2.begin(2).onPress(btn_change2);

  // register custom message handler
  att_net.onMsg(msgcb);
  // begin RF24Network within machine, pass nodes address
  att_net.begin(115, 01);

  factory
  .add(att_net)
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
void btn_change1( int press )
{
  if ( press ) {
      Serial.println(relay1.state());
      relay1.trigger(relay1.EVT_TOGGLE);
      char pl[] = "test payload";
      att_net.send(pl, sizeof(pl), 'A');
  }
}


// button callback
void btn_change2( int press)
{
  if ( press ) {
      Serial.println(relay2.state());
      relay2.trigger(relay2.EVT_TOGGLE);
      char pl[] = "test payload";
      att_net.send(pl, sizeof(pl), 'A');
  }
}


// message callback definition
void msgcb()
{
  RF24NetworkHeader header;
  char payload[65];
  size_t full_len = att_net._network.read(header, &payload, sizeof(payload));
  payload[full_len] = '\0';
  Serial.println(payload);
  Serial.println("CUSTOM HANDLER");
}
