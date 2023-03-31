/*
  Connect INA219 board to Wemos D1 Mini

    VCC -> 3.3V
    GND -> GND
    SDA -> D2
    SCL -> D1

*/

#include <WPower.h>
#include <WTimer.h>
#include <WSerial.h>

WPower power;
WSerial serial;
WTimer timer;

void setup()
{
  serial.setup();
  power.setup();
  timer.setup();
  timer.start(1000);
}

void loop()
{
  power.loop();

  switch(timer.event())
  {
    case WTimer::Expired:
      Serial.println("WDMP"); 
      if(power.getInitFailed())
      {
        Serial.println("failed"); 
        power.setup();
      }
      else
      {
        float current = power.getCurrent_mA();
        Serial.print("C ");
        Serial.print(current);
        Serial.println("");
        float voltage = power.getBusVoltage_V();
        Serial.print("V ");
        Serial.print(voltage);
        Serial.println("");
      }
      timer.start(1000);
      break;
  }
  
  switch(serial.event())
  {
    default:
      break;
  }
  
}
