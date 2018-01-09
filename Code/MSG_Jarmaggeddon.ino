/*
         Jarmaggedøn
      Møffenzeef Mødular
        Røss Fish 2017
  http://moffenzeefmodular.com
         CC-BY-NC-SA

  Based on "Audio Sample Player"
      By: David Johnson-Davies
  http://www.technoblogy.com/show?QBB
          CC BY 4.0
*/

#include <avr/pgmspace.h>

unsigned int Acc[] {0, 0};
unsigned int Note = 0;
unsigned int Note1 = 0;

int potRaw = 0;
int CVraw = 0;
int pitch = 0;
int modulate = 0;
int math = 0;

void setup() {
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1 << PCKE | 1 << PLLE;

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                              // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10; // PWM A, clear on match, 1:1 prescale

  // Set up Timer/Counter0 for 8kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;                    // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00;        // 1/8 prescale
  TIMSK = 1 << OCIE0A;                    // Enable compare match

  pinMode(1, OUTPUT);
  pinMode(0, OUTPUT);
}

void loop() {

  potRaw = analogRead(A2);

  Note = map(potRaw, 0, 1023, 60000, 10);
  Note1 = map(potRaw, 0, 1023, 100, -100);
  modulate = map(potRaw, 0, 1023, 32, -32);

  OCR0A = map(potRaw, 0, 1023, 255, 20);
  
}

// Sample interrupt
ISR(TIMER0_COMPA_vect) {
  Acc[0] = Acc[0] + Note;
  Acc[1] = Acc[1] + Note1;
  OCR1A = ((((char)Acc[0] ^ Acc[1]) >> 8 & 0x80) ^ modulate); //
}
