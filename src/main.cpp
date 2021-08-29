#include <Arduino.h>
#include "NativeEthernet.h"

static char SprintfBuffer[100];
const char *DomainName = "SmartGarden";

EthernetServer server(80);

void teensyMAC(uint8_t *mac) {
  for(uint8_t by=0; by<2; by++) mac[by]=(HW_OCOTP_MAC1 >> ((1-by)*8)) & 0xFF;
  for(uint8_t by=0; by<4; by++) mac[by+2]=(HW_OCOTP_MAC0 >> ((3-by)*8)) & 0xFF;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial port to connect.

  Serial.println("Welcome to SmartGarden!");
  Serial.println("=======================");

  uint8_t mac[6];
  teensyMAC(mac);

  sprintf(SprintfBuffer, "MAC address: %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(SprintfBuffer);

  if (Ethernet.begin(mac) != 0) {
    IPAddress myIP = Ethernet.localIP();
    Serial.printf( "IP Address %u.%u.%u.%u\n", myIP[0], myIP[1], myIP[2], myIP[3] );
  }
  else {
    Serial.println("\nFailed to configure Ethernet using DHCP");
  }

  MDNS.begin(DomainName, 1); //.local Domain name and number of services
  MDNS.addService("_http._tcp", 80);
  server.begin();
}

void loop() {// if there are incoming bytes available
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          Serial.print("Responding to client: ");
          Serial.println(client.remoteIP());

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          client.close(); //If "Connection: close" make sure to close after printing and before stop to avoid harsh reset
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}