#include "Wire.h"
#include "EveryTimerB.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) ///< Bit-value if not defined by Arduino
#endif

//Some settings
uint8_t sw_version = 1;
uint8_t brightness = 15;
uint8_t blinkrate = 0;
long baudrate = 9600;

// This is the H600 timer state machine
unsigned int secs = 0;
bool h600running = 0;
bool timeractive = 0;

// These are old values of the state machine for debugging purposes
bool old_h600running = 0;
bool old_timeractive = 0;


// Set 0 to disable the relay for testing purposes
bool relayactive = 1;

// 7-segment display 
uint16_t volatile displaybuffer[8];
int volatile isready = 0;

// Grey Code variables
bool laston2 = 0;
bool laston3 = 0;
int state1 = 0;
int state2 = 0;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  pinMode(4, INPUT);
  pinMode(17, OUTPUT);

  Serial.begin(baudrate);
  Serial.print("Baudrate: ");
  Serial.println(baudrate);
  Serial.println("********************************************");
  Serial.println("Hobart H600 Timer");
  Serial.print("Version ");
  Serial.println(sw_version);
  Serial.println("2022-07-28 by Sebastian Pape");
  Serial.println("sebastian@die-knoedelwerkstatt.de");
  Serial.println("********************************************");

  startDisplay();

  TimerB2.initialize();
  TimerB2.attachInterrupt(countdown);
  TimerB2.setPeriod(1000000);

}

void countdown() {
  if (h600running == 1 && secs > 0) {
    secs--;
    printSeconds();
  }
}



void loop() {
  checkH600Running();

  // Rotary Encoder Verarbeitung
  bool curon2 = digitalRead(2);
  bool curon3 = digitalRead(3);
  if ((curon2 != laston2) != (curon3 != laston3)) {
    laston2 = curon2;
    laston3 = curon3;
    int curstate = curon3 + 2 * curon2;
    if (curstate == 3 && state1 == 2 && state2 == 0) {
      goRight();
    }
    if (curstate == 3 && state1 == 1 && state2 == 0 && secs > 0) {
      goLeft();
    }
    state2 = state1;
    state1 = curstate;
  }

  if (isready == 1) {
    writeToDisplay();
    Serial.println("Display: WRITE");
  }

  if(old_timeractive != timeractive) {
    Serial.print("Timeractive: ");
    Serial.println(timeractive);
    old_timeractive = timeractive;
  }

  if(old_h600running != h600running) {
    Serial.print("H600Running: ");
    Serial.println(h600running);
    old_h600running = h600running;
  }
}

void goLeft() {
  if (secs <= 15) {
    secs = 0;
    printHold();
    timeractive = 0;
  } else {
    secs = secs - 15;
    printSeconds();
    Serial.println("Encoder: LEFT");
    Serial.print("Seconds: ");
    Serial.println(secs);
  }
}

void goRight() {
  timeractive = 1;
  secs = secs + 15;
  if(secs >= 3600) {
    secs = 0;
    printHold();
    timeractive = 0;
  } else {
    printSeconds();
    Serial.println("Encoder: RIGHT");
    Serial.print("Seconds: ");
    Serial.println(secs);
  }
}

void checkH600Running() {
  if (digitalRead(4) == HIGH) {
    h600running = 1;
    if (timeractive == 1 && secs == 0) {
      printSeconds();
      writeToDisplay();
      stoph600();
      Serial.println("Timer: END");
    }
  } else {
    h600running = 0;
  }
}

/*
 * Stop the H600 by pulling the relay and releasing it a second later
 * This makes the contactor stop the motor.
 */
 
void stoph600() {
  timeractive = 0;
  Serial.println("Relay: PULL");
  if (relayactive) digitalWrite(17, HIGH);
  delay(1000);
  Serial.println("Relay: RELEASE");
  if (relayactive) digitalWrite(17, LOW);
  printHold();
}


