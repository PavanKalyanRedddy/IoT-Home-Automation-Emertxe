/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/



// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL3EDR-3V38"
#define BLYNK_TEMPLATE_NAME "Home automation"
#define BLYNK_AUTH_TOKEN "Lgbqd2p-MRwRn8_j2VzW3TXBlvrItrJ1"


/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/




// Comment this out to disable prints
//#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x27 for a 16 chars and 2 line display


#include "ldr.h"
#include "main.h"
#include "temperature_system.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "serial_tank.h"
//char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw, inlet_sw, outlet_sw, cooler_sw;

// creating objectcalled  timer to update temp on blynk at particular period
BlynkTimer timer;



// This function is called every time the Virtual Pin 0 state changes
//To turn ON and OFF cooler based virtual PIN value/
BLYNK_WRITE(COOLER_V_PIN) 
{
  //read the changed value on the pin
  cooler_sw = param.asInt();
  //based on the pin value control heater and cooler
  cooler_control(cooler_sw);
  if (cooler_sw) {
    lcd.setCursor(8, 0);
    lcd.print("CLR_ON ");

  } else {
    lcd.setCursor(8, 0);
    lcd.print("CLR_OFF");
  }
}
//To turn ON and OFF heater based virtual PIN value/
BLYNK_WRITE(HEATER_V_PIN) 
{
  //read the changed value on the pin
  heater_sw = param.asInt();
  //based on the pin value control heater and cooler
  heater_control(heater_sw);
  if (heater_sw) {
    lcd.setCursor(8, 0);
    lcd.print("HTR_ON ");

  } else {
    lcd.setCursor(8, 0);
    lcd.print("HTR_OFF");
  }
}
//To turn ON and OFF inlet vale based virtual PIN value/
BLYNK_WRITE(INLET_V_PIN) {
  inlet_sw = param.asInt();
  if (inlet_sw) {
    enable_inlet();
    lcd.setCursor(7, 1);
    lcd.print("IN_FL_ON ");

  } else {
    disable_inlet();
    lcd.setCursor(7, 1);
    lcd.print("IN_FL_OFF");
  }
}
//To turn ON and OFF outlet value based virtual switch value/
BLYNK_WRITE(OUTLET_V_PIN) 
{
  outlet_sw = param.asInt();
  if (outlet_sw) 
  {
    enable_outlet();
    lcd.setCursor(7, 1);
    lcd.print("OT_FL_ON ");

  } else 
  {
    disable_outlet();
    lcd.setCursor(7, 1);
    lcd.print("OT_FL_OFF");
  }
}
/* To display temperature and water volume as gauge on the Blynk App*/
void update_temperature_reading() {
  //send temp to blynk app on pin v1
  Blynk.virtualWrite(TEMPERATURE_GAUGE, read_temperature());

  Blynk.virtualWrite(WATER_VOL_GAUGE, volume());
}

//To turn off the heater if the temperature raises above 35 deg C/
void handle_temp(void) 
{
  //read the temp and compare it with 35 and if heater is on
  if (read_temperature() > (float) 35 && heater_sw) 
  {
    // turn off the heater and button on the blynk app also off
    heater_sw = 0;
    heater_control(0);
    Blynk.virtualWrite(HEATER_V_PIN, 0);
    // print notification on the clcd
    lcd.setCursor(8, 0);
    lcd.print("HTR_OFF");
  }
}

//To control water volume above 2000ltrs/
void handle_tank(void) 
{
  if ((volume() < 2000) && (inlet_sw == 0)) 
  {
    // turn off the heater and button on the blynk app also off
    inlet_sw = 1;
    enable_inlet();
    Blynk.virtualWrite(INLET_V_PIN, 1);
    // print notification on the clcd
    lcd.setCursor(7, 1);
    lcd.print("IN_FL_ON ");
  }
}




void setup(void) {
  //initialise ldr
  init_ldr();
  //initialise temperature system
  init_temperature_system();
  //connect arduino to blink cloud
  Blynk.begin(BLYNK_AUTH_TOKEN);
  // initialise lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("Home Automation");
  delay(1000);
  lcd.clear();
  init_serial_tank();
  //update temp on blynk every one second
  timer.setInterval(1000, update_temperature_reading);
}

float temp;
unsigned int volume_water;
void loop(void) 
{
  //control the heater and cooler from blynk
  Blynk.run();  //button heater v0 , cooler v2
                // send the temp to the blynk app on virtual pin v1
  timer.run();  //turn on the timer
  brightness_control();
  //read the temperature from temp sensor LM35
  temp = read_temperature();
  // if temp is more than 35
  handle_temp();
  lcd.setCursor(0, 0);
  lcd.print(temp);

  handle_tank();

  volume_water = volume();
  lcd.setCursor(0, 1);
  lcd.print(volume_water);
}
