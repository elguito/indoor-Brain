//ARDUINO Indoor Gardening control.

#define ledPin_1_PIN LED_BUILTIN
#include <TimerOne.h>
#include "TM1637.h"
#define ON 1
#define OFF 0

int8_t TimeDisp[] = {0x00, 0x00, 0x00, 0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char halfsecond = 0;
unsigned char second = 10;
unsigned char minute = 5;
unsigned char hour = 16;


#define CLK 3//pins definitions for TM1637 and can be changed to other ports    
#define DIO 2
TM1637 tm1637(CLK, DIO);


class Led {
  private:
    byte pin;
  public:
    Led(byte pin) {
      // Use 'this->' to make the difference between the
      // 'pin' attribute of the class and the 
      // local variable 'pin' created from the parameter.
      this->pin = pin;
      init();
    }
    void init() {
      pinMode(pin, OUTPUT);
      // Always try to avoid duplicate code.
      // Instead of writing digitalWrite(pin, LOW) here,
      // call the function off() which already does that
      off();
    }
    void on() {
      digitalWrite(pin, HIGH);
    }
    void off() {
      digitalWrite(pin, LOW);
    }
}; // don't forget the semicolon at the end of the class
Led rele1(13);


void setup()
{

  Serial.begin(9600);
  //Toda esta seccion es para setear le inicializar la libreria del display
  tm1637.set();
  tm1637.init();

  //Inicializa el timer en la libreria TimerOne
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR

  //Set de Outputs e inputs

  //El Led
  //pinMode(LED_BUILTIN, OUTPUT);


}
void loop()
{
  if (Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);

  }
     rele1.on();
    delay(100);
    rele1.off();
    delay(100);
}



void TimingISR()
{
  halfsecond ++;
  Update = ON;
  if (halfsecond == 2) {
    second ++;
    if (second == 60)
    {
      minute ++;
      if (minute == 60)
      {
        hour ++;
        if (hour == 24)hour = 0;
        minute = 0;
      }
      second = 0;
    }
    halfsecond = 0;
  }
  // Serial.println(second);
  ClockPoint = (~ClockPoint) & 0x01;



}

void TimeUpdate(void)
{
  if (ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF);
  TimeDisp[0] = minute / 10;
  TimeDisp[1] = minute % 10;
  if (second / 10 == 6) {
    TimeDisp[2] = 0;
  }
  else {
    TimeDisp[2] = second / 10;

  }
  TimeDisp[3] = second % 10;
  Update = OFF;
}
