#include <OneWire.h>
#include <EtherCard.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

// A0 relais
// A3 button

OneWire  ds(2);  // on pin 10 (a 4.7K resistor is necessary)

static byte mymac[] = {0x06,0x20,0x69,0x2D,0x30,0x31};
byte Ethernet::buffer[830];

float tapSensorTemp = 0;
float optSensor1Temp = 0;
float optSensor2Temp = 0;

bool bpState = FALSE;
bool fissaMode = FALSE;

void setup(void)
{
  Serial.begin(9600);

  // Setup IO
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(A3, INPUT_PULLUP);
  
  // Connecting to network
  Serial.print("MAC: ");
  for (byte i = 0; i < 6; ++i) {
    Serial.print(mymac[i], HEX);
    if (i < 5)
      Serial.print(':');
  }
  Serial.println();
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
    
  Serial.println(F("Setting up DHCP"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
  
  ether.printIp("My IP: ", ether.myip);
  ether.printIp("Netmask: ", ether.netmask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);

  ether.hisport = 8182;
}

void loop(void)
{
  pollTimer();
  handleNetworking();
  handleIO();
}

