#include <Automaton.h>
#include "Att_relay.h"

Att_relay & Att_relay::begin( short r_pin, short l_pin)
    {
      const static tiny_state_t state_table[] PROGMEM = {
  /*                    ON_ENTER           ON_LOOP      ON_EXIT       EVT_TIMER   EVT_COUNTER_ON    EVT_COUNTER_OFF   EVT_TOGGLE       ELSE */
  /* OFF             */    ACT_LED_OFF,      -1,    ACT_RELAY_ON,        -1,           -1,           -1,           BLINK_ON,      -1, // relay OFF, led OFF
  /* ON              */    ACT_LED_ON,       -1,    ACT_RELAY_OFF,       -1,           -1,           -1,           BLINK_OFF,     -1, // relay ON, led ON
  /* BLINK_ON        */    ACT_LED_ON,       -1,        -1,          BLINK_OFF,        ON,            -1,          -1,            -1, // relay ON, LED BLINK
  /* BLINK_OFF       */    ACT_LED_OFF,      -1,        -1,          BLINK_ON,        -1,           OFF,           -1,            -1, // relay ON, LED BLINK

  };

      TinyMachine::begin( state_table, ELSE );
      relay_pin = r_pin; 
      led_pin = l_pin;
      counter_on.set( on_blink );
      counter_off.set( off_blink );
      timer.begin( this, 200 );    
      pinMode( relay_pin, OUTPUT );
      pinMode( led_pin, OUTPUT );

      // start with relay OFF
      digitalWrite( relay_pin, HIGH );
      return *this;          
  }

Att_relay & Att_relay::setBlink( short repeat, bool on_off  ) 
{
  if(on_off == 0){
    off_blink = repeat;
    counter_off.set( repeat );
  } else {
    on_blink = repeat;
    counter_on.set( repeat );
  }
  return *this;
}



int  Att_relay::event( int id ) 
    {
  switch ( id ) {
    case EVT_TIMER :
      return timer.expired();              
    case EVT_COUNTER_ON :
      return counter_on.expired();
    case EVT_COUNTER_OFF :
      return counter_off.expired();
  }
  return 0;
    }


void  Att_relay::action( int id )
    {
      switch ( id ) {
        case ACT_LED_OFF :
          digitalWrite( led_pin, LOW );
          if((state() == BLINK_OFF || state() == BLINK_ON) && prev_relay_state == 0){
             counter_on.decrement();
          } else if((state() == BLINK_OFF || state() == BLINK_ON) && prev_relay_state == 1) {
             counter_off.decrement();
          }  
          return;
        case ACT_LED_ON :
         digitalWrite( led_pin, HIGH );
          if((state() == BLINK_OFF || state() == BLINK_ON) && prev_relay_state == 0){
             counter_on.decrement();
          } else if((state() == BLINK_OFF || state() == BLINK_ON) && prev_relay_state == 1) {
             counter_off.decrement();
          }        
          return;
        case ACT_RELAY_ON :
          prev_relay_state = 0;
          counter_on.set(on_blink);
          counter_off.set(off_blink);       
          digitalWrite( relay_pin, LOW );
          return;
        case ACT_RELAY_OFF :
          prev_relay_state = 1;
          counter_on.set(on_blink);   
          counter_off.set(off_blink);  
          digitalWrite( relay_pin, HIGH );
          return;
       }
    }


