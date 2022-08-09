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
byte const alphaArr[] = {0x77,  0x7C,  0x39,  0x5E  , 0x79  , 0x71  , 0x3D  , 0x74  , 0x30  , 0x0E  , 0x70  , 0x38  , 0x15  , 0x54  , 0x5C  , 0x73  , 0x67  , 0x50  , 0x6D  , 0x78  , 0x3E  , 0x1C  , 0x2A  , 0x52  , 0x6E  , 0x49,};

//byte const seg7Array[] = {0x01, 0x02, 0x04, 0x08, 0x00,0x10, 0x20, 0x40, 0x80,0xff,};
byte const segmentPins[] = {13, 10, 8, 12, 7, 9, 11}; //{a,b,c,d,e.f,g} // a lits first
byte const digitPins[] = {5, 3, 4, 2}; //{d0, d1, d2, d3} // d0 lits first
byte num2display[] = {8, 8, 8, 8};//{d0, d1, d2, d3}
bool colon = false;

byte state = 0; // state tick
int pinColon = 6;
int D1 = 2;
int D2 = 3;
int D3 = 4;
int D4 = 5;

int set = A0;
int inc = A1;
byte hr = 12, mn = 0, mh = 3, dy = 17;
int yr = 2022;

// time change features
int loopTicks = 0; // update every loop


//---------Real Time Clock----------
#include <Wire.h>
#include <RTClib.h>
//DS1307 rtc;
RTC_DS1307 rtc;

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
    //rtc.adjust(DateTime(__DATE__, __TIME__)); //compile time date-time
    // January 21, 2014 at 3am you would call:

  }

  pinMode(set, INPUT_PULLUP);
  pinMode(inc, INPUT_PULLUP);

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

  //rtc.adjust(DateTime(2022, 8, 8, 16, 25, 0));
}

void loop() {
  byte tempByte = 0;
  loopTicks++;

  DateTime now;
  now = rtc.now(); //get time from RTC


  if (loopTicks >= 20) {
    loopTicks = 0;
    boolean minChanged = false;

    //colon = ~colon;
    switch (state)
    {

      case 0:// idle or normal state
        if (!digitalRead(set)) state = state + 2;
        now = rtc.now(); //get time from RTC
        tempByte = now.minute();
        num2display[3] = digArr[tempByte % 10];
        num2display[2] = digArr[tempByte / 10];
        tempByte = now.twelveHour();
        num2display[1] = digArr[tempByte % 10];
        num2display[0] = digArr[tempByte / 10];

        tempByte = now.second();
        colon = tempByte & 0x01;//colon changes value every lsb change of sec
        mn = now.minute();
        hr = now.hour();
        yr = now.year();
        mh = now.month();
        dy = now.day();
        //digitalWrite(pinColon, colon);
        break;
      case 1:// debounce1
        if (!digitalRead(set)) state++;
        else state = 0;
        break;
      case 2:// debounce 2
        if (!digitalRead(set)) state = state + 1;
        else state = 0;
        break;

      case 3://change date
        if (digitalRead(set))
          state = state + 1;
        break;
      case 4://change date
        if (!digitalRead(inc)) {
          dy++;
          if (dy >= 32) dy = 0;
        }
        if (!digitalRead(set))
          state = state + 1;
        num2display[3] = alphaArr['a' - 'a']; //d
        num2display[2] = alphaArr['d' - 'a']; //y
        num2display[1] = digArr[dy % 10];
        num2display[0] = digArr[dy / 10];
        delay(100);
        break;

      case 5://change date
        if (digitalRead(set))
          state = state + 1;
        break;
      case 6://change month
        if (!digitalRead(inc)) {
          mh++;
          if (mh >= 13) mh = 1;
        }
        if (!digitalRead(set)) state++;
        num2display[3] = digArr[mh % 10];
        num2display[2] = digArr[mh / 10];
        num2display[1] = alphaArr['t' - 'a'];
        num2display[0] = alphaArr['m' - 'a'];
        delay(100);
        break;
      case 7://change date
        if (digitalRead(set))
          state = state + 1;
        break;

      case 8://change year
        if (!digitalRead(inc)) {
          yr++;
          if (yr<2000 or yr>2099) yr = 2000;
        }
        if (!digitalRead(set)) state++;
    
        num2display[3] = digArr[yr%10];
        tempByte=yr/1000;

        num2display[2] = digArr[tempByte%10];
    num2display[1] = alphaArr['r'-'a'];
    num2display[0] = alphaArr['y'-'a'];

            delay(100);
    
    /*
        n = q;

        q = n / 10;
        r = n % 10;
        num2display[1] = digArr[r];
        n = q;

        q = n / 10;
        r = n % 10;
        num2display[0] = digArr[r];
        
*/
        /*
          // send to rtc
          Serial.println();

          num2display[0] = digArr[yr / 1000];//2023 -> 3
          Serial.print(digArr[tempByte] + 0x30);
          tempByte = yr / 100;// 2023/10=202
          num2display[1] = digArr[tempByte] % 10;//202 % 10 =2
          Serial.print(digArr[tempByte] + 0x30);
          tempByte = tempByte / 10; //tb = 202/10=20
          num2display[2] = digArr[tempByte] % 10;//0
          Serial.print(digArr[tempByte] + 0x30);
          tempByte = tempByte % 10;//2
          num2display[3] = digArr[tempByte] ;
          Serial.print(digArr[tempByte] + 0x30);
          Serial.println(); delay(100);
        */
        break;
      case 9://change date
        if (digitalRead(set))
          state = state + 1;
        break;
      case 10://change hours
        if (!digitalRead(inc)) {
          hr++;
          if (hr >= 23) hr = 0;
        }
        if (!digitalRead(set)) state++;
        num2display[3] = alphaArr['r' - 'a']; //h
        num2display[2] = alphaArr['h' - 'a']; //r
        num2display[1] = digArr[hr % 10];
        num2display[0] = digArr[hr / 10];
        delay(100);
        break;
      case 11://change date
        if (digitalRead(set))
          state = state + 1;
        break;
      case 12://change minutes
        if (!digitalRead(inc)) {
          minChanged = true;
          mn++;
          if (mn >= 60) mn = 0;
        }

        num2display[3] = digArr[mn % 10];
        num2display[2] = digArr[mn / 10];
        num2display[1] = alphaArr['n' - 'a']; //m
        num2display[0] = alphaArr['m' - 'a']; //n
        if (!digitalRead(set)) {
          state = 0;
          if (!minChanged)mn = now.minute();
          minChanged = false;
        }

        rtc.adjust(DateTime(yr, mh, dy, hr, mn, 0));
        //        rtc.adjust(DateTime(2023, 11, 1, 1, 30, 0));
        delay(100);
        break;
    }
    tempByte = now.second();
    
    Serial.println(String("") + hr + ":" + mn + "  " + dy + "-" + mh + "-" + yr + " state" + state);
  }

  //--------------- Display on 7-Seg one digit at a time (More Brightness, but more segments -> less brightness)------------
  for (int n = 0; n <= 3; n++) {
    //    byte k = digArr[num2display[n]];
    byte k = num2display[n];
    for (int i = 0; i <= 6; i++)
    {
      digitalWrite(segmentPins[i], k & 1);
      k = k >> 1;
    }
    digitalWrite(digitPins[n], HIGH);
    delay(4); // wait for a second
    digitalWrite(digitPins[n], LOW);
  }
  digitalWrite(pinColon, colon);
  //--------------- Display on 7-Seg Ends-----------
}
