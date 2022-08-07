/*
      A
     ---
  F |   | B
    | G |
     ---
  E |   | C
    |   |
     ---
      D
*/
//---------------- 7-Segment-Display------
byte const digArr[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
//byte const seg7Array[] = {0x01, 0x02, 0x04, 0x08, 0x00,0x10, 0x20, 0x40, 0x80,0xff,};
byte const segmentPins[] = {13, 12, 11, 10, 9, 8, 7};//{a,b,c,d,e.f,g}
byte const digitPins[] = {4, 3, 2, 5};//{}
byte num2display[] = {1, 1, 8, 8};//{d0, d1, d2, d3}
bool colon = false;

int pinColon = 6;
int pinA = 7;
int pinB = 8;
int pinC = 9;
int pinD = 10;
int pinE = 11;
int pinF = 12;
int pinG = 13;
int D1 = 2;
int D2 = 3;
int D3 = 4;
int D4 = 5;

//---------Real Time Clock----------
#include <Wire.h>
#include <RTClib.h>
//DS1307 rtc;
RTC_DS1307 rtc;
// buffer for DateTime.tostr
//char buf[20];
//-----------------------------------

// the setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);

#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(__DATE__, __TIME__)); //compile time date-time
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
 //delay(1000);
 
  // initialize the digital pins as outputs.
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(D1, OUTPUT);
    pinMode(pinColon, OUTPUT);

  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  for (int d = 0; d <= 3; d++) {
    digitalWrite(digitPins[d], LOW);
  }
}

void loop() {
  DateTime now = rtc.now();
  
      
  byte tempByte;
  tempByte = now.minute();
  Serial.print(tempByte, DEC);
      Serial.print(':');
  num2display[1] = tempByte/10;
  num2display[0] = tempByte % 10;
  
  tempByte = now.twelveHour();
  Serial.print(tempByte, DEC);
      Serial.print(':');
  num2display[3] = tempByte/10;
  num2display[2] = tempByte % 10;

  tempByte = now.second();
  Serial.print(tempByte, DEC);
      Serial.print(',');
colon = tempByte & 1;
  
  digitalWrite(pinColon, colon);
  Serial.println(colon, BIN);

  //--------------- Display on 7-Seg-----------
  for (int n = 0; n <= 3; n++) {
    digitalWrite(digitPins[n], HIGH);
    byte k = digArr[num2display[n]];
    for (int i = 0; i <= 6; i++)
    {
      digitalWrite(segmentPins[i], k & 1);
      k = k >> 1;
      delay(1); // wait for a second
      digitalWrite(segmentPins[i], LOW);
    }
    digitalWrite(digitPins[n], LOW);
  }
  //--------------- Display on 7-Seg Ends-----------
  int a =1;
}
