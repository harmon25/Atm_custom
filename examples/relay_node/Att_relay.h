#ifndef Atm_relay_h
  #define Atm_relay_h
  #include <Automaton.h>
  #include <Atm_led.h>

  class Att_relay : public TinyMachine {
  
    public:
      Att_relay( void ) : TinyMachine() {};
  
      short relay_pin;
      short led_pin;
          
      enum {OFF, ON} STATES;
      enum {EVT_TOGGLE, ELSE } EVENTS;
      enum {ACT_OFF, ACT_ON, CYCLE_LED } ACTIONS;

      Att_led relay_led;
      
      Att_relay & begin( short relay_pin, short l_pin);
  
  
      int event( int id );
  
      void action( int id ); 
  
  };

#endif

