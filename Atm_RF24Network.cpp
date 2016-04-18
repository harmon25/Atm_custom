#include <Automaton.h>
#include "Atm_RF24Network.h"

Atm_RF24Network & Atm_RF24Network::begin(uint16_t node_addr)
{
  const static state_t state_table[] PROGMEM = {
  /*                  ON_ENTER      ON_LOOP  ON_EXIT   EVT_NEW_MSG     EVT_SEND_MSG  EVT_PING     ELSE */
  /* IDLE  */         -1,    UPDATE_NETWORK,   -1,      RECV_MSG,       SEND_MSG,      PING,        -1,
  /* RECIEVING */  RECV_MSG,          -1,      -1,        -1,           -1,            -1,          IDLE,
  /* SENDING */    SEND_MSG,          -1,      -1,        -1,           -1,            -1,          IDLE,
  /* PINGING */    PING,              -1,      -1,        -1,           -1,            -1,          IDLE,

   };
   // begin rf24network, with node address param
  _network.begin(node_addr);
  
  // msgqueue for triggering the SEND event
  Machine::msgQueue( messages, MSG_END, 1 );
  
  // begin machine
  Machine::begin( state_table, ELSE );


  // 10 second ping timer, will ping master every 10 seconds
  // make this more configurable? 
  ping_timer.begin( this, 10000); 

  return *this; 
}

// register msg handler callback
Atm_RF24Network & Atm_RF24Network::onMsg( msgcb_t msg_callback ) 
{
  callback = msg_callback;
  return *this;  
}

// register ping callback
Atm_RF24Network & Atm_RF24Network::onPing( pingcb_t ping_callback ) 
{
  ping_cb = ping_callback;
  return *this;  
}

int Atm_RF24Network::event(int id)
{
   switch ( id )
   {  
      // if network is available, got a new message
      case EVT_NEW_MSG :
        return _network.available();
      // if we have a SEND message in the queue
      case EVT_SEND_MSG :
        return msgRead( SEND );
      // when our ping timer has expired
      case EVT_PING :
        return ping_timer.expired();
   }
   // no events
   return 0;
}

bool Atm_RF24Network::send(char* payload, size_t payload_len )
{
  this->state(this->SENDING);
  RF24NetworkHeader header(00, 1);
  return _network.write(header, payload, payload_len);
}

void Atm_RF24Network::action(int id)
{
 switch ( id ) {
    // keep network running, needs to be run in loop, and regularly
    case UPDATE_NETWORK :
     _network.update();
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
        size_t full_len = _network.read(header, &payload, sizeof(payload));
        payload[full_len] = '\0';
        Serial.println(payload);
      }
      return;
    }
    // 
    case SEND_MSG :
      // read data from a global buffer and pass it to network?
      Serial.println("SEND_MSG Action");
      return;

    case PING :
     Serial.println("SEND_PING Action");
      if(ping_cb) { (*ping_cb)( ); }
      return;
   }
}

Atm_RF24Network & Atm_RF24Network::onSwitch(swcb_sym_t switch_callback)
{
  Machine::onSwitch( switch_callback, "IDLE\0RECIEVING\0SENDING\0PINGING", "EVT_NEW_MSG\0EVT_SEND_MSG\0EVT_PING\0ELSE" );
  return *this;
}
