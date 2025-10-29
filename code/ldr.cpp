#include "ldr.h"
#include "Arduino.h"
#include "main.h"

void init_ldr(void)
{
   pinMode(GARDEN_LIGHT, OUTPUT);
   
}
unsigned int Light_intensity,duty;

void brightness_control(void)
{
   //read light intensity from LDR sensor
  Light_intensity = analogRead(LDR_SENSOR); // 0 to 1023

//scale down the light intensity or map the values from 1 to 1023 to 225 to 8
duty = 255 - (Light_intensity / 4);

// control brightness of the led
analogWrite(GARDEN_LIGHT, duty);

delay(100);
}
