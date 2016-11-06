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

unsigned int Acc;
unsigned int Offset = 400; 

int wave = 0;
int scale = 0; 

int pot1Raw = 0;
int pitch = 0;

int pot2Raw = 0;
int fuckery = 0; 

int pot3Raw = 0;
int bitShift = 0;

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
}

void loop() { 
  
  trigState = digitalRead(0); // Read digital pin 0 as input
  pot1Raw = analogRead(A1); // Read Pin 1 as analog input
  pot2Raw = analogRead(A2); // Read pin 2 as analog input
  pot3Raw = analogRead(A3); // Read pin 3 as analog input
  
  fuckery = map(pot1Raw, 1, 1023, 1, 32);
  
  pitch = map(pot2Raw, 1, 1023, 255, 1); 
  OCR0A = pitch;
    
  bitShift = map(pot3Raw, 1, 1023, 8, 1); 


  }

ISR(TIMER0_COMPA_vect) {

  signed char Temp, Mask;
  Acc = Acc + Offset;

  if (trigState == HIGH) 
  {
     Temp = Acc >> bitShift;
     Mask = Temp >> 7; 
     wave = Temp ^ Mask; // tri 
  }
  else 
  {
     wave = Acc >> bitShift; // Saw 
   //wave = (Acc >> 8) & 0x80; // Square
  }

  scale = (wave * fuckery);
  scale = max(scale, 0); 
  scale = min(scale, 255);

  OCR1A = scale;
}
