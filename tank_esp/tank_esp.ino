#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "Whizkid"; //Enter Wi-Fi SSID
const char* password =  "sowmi789"; //Enter Wi-Fi Password
 
void setup() {
  Serial.begin(9600); //Begin Serial at 115200 Baud
  WiFi.begin(ssid, password);  //Connect to the WiFi network
  
  while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
      delay(500);
      Serial.println("Waiting to connect...");
  }
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP
  
  server.on("/levelCheck", levelCheck); //Handle Index page
  
  server.begin(); //Start the server
  Serial.println("Server listening");
}

void loop() {
  server.handleClient(); //Handling of incoming client requests
  while(Serial.available()){
    Serial.readString();
  }
}

void levelCheck() {
  int requestRandomNumber = random(1000, 9999);
  char request[100];
  StaticJsonDocument<100> doc;
  doc["requestId"] = requestRandomNumber;
  serializeJson(doc, request);
  Serial.write(request);
  
  int timeout = 30; 
  // 10 is 1 second;
  while( ! Serial.available() && timeout-- > 0){
    delay(100);
  }
  if( timeout <= 0){
    server.send(504, "application/json", "{[\"Unable to connect to UNO \"]}");
  }else{
    String incomingString = "";
  
      incomingString = Serial.readString();
  
    server.send(200, "application/json", incomingString);
    serialFlush();
  }
}

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
} 
