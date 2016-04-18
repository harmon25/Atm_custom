#ifndef Atm_RF24Network_h
#define Atm_RF24Network_h
  
  #include <SPI.h>
  #include <RF24Network.h>
  #include <RF24.h>
  #include <Automaton.h>
  #include <RF24Network.h>

  // message handler callback
  typedef void (*msgcb_t)( );

  class Atm_RF24Network: public Machine {
    
    private:
    // reference global network object inside class
    RF24Network& _network; 

    public:
      //ATM States, Events, Actions, Messages
      enum { IDLE, RECIEVING} STATES;
      enum { EVT_NEW_MSG,  ELSE } EVENTS;
      enum { UPDATE_NETWORK, RECV_MSG} ACTIONS;
           
      // constructor, passes network by ref
      Atm_RF24Network( RF24Network &net ): _network(net), Machine() { class_label = "NETWORK"; } ;
     
      // null ptr to be assigned message handler callback
      void (*callback)( ) = 0;

      bool send(char* payload, size_t payload_len, uint8_t msg_type);

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