#include <Arduino.h>
#include "NativeEthernet.h"
#include "Ezo_uart.h"

static char SprintfBuffer[100];
const char *DomainName = "SmartGarden";

EthernetServer server(80);
Ezo_uart EZO_ph(Serial1, "pH");

char httpMessage[255];
int httpPos = 0;

void teensyMAC(uint8_t *mac) {
  for(uint8_t by=0; by<2; by++) mac[by]=(HW_OCOTP_MAC1 >> ((1-by)*8)) & 0xFF;
  for(uint8_t by=0; by<4; by++) mac[by+2]=(HW_OCOTP_MAC0 >> ((3-by)*8)) & 0xFF;
}

void setup() {
  // Sensor serial port (pins 0, 1)
  Serial1.begin(9600, SERIAL_8N1);

  // USB debugging serial port.
  Serial.begin(115200);

  Serial.println("Welcome to SmartGarden!");
  Serial.println("=======================");

  uint8_t mac[6];
  teensyMAC(mac);

  sprintf(SprintfBuffer, "MAC address: %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(SprintfBuffer);

  if (Ethernet.begin(mac) != 0) {
    IPAddress myIP = Ethernet.localIP();
    Serial.printf("IP Address %u.%u.%u.%u\r\n", myIP[0], myIP[1], myIP[2], myIP[3] );
  }
  else {
    Serial.println("Failed to configure Ethernet using DHCP\r\n");
  }

  MDNS.begin(DomainName, 1); //.local Domain name and number of services
  MDNS.addService("_http._tcp", 80);
  server.begin();

  // in order to the EZO circuit more effectively we need to turn off continuous mode and the *ok response
  EZO_ph.send_cmd_no_resp("c,0");
  EZO_ph.send_cmd_no_resp("*ok,0");
  EZO_ph.flush_rx_buffer();
  String& s = EZO_ph.send_cmd("status");
  Serial.print("ph Status=");
  Serial.println(s);
  s = EZO_ph.send_cmd("i");
  Serial.print("device info=");
  Serial.println(s);
}

String readToken(const String& msg, int& pos){
  String token;
  int len = msg.length();
  boolean started = false;
  for (; pos < len; pos++) {
    char ch = msg[pos];
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'){
      if (started) {
        break;
      }
    }
    else {
      if (!started){
        started = true;
      }
      token.append(ch);
    }
  }
  return token;
}

void loop() {// if there are incoming bytes available
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    httpPos = 0;
    httpMessage[httpPos] = '\0';
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
          // Serial.print("message: ");
          // Serial.println(httpMessage);

          int pos = 0;
          String method = readToken(httpMessage, pos);
          String path = readToken(httpMessage, pos);
          const char* cmd = path.c_str() + 1; // remove leading '/' on the path to get the sensor command.
          Serial.print("method: [");
          Serial.print(method);
          Serial.println("]");
          Serial.print("cmd: [");
          Serial.print(cmd);
          Serial.println("]");

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<style>");
          client.println("body { background-color: #1E1E1E; color: #D4D4D4;}");
          client.println("</style>");
          client.println("<body>");
          client.print("command: ");
          client.println(cmd);
          client.println("<br />");

          sprintf(SprintfBuffer, "%s\r", cmd);
          String& response_data = EZO_ph.send_cmd(cmd);
          // float pH = 0;

          client.println(response_data);
          client.println("<br />");

          // if (isdigit(response_data[0])) {                   //if the first character in the string is a digit
          //   pH = sensorstring.toFloat();                    //convert the string to a floating point number so it can be evaluated by the Arduino
          //   if (pH >= 7.0) {                                //if the pH is greater than or equal to 7.0
          //     Serial.println("alkali");                     //print "high" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
          //   }
          //   if (pH <= 6.999) {                              //if the pH is less than or equal to 6.999
          //     Serial.println("acid");                       //print "low" this is demonstrating that the Arduino is evaluating the pH as a number and not as a string
          //   }
          // }

          //sprintf(SprintfBuffer, "pH level %f", pH);
          //client.println(SprintfBuffer);
          //client.println("<br />");

          client.println("</body>");
          client.println("</html>");
          client.close(); //If "Connection: close" make sure to close after printing and before stop to avoid harsh reset
          break;
        }
        if (httpPos < 254){
          httpMessage[httpPos++] = c;
          httpMessage[httpPos] = '\0';
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
  else
  {
      // consume any continuous readings
      if (Serial1.available()) {
        String temp = Serial1.readStringUntil(13);
        Serial.print("cont: ");
        Serial.print(temp);
        Serial.println();
      }
  }
}