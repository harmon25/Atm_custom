#include <Automaton.h>
#include "Atm_RF24Network.h"

Att_RF24Network & Att_RF24Network::begin(uint8_t radio_channel, uint16_t node_addr)
{
  const static tiny_state_t state_table[] PROGMEM = {
  /*               ON_ENTER         ON_LOOP      ON_EXIT   EVT_NEW_MSG         ELSE */
  /* IDLE  */            -1,    UPDATE_NETWORK,   -1,      RECIEVING,           -1,
  /* RECIEVING */  RECV_MSG,         -1,          -1,        -1,               IDLE,

   };
   // begin rf24network, with node address param
   SPI.begin();
   _radio.begin();
   _radio.setChannel(radio_channel);
   _radio.setPALevel(RF24_PA_MAX);

  network.begin(node_addr);
  
  
  // begin machine
  TinyMachine::begin( state_table, ELSE );

  return *this; 
}

// register msg handler callback
Att_RF24Network & Att_RF24Network::onMsg( msgcb_t msg_callback ) 
{
  callback = msg_callback;
  return *this;  
}

int Att_RF24Network::event(int id)
{
   switch ( id )
   {  
      // if network is available, got a new message
      case EVT_NEW_MSG :
        return network.available();
   }
   // no events
   return 0;
}

bool Att_RF24Network::send(char* payload, size_t payload_len, uint8_t msg_type )
{
  RF24NetworkHeader header(00, msg_type);
  return network.write(header, payload, payload_len);
}

void Att_RF24Network::action(int id)
{
 switch ( id )
  {
    // keep network running, needs to be run in loop, and regularly
    case UPDATE_NETWORK :
     network.update();
     return;
    // we have a message for us to recieve
    case RECV_MSG :
    { // if a callback has been registered, do it
      if(callback)
      {
        (*callback)( );
      } else {
        // no callback, just read payload and print it out
        // without reading a payload when network is ready, it will be ready until message is read
        RF24NetworkHeader header;
        //_network.peek(header);
        char payload[100]; 
        size_t full_len = network.read(header, &payload, sizeof(payload));
        payload[full_len] = '\0';
        Serial.println(payload);
      }
      return;
    }
  }
}
/*
Att_RF24Network & Att_RF24Network::onSwitch(swcb_sym_t switch_callback)
{
  Machine::onSwitch( switch_callback, "IDLE\0RECIEVING", "EVT_NEW_MSG\0ELSE" );
  return *this;
}
*/