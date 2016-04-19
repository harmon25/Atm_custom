#ifndef Atm_relay_h
  #define Atm_relay_h
  #include <Automaton.h>
  #include <Atm_led.h>

  class Atm_relay : public Machine {
  
    public:
      Atm_relay( void ) : Machine() { class_label = "RELAY"; };
  
      short relay_pin;
      short led_pin;
          
      enum {ON, OFF} STATES;
      enum {EVT_TOGGLE, ELSE } EVENTS;
      enum {ACT_OFF, ACT_ON, CYCLE_LED } ACTIONS;

      Atm_led relay_led;
      
      Atm_relay & begin( short relay_pin, short l_pin);
      Atm_relay & onSwitch( swcb_sym_t switch_callback );
  
      int event( int id );
  
      void action( int id ); 
  
  };

#endif

