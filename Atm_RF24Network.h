#ifndef Atm_RF24Network_h
#define Atm_RF24Network_h

  #include <Automaton.h>
  #include <RF24Network.h>

  // message handler callback
  typedef void (*msgcb_t)( );

  // ping callback
  typedef void (*pingcb_t)( );


  class Atm_RF24Network: public Machine {
    
    private:
    // reference global network object inside class
    RF24Network& _network; 

    public:
      //ATM States, Events, Actions, Messages
      enum { IDLE, RECIEVING, SENDING, PINGING } STATES;
      enum { EVT_NEW_MSG, EVT_SEND_MSG, EVT_PING, ELSE } EVENTS;
      enum { UPDATE_NETWORK, RECV_MSG, SEND_MSG, PING } ACTIONS;
      enum { SEND, MSG_END } MESSAGES;
      
      atm_msg_t messages[MSG_END];

      atm_timer_millis ping_timer;
      // constructor, passes network by ref
      Atm_RF24Network( RF24Network &net ): _network(net), Machine() { class_label = "NETWORK"; } ;

      //not sure if i want to use this, shared payload buff?
      char payloadBuf[144];
      
      // null ptr, to be assigned a ping callback  
      void (*ping_cb)( ) = 0;
      // null ptr to be assigned message handler callback
      void (*callback)( ) = 0;

      bool send(char* payload, size_t payload_len );

      // to register ping callback
      Atm_RF24Network & onPing( pingcb_t msg_callback );

      // to register message handler callback
      Atm_RF24Network & onMsg( msgcb_t msg_callback );
      
      // pass this nodes addr to start rf24network
      Atm_RF24Network & begin(uint16_t node_addr);
      // on switch for debugging
      Atm_RF24Network & onSwitch( swcb_sym_t switch_callback );
      // event handler
      int event( int id ); 
      // action handler
      void action( int id ); 

  };

#endif