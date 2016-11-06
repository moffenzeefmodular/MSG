/*
 
      Mini Søund Generatør 
      Møffenzeef Mødular 
        Røss Fish 2016 
  http://moffenzeefmodular.com
         CC-BY-NC-SA 
 
 Based on "Generating a tone using DDS"
      By: David Johnson-Davies 
 http://www.technoblogy.com/show?QVN
          CC BY 4.0

*/

signed char Mask, Temp, Sum=0, LFO, LFO_depth, Final;
int p = 0; 
   
unsigned int Acc[] = {0, 0};
unsigned int Freqs[] = {0, 0};

int pot1Raw = 0;
int pitch = 0;

int pot2Raw = 0;
int modulationOne = 0;
int modulationTwo = 0;
int modulationThree = 0;

int pot3Raw = 0;
int LFO_rate = 0; 

int trigState = 0;

void setup() {

  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1<<PCKE | 1<<PLLE;     

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                     // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10;
  pinMode(1, OUTPUT);            // Enable PWM output pin

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 <<WGM00;             // Fast PWM
  TCCR0B = 1 <<WGM02 | 2<<CS00;   // 1/8 prescale
  TIMSK = 1 <<OCIE0A;             // Enable compare match, disable overflow

  OCR0A = 70; // clock speed 

}

void loop() { 
  
  trigState = digitalRead(0); // Read digital pin 0 as input
  pot1Raw = analogRead(A1); // Read Pin 1 as analog input
  pot2Raw = analogRead(A2); // Read pin 2 as analog input
  pot3Raw = analogRead(A3); // Read pin 3 as analog input

  pitch = map(pot2Raw, 0, 1023, 1, 127); 
   
  LFO_depth = map(pot1Raw, 0, 1023, 1, 127); 

  Freqs[0] = pitch; 

  if (trigState == HIGH) {
  LFO_rate = map(pot3Raw, 0, 1023, 1, 100); // LFO settings  
  } 
  else { 
  LFO_rate = map(pot3Raw, 0, 1023, 100, 3000); // Audio Rate settings 
  }

  Freqs[1] = LFO_rate;

}

ISR(TIMER0_COMPA_vect) {

     Acc[0] = Acc[0] + Freqs[0] * LFO + 400;
     Temp = Acc[0] >> 8;
     Mask = Temp >> 15;
     Sum = Temp ^ Mask;

     Acc[1] = Acc[1] + Freqs[1];
     Temp = Acc[1] >> 8; 
     Mask = Temp >> 15; 
     LFO = Temp ^ Mask; 

     LFO = min(LFO, LFO_depth);
  
  OCR1A = Sum; // 
}

