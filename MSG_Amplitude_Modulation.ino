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

signed char Mask, Temp, Sum=0, LFO;
int p = 0; 
   
unsigned int Acc[] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned int Freqs[] = {0, 0, 0, 0, 0, 0, 0, 0};

int pot1Raw = 0;
int pitch = 0;

int pot2Raw = 0;
int modulationOne = 0;
int modulationTwo = 0;
int modulationThree = 0;
int modulationFour = 0;  
int modulationFive = 0;
int modulationSix = 0;

int pot3Raw = 0;
int LFO_rate = 0; 
int LFO_depth = 0; 

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

  pitch = map(pot2Raw, 0, 1023, 80, 3000); 
   
  modulationOne = map(pot1Raw, 0, 1023, 0, 50); 
  modulationThree = map(pot1Raw, 0, 1023, 0, 100); 
  modulationFive = map(pot1Raw, 0, 1023, 0, 400); 

  Freqs[0] = pitch; 
  Freqs[1] = pitch + modulationOne; 
  Freqs[2] = pitch + modulationTwo;
  Freqs[3] = pitch + modulationThree;
  Freqs[4] = pitch - modulationOne; 
  Freqs[5] = pitch - modulationTwo; 
  Freqs[6] = pitch - modulationThree;

  if (trigState == HIGH) {
  LFO_rate = map(pot3Raw, 0, 1023, 1, 100); // LFO settings  
  } 
  else { 
  LFO_rate = map(pot3Raw, 0, 1023, 100, 3000); // Audio Rate settings 
  }

  Freqs[7] = LFO_rate;

}

ISR(TIMER0_COMPA_vect) {

   for (int c=0; c<6; c++) {
     Acc[c] = Acc[c] + Freqs[c];
     Temp = Acc[c] >> 8;
     Mask = Temp >> 15;
     Sum = Sum + ((char)(Temp ^ Mask)) >> 2;
   }    

     Acc[7] = Acc[7] + Freqs[7];
     Temp = Acc[7] >> 8; 
     Mask = Temp >> 15; 
     LFO = Temp ^ Mask; 
  
  OCR1A = (Sum * LFO) >> 5; // 
}

