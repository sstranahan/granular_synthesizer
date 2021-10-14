/* Example code with timer intyerrutp that will create an interruption each
    500ms using timer1 and prescalar of 256.
  Calculations (for 500ms):
  System clock 16 Mhz and Prescalar 256;
  Timer 1 speed = 16Mhz/256 = 62.5 Khz
  Pulse time = 1/62.5 Khz =  16us
  Count up to = 500ms / 16us = 31250 (so this is the value the OCR register should have)*/

#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

#define DAC_RESOLUTION    (8)

#define B0 2
#define B1 3
#define B2 4
#define B3 5
#define B4 6
#define B5 7
#define B6 8
#define B7 9

const PROGMEM uint16_t FREQ_LOOKUP [8] {
  61154, 54482, 48538, 45814,
  40815, 36363, 32396, 30577
};

bool OUT_STATE = true;
bool setupFlag = true;


void setup() {

  Serial.begin(9600);
  Serial.println("Hello!");

  Serial.println("Setting Up I/O ...");
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  //  Serial.println("Initializing DAC ...");
  //  dac.begin(0x62);

  pinMode(13, OUTPUT);        //Set the pin to be OUTPUT
  cli();                      //stop interrupts for till we make the settings
  /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0
  TCCR1B = 0;                 // Reset entire TCCR1B to 0

  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */
  // TCCR1B |= B00000100;           // Set CS12 to 1 so we get prescalar 256
  TCCR1B = B00000001;                 // Prescaler = 1

  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000000;           //Set OCIE1A to 0 so we disable compare match A

  /*3. We enable compare match mode on register A*/
  // TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A

  /*4. Set the value of register A to 31250*/
  // OCR1A = 31250;              //Finally we set compare register A to this value
  // OCR1A = 156.25;             //Should be around 200Hz
  // OCR1A = 78.125;             // 400Hz
  // OCR1A = 19;
  // OCR1A = 60605;              // Interrupt freq = 254 Hz
  sei();                         //Enable back the interrupts
  Serial.println("Initialization complete ...");
}

void loop() {
  // put your main code here, to run repeatedly:

  cli();
  setupFlag = 1;

  while (digitalRead(B0) == LOW) {
    if (setupFlag) {
      cli();
      TIMSK1 |= B00000010;        // Set OCIE1A to 1 so we enable compare match A
      OCR1A = 61154;     // C4
      setupFlag = 0;
      sei();
    }
  }

  TIMSK1 |= B00000000;            // Disable OCC

  while (digitalRead(B1) == LOW) {
    if (setupFlag) {
      cli();
      TIMSK1 |= B00000010;        // Set OCIE1A to 1 so we enable compare match A
      OCR1A = FREQ_LOOKUP[1];     // D4
      setupFlag = 0;
      sei();
    }
  }

  TIMSK1 |= B00000000;            // Disable OCC

  while (digitalRead(B2) == LOW) {
    if (setupFlag) {
      cli();
      TIMSK1 |= B00000010;        // Set OCIE1A to 1 so we enable compare match A
      OCR1A = FREQ_LOOKUP[2];     // E4
      setupFlag = 0;
      sei();
    }
  }

  TIMSK1 |= B00000000;            // Disable OCC

  while (digitalRead(B3) == LOW) {
    if (setupFlag) {
      cli();
      TIMSK1 |= B00000010;        // Set OCIE1A to 1 so we enable compare match A
      OCR1A = FREQ_LOOKUP[3];     // F4
      setupFlag = 0;
      sei();
    }
  }

  TIMSK1 |= B00000000;            // Disable OCC

  while (digitalRead(B4) == LOW) {
    if (setupFlag) {
      cli();
      TIMSK1 |= B00000010;        // Set OCIE1A to 1 so we enable compare match A
      OCR1A = FREQ_LOOKUP[4];     // G4
      setupFlag = 0;
      sei();
    }
  }

  TIMSK1 |= B00000000;            // Disable OCC

  while (digitalRead(B5) == LOW) {
    if (setupFlag) {
      cli();
      TIMSK1 |= B00000010;        // Set OCIE1A to 1 so we enable compare match A
      OCR1A = FREQ_LOOKUP[5];     // A4
      setupFlag = 0;
      sei();
    }
  }

  TIMSK1 |= B00000000;            // Disable OCC

  while (digitalRead(B6) == LOW) {
    if (setupFlag) {
      cli();
      TIMSK1 |= B00000010;        // Set OCIE1A to 1 so we enable compare match A
      OCR1A = FREQ_LOOKUP[6];     // B4
      setupFlag = 0;
      sei();
    }
  }

  TIMSK1 |= B00000000;            // Disable OCC

  while (digitalRead(B7) == LOW) {
    if (setupFlag) {
      cli();
      TIMSK1 |= B00000010;        // Set OCIE1A to 1 so we enable compare match A
      OCR1A = FREQ_LOOKUP[7];     // C5
      setupFlag = 0;
      sei();
    }
  }

  TIMSK1 |= B00000000;            // Disable OCC

}

//With the settings above, this IRS will trigger each 500ms.
ISR(TIMER1_COMPA_vect) {

  TCNT1 = 0;                  //First, set the timer back to 0 so it resets for next interrupt

  OUT_STATE = !OUT_STATE;      //Invert output state

  digitalWrite(13, OUT_STATE); //Write new state to the speaker on pin D5

  if (digitalRead(3) == HIGH && digitalRead(2) == HIGH) {
    TIMSK1 |= B00000000;
  }

}
