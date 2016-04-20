#ifndef Atm_relay_h
  #define Atm_relay_h
  #include <Automaton.h>

  class Att_relay : public TinyMachine {
  
    public:
      Att_relay( void ) : TinyMachine() {};
  
      short relay_pin;
      short led_pin;
             
      enum { OFF, ON, BLINK_ON, BLINK_OFF } STATES; 
      enum { EVT_TIMER, EVT_COUNTER_ON,EVT_COUNTER_OFF, EVT_TOGGLE, ELSE } EVENTS; 
      enum { ACT_LED_OFF, ACT_LED_ON, ACT_RELAY_ON, ACT_RELAY_OFF } ACTIONS; 

      bool prev_relay_state;

      short repeat_on;
      atm_timer_millis timer; // for time led is on, off during blinking phase
      atm_counter counter;  // so on, off state can blink a different number of times...
      
      Att_relay & begin( short relay_pin, short l_pin);

  
      int event( int id );
  
      void action( int id ); 
  
  };

#endif