/*
 * 7-segment display methods
 * 
 *
*/

/*
 * Prepare the display, clear the memory and print the word "HOLd" to the display
 */
 
void startDisplay(void) {
  Wire.begin();
  Wire.beginTransmission(0x70);
  Wire.write(0x21);
  Serial.println("Sending 0x21, with length 1");
  int returncode = Wire.endTransmission();
  Serial.print("Return Code: ");
  Serial.println(returncode);

  setBrightness();
  setBlinkRate();
  
  clearDisplayBuffer();

  writeToDisplay();

  printH600();

  writeToDisplay();

  delay(1000);

  printHold();

  writeToDisplay();
}

/* 
 *  convert the displaybuffer[8] to a buffer
 *  for the i2c 7-segment display
 */
 
void writeToDisplay(void) {
  uint8_t buffer[17];

  buffer[0] = 0x00;

  for (uint8_t i = 0; i < 8; i++) {
    buffer[1 + 2 * i] = displaybuffer[i] & 0xFF;
    buffer[2 + 2 * i] = displaybuffer[i] >> 8;
  }

  Wire.beginTransmission(0x70);
  Wire.write(buffer, 17);
  int returncode = Wire.endTransmission();
  isready = 0;
  if (returncode != 0) {
    Serial.print("i2c Error Display: ");
    Serial.println(returncode);
  }
}

void clearDisplayBuffer() {
  for (uint8_t i = 0; i < 8; i++) {
    displaybuffer[i] = 0;
  }
}

/* 
 *  Print the word "HOLd" to the 7-digit display.
 */

void printHold() {
  clearDisplayBuffer();
  displaybuffer[0] = 0b01110110;
  displaybuffer[1] = 0b00111111;
  displaybuffer[3] = 0b00111000;
  displaybuffer[4] = 0b01011110;
  Serial.println("Display: HOLD");
  isready = 1;
}

void printH600() {
  displaybuffer[0] = 0b01110110;
  displaybuffer[1] = 0b01111101;
  displaybuffer[3] = 0b00111111;
  displaybuffer[4] = 0b00111111;
  Serial.println("Display: H600");
  isready = 1;
}


/* 
 *  Prepare the number of seconds for the display and convert it to
 *  an integer representing minutes and seconds
 */
 
void printSeconds() {
  printNumber(((secs / 60) * 100) + (secs % 60));
}


/*
 * Print any integer/number between 0 and 9999.
 * Display a colon between 100th and 10th (displaybuffer[2])
 * 
 * int number: the number to display
 */
 
void printNumber(int number) {
  if (number > 9999 || number < 0) {
    return;
  }
  clearDisplayBuffer();
  displaybuffer[0] = getDigit(number / 1000);
  displaybuffer[1] = getDigit((number % 1000) / 100);
  displaybuffer[2] = 0b00111111;
  displaybuffer[3] = getDigit((number % 100) / 10);
  displaybuffer[4] = getDigit((number % 10));
  isready = 1;
}

/*
 * Return the binary code for digits 0 - 9
 * 
 * int val: the digit to return the binary code for
 */
int getDigit(int val) {
  uint8_t digittable[] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
  };
  return digittable[val];
}

void setBrightness() {
  uint8_t buffer = 0xE0 | brightness;
  Wire.beginTransmission(0x70);
  Wire.write(buffer);
  Serial.print("Sending ");
  Serial.print(buffer);
  Serial.println(", with length 1");
  int returncode = Wire.endTransmission();
  Serial.print("Return Code: ");
  Serial.println(returncode);
}

void setBlinkRate() {
  uint8_t buffer = 0x80 | 0x01 | (blinkrate << 1);
  Wire.beginTransmission(0x70);
  Wire.write(buffer);
  Serial.print("Sending ");
  Serial.print(buffer);
  Serial.println(", with length 1");
  int returncode = Wire.endTransmission();
  Serial.print("Return Code: ");
  Serial.println(returncode);
}
