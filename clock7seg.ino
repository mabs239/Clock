/*
  Showing number 0-9 on a Common Anode 7-segment LED display
  Displays the numbers 0-9 on the display, with one second inbetween.
    A
   ---
  F |   | B
  | G |
   ---
  E |   | C
  |   |
   ---
    D
  This example code is in the public domain.
*/
//---------------- 7-Segment-Display------
byte const digArr[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
//byte const seg7Array[] = {0x01, 0x02, 0x04, 0x08, 0x00,0x10, 0x20, 0x40, 0x80,0xff,};
byte const segmentPins[] = {13, 10, 8, 12, 7, 9, 11}; //{a,b,c,d,e.f,g} // a lits first
byte const digitPins[] = {5, 4, 2, 3}; //{d0, d1, d2, d3} // d0 lits first
byte num2display[] = {8, 8, 8, 8};//{d0, d1, d2, d3}
bool colon = false;

int pinColon = 6;
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
  pinMode(pinColon, OUTPUT);
  for (int i = 0; i <= 6; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);
  }

  for (int i = 0; i <= 3; i++)
  {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], LOW);

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
  

  
    //--------------- Display on 7-Seg one digit at a time (More Brightness, but more segments -> less brightness)------------
    for (int n = 0; n <= 3; n++) {
      byte k = digArr[num2display[n]];
      for (int i = 0; i <= 6; i++)
      {
        digitalWrite(segmentPins[i], k & 1);
        k = k >> 1;
      }
      digitalWrite(digitPins[n], HIGH);
      delay(5); // wait for a second
      digitalWrite(digitPins[n], LOW);
    }
    //--------------- Display on 7-Seg Ends-----------
  

/*
  //--------------- Display on 7-Seg one segment at a time (Equal brightness but DIM)-----------
  for (int n = 0; n <= 3; n++) {
    digitalWrite(digitPins[n], HIGH);
    byte k = digArr[num2display[n]];
    for (int i = 0; i <= 6; i++)
    {
      digitalWrite(segmentPins[i], k & 1);
      k = k >> 1;
      delay(1); // wait for a second
      digitalWrite(segmentPins[i], LOW); // segment off
    }
    digitalWrite(digitPins[n], LOW);
  }
*/
}
