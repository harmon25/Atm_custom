#ifndef Atm_RF24Network_h
#define Atm_RF24Network_h
  
  #include <SPI.h>
  #include <RF24Network.h>
  #include <RF24.h>
  #include <Automaton.h>
  #include <RF24Network.h>

  // message handler callback
  typedef void (*msgcb_t)( );

  class Att_RF24Network: public TinyMachine {
 
    public:
      RF24& _radio; 
      RF24Network network; 
      //ATM States, Events, Actions, Messages
      enum { IDLE, RECIEVING} STATES;
      enum { EVT_NEW_MSG,  ELSE } EVENTS;
      enum { UPDATE_NETWORK, RECV_MSG} ACTIONS;
           
      // constructor, passes network by ref
      Att_RF24Network( RF24 &radio ): _radio(radio), network(radio), TinyMachine() {};
     
      // null ptr to be assigned message handler callback
      void (*callback)( ) = 0;
      // wrapper around network.write, sends to master node
      bool send(char* payload, size_t payload_len, uint8_t msg_type);
      // to register message handler callback
      Att_RF24Network & onMsg( msgcb_t msg_callback );
      // pass this nodes addr to start rf24network
      Att_RF24Network & begin(uint8_t radio_channel, uint16_t node_addr);
      // on switch for debugging
      //Att_RF24Network & onSwitch( swcb_sym_t switch_callback );
      // event handler
      int event( int id ); 
      // action handler
      void action( int id ); 

  };

#endif