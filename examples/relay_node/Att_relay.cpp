#include <Automaton.h>
#include "Att_relay.h"

Att_relay & Att_relay::begin( short r_pin, short l_pin)
    {
      const static tiny_state_t state_table[] PROGMEM = {
  /*                    ON_ENTER           ON_LOOP      ON_EXIT       EVT_TIMER   EVT_COUNTER_ON    EVT_COUNTER_OFF   EVT_TOGGLE       ELSE */
  /* OFF             */    ACT_LED_OFF,      -1,    ACT_RELAY_ON,        -1,           -1,           -1,           BLINK_ON,        -1, // relay OFF, led OFF
  /* ON              */    ACT_LED_ON,       -1,    ACT_RELAY_OFF,       -1,           -1,           -1,           BLINK_OFF,       -1, // relay ON, led ON
  /* BLINK_ON        */    ACT_LED_ON,       -1,        -1,          BLINK_OFF,        ON,           -1,             ON,            -1, // relay ON, LED BLINK
  /* BLINK_OFF       */    ACT_LED_OFF,      -1,        -1,          BLINK_ON,         -1,            OFF,           OFF,            -1, // relay ON, LED BLINK

  };

      TinyMachine::begin( state_table, ELSE );
      relay_pin = r_pin; 
      led_pin = l_pin;
      repeat_on = 5;
      counter.set( repeat_on );
      timer.begin( this, 500 );    
      pinMode( relay_pin, OUTPUT );
      pinMode( led_pin, OUTPUT );

      // start with relay OFF
      digitalWrite( relay_pin, HIGH );
      return *this;          
    }

int  Att_relay::event( int id ) 
    {
  switch ( id ) {
    case EVT_TIMER :
      return timer.expired();              
    case EVT_COUNTER_ON :
      return counter.expired() && prev_relay_state == 0;
    case EVT_COUNTER_OFF :
      return counter.expired() && prev_relay_state == 1;
  }
  return 0;
    }


void  Att_relay::action( int id )
    {
      switch ( id ) {
        case ACT_LED_OFF :
          digitalWrite( led_pin, LOW );
          if(state() == BLINK_OFF) {
            counter.decrement();
          };
          return;
        case ACT_LED_ON :
         digitalWrite( led_pin, HIGH );
          if(state() == BLINK_ON){
             counter.decrement();
          };         
          return;
        case ACT_RELAY_ON :
          prev_relay_state = 0;
          counter.set(repeat_on);        
          digitalWrite( relay_pin, LOW );
          return;
        case ACT_RELAY_OFF :
          prev_relay_state = 1;
          counter.set(repeat_on);  
          digitalWrite( relay_pin, HIGH );
          return;
       }
    }


