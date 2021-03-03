#include <TheThingsNetwork.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial serialgps(2,3);
SoftwareSerial loraSerial = SoftwareSerial(8, 7);
#define debugSerial Serial
// Set your AppEUI and AppKey
const char *appEui = "70B3D57EF000630E";
const char *appKey = "DC02B9CD60F635CD719BE06DD9EDC792";

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868
int year;
byte month, day, hour, minute, second, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
loraSerial.begin(57600);
debugSerial.begin(9600);
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
//Serial.begin(9600);/
serialgps.begin(9600);
debugSerial.println("==========================");
debugSerial.println("GPS");
debugSerial.println("===========================");
}

void loop()
{
debugSerial.println("-- LOOP");
while(serialgps.available()) 
{
int c = serialgps.read(); 
if(gps.encode(c)) 
{
float latitude, longitude;
gps.f_get_position(&latitude, &longitude);
uint16_t latit = (latitude,6)*100;
uint16_t longit = (longitude,6)*100;

byte payload[4];
payload[0] = highByte(latit);
payload[1] = lowByte(latit);
payload[2] = highByte(longit);
payload[3] = lowByte(longit);

debugSerial.println("-- LOOP");
debugSerial.print("Latitude: "); 
debugSerial.println("-- LOOP");
debugSerial.println(latit); 
debugSerial.println("-- LOOP");
debugSerial.print("Longitude: "); 
debugSerial.println("-- LOOP");
debugSerial.println(longit); 
debugSerial.println("-- LOOP");
debugSerial.println();

gps.stats(&chars, &sentences, &failed_checksum);

ttn.sendBytes(payload, sizeof(payload));
delay(2000);
}
}
}
