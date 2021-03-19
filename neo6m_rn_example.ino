
/*
//Decoder function

function Decoder(b, port) {

  // LSB, Least Significant Bit/Byte first! Your node likely sends MSB instead.

  // Sign-extend the 3rd and 6th bytes into a 4th and 8th byte:
  var lat = (b[0] | b[1]<<8 | b[2]<<16 | (b[2] & 0x80 ? 0xFF<<24 : 0)) / 10000;
  var lng = (b[3] | b[4]<<8 | b[5]<<16 | (b[5] & 0x80 ? 0xFF<<24 : 0)) / 10000;

  return {
    location: {
      lat: lat,
      lng: lng
    },
    love: "TTN payload functions"
  };
}
*/
#include <TheThingsNetwork.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial serialgps(3,4);
SoftwareSerial loraSerial = SoftwareSerial(8, 7);
#define debugSerial Serial
// Set your AppEUI and AppKey
const char *appEui = "70B3D57ED003EF69";
const char *appKey = "B4AF71B9B7EF65716A91CA26ABE726CC";

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
//debugSerial.println("-- LOOP");
while(serialgps.available()) 
{
int c = serialgps.read(); 
if(gps.encode(c)) 
{
float latitude, longitude;
gps.f_get_position(&latitude, &longitude);
byte coords[6];
uint32_t latit = (latitude)*10000;
uint32_t longit = (longitude)*10000;

coords[0] = latit;
coords[1] = latit >> 8;
coords[2] = latit >> 16;
coords[3] = longit;
coords[4] = longit >> 8;
coords[5] = longit >> 16;
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

ttn.sendBytes(coords, sizeof(coords));
delay(2000);
}
}
}
