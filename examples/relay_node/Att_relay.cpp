#include <Automaton.h>
#include "Att_relay.h"

Att_relay & Att_relay::begin( short r_pin, short l_pin)
    {
      const static tiny_state_t state_table[] PROGMEM = {
      /*            ON_ENTER    ON_LOOP    ON_EXIT    EVT_TOGGLE     ELSE */
      /* OFF  */    ACT_OFF,     CYCLE_LED,         -1,       ON,     -1,
      /* ON   */    ACT_ON,      CYCLE_LED,         -1,       OFF,    -1,


      };
      TinyMachine::begin( state_table, ELSE );
      relay_pin = r_pin; 
      led_pin = l_pin;
      pinMode( relay_pin, OUTPUT );
      return *this;          
    }

int  Att_relay::event( int id ) 
    {
    // events all triggered by .trigger
      return 0;
    }

void  Att_relay::action( int id )
    {
      switch ( id ) {
        case ACT_ON :
          digitalWrite( relay_pin, LOW );
          // blink twice before turning on
          relay_led.begin( led_pin ).repeat( 4 ).blink( 200 ).pause( 100 ).trigger( relay_led.EVT_BLINK );
          while ( relay_led.cycle().state() ) {};
          //turn led ON
          relay_led.trigger(relay_led.EVT_ON);
          return;
        case ACT_OFF :
          // blink led blink 3 times before turning off
          digitalWrite( relay_pin, HIGH );
          // trigger blink
          relay_led.begin( led_pin ).repeat( 5 ).blink( 200 ).pause( 100 ).trigger( relay_led.EVT_BLINK );
          while ( relay_led.cycle().state() ) { };
          // turn led OFF
          relay_led.trigger(relay_led.EVT_OFF);
          return;
        case CYCLE_LED :
          // keep led cycling in loop. 
          relay_led.cycle();
          return;
       }
    }


