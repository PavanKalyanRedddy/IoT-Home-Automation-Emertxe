#include "temperature_system.h"
#include "Arduino.h"
#include "main.h"

float temperature;

void init_temperature_system(void)
{
  //setting heater and cooler GPIO pins as output pins
  pinMode(HEATER, OUTPUT);
  pinMode(COOLER, OUTPUT); 
}    


float read_temperature(void)
{
   //read the temperature sensor return the temperature
   temperature = (((analogRead(A0) * (float)5/1024)) / (float) 0.01);
   return temperature;
}

void cooler_control(bool control)  //receiving the state of the button on mobile app
{
   if(control)
   {
      //turn on the cooler -> assign dig val 1 to cooler gpio pin
      digitalWrite(COOLER, 1);
      
   }
   else
   { //off
      digitalWrite(COOLER, 0);
      
   }
}
void heater_control(bool control)
{
    if(control)
   {
      //turn on the cooler -> assign dig val 1 to cooler gpio pin
      digitalWrite(HEATER, 1);
      
   }
   else
   { //off
      digitalWrite(HEATER, 0);
     
   }
}
