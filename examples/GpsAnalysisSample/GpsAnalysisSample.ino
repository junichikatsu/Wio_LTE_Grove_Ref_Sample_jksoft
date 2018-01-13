#include <WioLTEforArduino.h>
#include <MicroNMEA.h>

HardwareSerial& gps = Serial;
char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));
bool ledState = LOW;

WioLTE Wio;

void led(bool state)
{
  if(state)  Wio.LedSetRGB(0, 255, 0);
  else       Wio.LedSetRGB(255, 0, 0);
}

void gpsHardwareReset()
{
  while (gps.available())
    gps.read();
  
  Wio.PowerSupplyGrove(false);
  delay(500);
  Wio.PowerSupplyGrove(true);

  while (1) {
    while (gps.available()) {
      char c = gps.read();
      if (nmea.process(c))  return;
      
    }
  }
}

void setup()
{
  gps.begin(9600);
  
  Wio.Init();
  Wio.PowerSupplyGrove(true);

  led(ledState);
  
  SerialUSB.println("Resetting GPS module ...");
  gpsHardwareReset();
  SerialUSB.println("... done");
  
  MicroNMEA::sendSentence(gps, "$PORZB");
  MicroNMEA::sendSentence(gps, "$PORZB,RMC,1,GGA,1");
  MicroNMEA::sendSentence(gps, "$PNVGNME,2,9,1");
  
}

void loop()
{  
  if(nmea.isValid()) {
    ledState = true;
    SerialUSB.println();
    SerialUSB.println("-----------------------");  
  
    SerialUSB.print("Valid fix: ");
    SerialUSB.println(nmea.isValid() ? "yes" : "no");
  
    SerialUSB.print("Nav. system: ");
    if (nmea.getNavSystem())
      SerialUSB.println(nmea.getNavSystem());
    else
      SerialUSB.println("none");
  
    SerialUSB.print("Num. satellites: ");
    SerialUSB.println(nmea.getNumSatellites());
  
    SerialUSB.print("HDOP: ");
    SerialUSB.println(nmea.getHDOP()/10., 1);
    
    SerialUSB.print("Date/time: ");
    SerialUSB.print(nmea.getYear());
    SerialUSB.print('-');
    SerialUSB.print(int(nmea.getMonth()));
    SerialUSB.print('-');
    SerialUSB.print(int(nmea.getDay()));
    SerialUSB.print('T');
    SerialUSB.print(int(nmea.getHour()));
    SerialUSB.print(':');
    SerialUSB.print(int(nmea.getMinute()));
    SerialUSB.print(':');
    SerialUSB.println(int(nmea.getSecond()));
  
    long latitude_mdeg = nmea.getLatitude();
    long longitude_mdeg = nmea.getLongitude();
    SerialUSB.print("Latitude (deg): ");
    SerialUSB.println(latitude_mdeg / 1000000., 6);
  
    SerialUSB.print("Longitude (deg): ");
    SerialUSB.println(longitude_mdeg / 1000000., 6);
  
    long alt;
    SerialUSB.print("Altitude (m): ");
    if (nmea.getAltitude(alt))
      SerialUSB.println(alt / 1000., 3);
    else
      SerialUSB.println("not available");
  
    SerialUSB.print("Speed: ");
    SerialUSB.println(nmea.getSpeed() / 1000., 3);
    SerialUSB.print("Course: ");
    SerialUSB.println(nmea.getCourse() / 1000., 3);
  
    SerialUSB.println("-----------------------");    
    nmea.clear();
  }
  else
  {
    ledState = false;
  }
  
  led(ledState);

  while (gps.available()) {
    char c = gps.read();
    if(!ledState) SerialUSB.print(c);
    nmea.process(c);
  }
  delay(1000);
}
