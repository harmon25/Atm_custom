#include <Automaton.h>
#include "Atm_relay.h"

Atm_relay & Atm_relay::begin( short r_pin, short l_pin)
    {
      const static state_t state_table[] PROGMEM = {
      /*            ON_ENTER    ON_LOOP    ON_EXIT    EVT_TOGGLE     ELSE */
      /* ON   */    ACT_ON,      CYCLE_LED,         -1,       OFF,    -1,
      /* OFF  */    ACT_OFF,     CYCLE_LED,         -1,       ON,     -1,

      };
      Machine::begin( state_table, ELSE );
      relay_pin = r_pin; 
      led_pin = l_pin;
      pinMode( relay_pin, OUTPUT );
      return *this;          
    }

int  Atm_relay::event( int id ) 
    {
    // events all triggered by .trigger
      return 0;
    }

void  Atm_relay::action( int id )
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

Atm_relay & Atm_relay::onSwitch( swcb_sym_t switch_callback ) {
  Machine::onSwitch( switch_callback, "OFF\0ON", "EVT_TOGGLE\0EVT_ON\0EVT_OFF\0ELSE" );
  return *this;
}

