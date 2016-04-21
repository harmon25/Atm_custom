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
      // store prev relay state while blinking, so after blink know to blink on or off.
      bool prev_relay_state;

      short on_blink = 3;
      short off_blink = 5;
      atm_timer_millis timer;
      atm_counter counter_on, counter_off;  
      
      Att_relay & begin( short relay_pin, short l_pin);

      Att_relay & setBlink( short repeat, bool on_off ) ;

  
      int event( int id );
  
      void action( int id ); 
  
  };

#endif

