// NodeMCU 1.0 (ESP12)
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// WiFi network credentials
const char* ssid = "deivideich";
const char* password = "ABCD1234";

// UDP server port
const int udp_port = 1234;

String receivedString = "";

int tempo;

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  pinMode(D7, OUTPUT);

  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 100) {
      printf("\n");
      i = 0;
    }
    i++;
  }
  Serial.println("");
  Serial.println("WiFi connected");

  udp.begin(udp_port);
  Serial.print("UDP server listening on port ");
  Serial.println(udp_port);
}

void loop() {

  char a[] = "a";
  udp.beginPacket("192.168.137.1", 1234);
  udp.write(a, sizeof(a));
  udp.endPacket();

  // // int packet_size = udp.parsePacket();
  // if (packet_size) {
  //   char msg_in[3];
  //   udp.read(msg_in, sizeof(msg_in));
  //   Serial.write(msg_in);
  // }

  // int i = 0;
  // char msg[12];

  // while(Serial.available()){
  //   char c = Serial.read();


  //   if (c == '\n') { // Newline character indicates the end of the string
  //     // Process the received string
  //     // Clear the string for the next data
  //     receivedString = "";
  //   } else {
  //     receivedString += c; // Append the character to the string
  //   }
  //   udp.beginPacket("192.168.137.1", 1234); // Replace with your computer's IP address and desired UDP port

  //   int str_len = receivedString.length()+1;
  //   char char_array[str_len];
  //   for(int i =0; i<str_len;i++){
  //     char_array[i] = receivedString[i];
  //   }
  //   if(sizeof(char_array) == 12){
  //     udp.write(char_array,sizeof(char_array));
  //     udp.endPacket();
  //   }
  // }
}
