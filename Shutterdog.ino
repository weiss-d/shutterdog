#include <eRCaGuy_Timer2_Counter.h>

// Pins which are both connected to the output of LM393 comparator
// one for processing the leading edge of pulse, which duration is to be measured
// another - for processing the trailing edge
const uint8_t LM393_1 = 2;
const uint8_t LM393_2 = 3;

// triggers of pulse detection
bool got_rising = false;
bool got_falling = false;

// volatile variables for processing inside the ISRs
volatile unsigned long t_start = 0; // time of leading edge arrival
volatile unsigned long t_end = 0;   // time of trailing edge arrival


void setup()
{
	timer2.setup();

	pinMode( LED_BUILTIN, OUTPUT );
	pinMode( LM393_1,  INPUT );
	pinMode( LM393_2,  INPUT );
	attachInterrupt( digitalPinToInterrupt( LM393_1 ), isrRising, RISING );
	attachInterrupt( digitalPinToInterrupt( LM393_2 ), isrFalling, FALLING );
	
	Serial.begin( 9600 );
	while (!Serial) {};
	Serial.println( "Ready for measurement." );

}

void loop()
{

  if ( got_rising && got_falling )
  {
	  Serial.print( ( t_end-t_start )/2000.0 ); Serial.println( "ms" );

	  got_rising = false;
	  got_falling = false;
	  t_start = 0;
	  t_end = 0;
  }

}


// ISR for processing pulse leading edge 
void isrRising()
{
	t_start = timer2.get_count();
	got_rising = true;
}

// ISR for processing pulse trailing edge 
void isrFalling()
{
	t_end = timer2.get_count();
	got_falling = true;
}