#include <dht11.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#define SECONDS_TO_RESTART 3

dht11 DHT_11;

OneWire oneWire(4);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();

  DDRB |= 1 << PB5;
  PORTB &= ~(1 << PB5);

  TCCR1A = 0;
  TCCR1B = (1 << WGM12);
  OCR1A = 15625;
  TIMSK1 = (1 << OCIE1A);

  sei();

  TCCR1B |= (1 << CS12) | (1 << CS10);

}

void getData();

volatile byte seconds = 1;
byte seconds_old = 0;
bool data_step = true;
float tem1 = 0;
int tem2 = 0;
int hum = 0;

volatile bool watchdog = true;
volatile char to_reset = SECONDS_TO_RESTART;

void loop() {

  if (seconds_old != seconds)
  {
    getData();
    seconds_old = seconds;
  }
  
  if (seconds > 0)
  {   
    Serial.print("{");
    Serial.print(tem1);
    Serial.print(";");
    Serial.print(tem2);
    Serial.print(";");
    Serial.print(hum);
    Serial.println("}");
    
    seconds = 0;
  }

  watchdog = true;
}

ISR(TIMER1_COMPA_vect)
{
   PORTB ^= (1 << PB5);
   ++seconds;

   if (watchdog)
   {
    watchdog = false;
    to_reset = SECONDS_TO_RESTART;
   }
   else
   {
    if (to_reset < 0)
    {
      wdt_enable(WDTO_15MS);

      for(;;)
      {
        
      }
    }
    
    --to_reset;
   }
}

void getData()
{ 
  sensors.requestTemperatures();
  
  if (data_step)
  {
    DHT_11.read(7);
    tem2 = DHT_11.temperature;
    hum = DHT_11.humidity;
  }

  data_step = !data_step;
  tem1 = sensors.getTempCByIndex(0);
}

