#include <Automaton.h>
#include "Atm_RF24Network.h"
#include <Atm_timer.h>


// initalize RF24 Radio & network
RF24 radio(9, 10);
// initalize RF24Network Machine
Att_RF24Network att_net(radio);
Att_timer ping_timer;

// message callback declaration
void msg_callback();
void timer_callback( int id, uint16_t cnt );

void setup()
{ 
  Serial.begin(9600);
  ping_timer.begin();
  ping_timer.interval_seconds( 15 ).repeat( ATM_COUNTER_OFF ); // repeat forever, every 15 seconds
  ping_timer.onTimer(timer_callback).id(1);

 
  // register custom message handler
  att_net.onMsg(msg_callback);
  // calls radio.begin and network.begin
  // channel, node address as params.
  att_net.begin(115, 04);
  delay(250);
  // send a test payload
  char pl[] = "test payload";
  att_net.send(pl, sizeof(pl), 'A');
}

void loop()
{
  // calls network.update() ON_LOOP to ensure network is up.
  att_net.cycle();
  ping_timer.cycle();

}

// timers setup as seperate machines, onTimer callback
void timer_callback( int id, uint16_t cnt ) 
{
  // Something to do when the timer goes off
  char pl[] = "test ping";
  att_net.send(pl, sizeof(pl), 1);
  
}


// message callback definition
void msg_callback()
{
  RF24NetworkHeader header;
  char payload[65];
  size_t full_len = att_net._network.read(header, &payload, sizeof(payload));
  payload[full_len] = '\0';
  Serial.println(payload);
  Serial.println("CUSTOM HANDLER");
}

