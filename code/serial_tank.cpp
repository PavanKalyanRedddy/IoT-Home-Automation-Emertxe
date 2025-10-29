#include "serial_tank.h"
#include "Arduino.h"
#include "main.h"

unsigned int volume_tank;
unsigned char value_h, value_l;

void init_serial_tank(void) 
{
    Serial.begin(19200);
    Serial.write(0xFF); 
    Serial.write(0xFF);
    Serial.write(0xFF);   
}

unsigned int volume(void)
{
   Serial.write(VOLUME);
   while(!Serial.available());
   value_h = Serial.read();
   while(!Serial.available());
   value_l = Serial.read();
   volume_tank = value_h << 8 | value_l; 
   return volume_tank;
}
void enable_inlet(void)
{
    Serial.write(INLET_VALVE);
    Serial.write(ENABLE);
}  
void disable_inlet(void)
{
    Serial.write(INLET_VALVE);
    Serial.write(DISABLE);
}  
void enable_outlet(void)
{  
    Serial.write(OUTLET_VALVE);
    Serial.write(ENABLE);
}
void disable_outlet(void)
{  
    Serial.write(OUTLET_VALVE);
    Serial.write(DISABLE);
}
