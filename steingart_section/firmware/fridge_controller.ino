// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_MAX31855.h>
#include <math.h>

char publishString[200]; //a place holer for the publish string
int state = 0;

//Pin Defintion for SPI Interface MAX31855K
int thermoCLK = D2;
int thermoCS  = D1;
int thermoDO  = D0;


//For thermostat settings
double set_point = 0;
bool thermostat_mode = false; 

//Instantiate the Thermocouple Reader
Adafruit_MAX31855 TC(thermoCLK, thermoCS, thermoDO);


//Define Variables we'll be connecting to
unsigned long sendTime;


void setup() {
    Particle.function("set_state", set_state);
    Particle.function("set_temp",  set_temp);
    Particle.variable("set_point",set_point);
    sendTime = millis();
    for (int i = 3; i<8;i++) pinMode(i,OUTPUT);
    RGB.control(true);
    RGB.color(255,200,50);

}


void loop()
{
    if (millis() - sendTime > 1000)
    {
      digitalWrite(7,HIGH);
      float TI = TC.readInternal();
      float TO = TC.readCelsius();
      sprintf(publishString,"{\"state\":%d,\"TI\":%f,\"TO\":%f,\"set_point\":%f,\"thermostat\":%d}",state,TI,TO,set_point,thermostat_mode);
      Particle.publish("MAE_519_LAB_1",publishString);
      sendTime = millis();
      delay(50);
      digitalWrite(7,LOW);
      if (thermostat_mode == true) thermostat_control();
      
    }
}


void thermostat_control()
{
    //code here for thermostat
    
}


void off()
{
    for (int i = 3; i < 7; i++) digitalWrite(i,LOW);
    RGB.color(255,200,50);
    delay(200);
}

void hot()
{
    off();
    digitalWrite(3,HIGH);
    digitalWrite(5,HIGH);
    RGB.color(255,0,0);

}

void cold()
{
    off();
    digitalWrite(4,HIGH);
    digitalWrite(6,HIGH);
    RGB.color(0,0,255);

}

void stater(int i)
{
    if (i == 1)    hot();
    else if (i==2) cold();
    else           off();
    state = i;
}


int set_state(String hotcold)
{
  //turn thermostat stetting off
  thermostat_mode = false;
  
  
  //just run hot or cold
  stater(hotcold.toInt());
  return hotcold.toInt();
}

int set_temp(String potter)
{
  stater(0); //turn fridge off
  
  set_point = potter.toFloat();
  thermostat_mode = true; //turn thermostat mode on;
  return potter.toInt();
}

