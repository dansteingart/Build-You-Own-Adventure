// This #include statement was automatically added by the Particle IDE.
#include <math.h>
#include "Adafruit_MAX31855.h"

char publishString[200]; //a place holer for the publish string
int state = 0;

//Pin Defintion for SPI Interface MAX31855K
int thermoCLK = D2;
int thermoCS = D1;
int thermoDO = D0;

//Instantiate the Thermocouple Reader
Adafruit_MAX31855 TC(thermoCLK, thermoCS, thermoDO);


//Define Variables we'll be connecting to
unsigned long sendTime;


void setup() {
    Particle.function("set_state", set_state);
    sendTime = millis();
    for (int i = 3; i<7;i++) pinMode(i,OUTPUT);

}


void loop()
{
    if (millis() - sendTime > 1000)
    {

      float TI = TC.readInternal();
      float TO = TC.readCelsius();
      sprintf(publishString,"{\"state\":%d,\"TI\":%f,\"TO\":%f}",state,TI,TO);
      Particle.publish("MAE_519_LAB_1",publishString);
      sendTime = millis();
    }
}


void off()
{
    for (int i = 3; i < 7; i++) digitalWrite(i,LOW);
    delay(200);
}

void hot()
{
    off();
    digitalWrite(3,HIGH);
    digitalWrite(5,HIGH);
}

void cold()
{
    off();
    digitalWrite(4,HIGH);
    digitalWrite(6,HIGH);
}

void stater(int i)
{
    if (i == 1)    hot();
    else if (i==2) cold();
    else           off();
}

int set_state(String potter)
{
  stater(potter.toInt());
  return potter.toInt();
}
