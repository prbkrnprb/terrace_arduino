#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial wifiSerial(2, 3); // RX, TX

int TANK1_LOW = 4;
int TANK1_HIGH = 5;
int TANK2_LOW = 6;
int TANK2_HIGH = 7;
int WIFI_BAUD = 9600;
int SERIAL_BAUD = 9600;


void setup() {
  setupPins();
  wifiSerial.begin(WIFI_BAUD);
  Serial.begin(SERIAL_BAUD);
}

void loop() {
  String incomingString;
  boolean incoming = false;
  
  while(wifiSerial.available()){
    incomingString = wifiSerial.readString();
    incoming = true;
  }

  if(incoming){
    StaticJsonDocument<100> receivedDoc;
    deserializeJson(receivedDoc, incomingString);
    int requestId = receivedDoc["requestId"];
    StaticJsonDocument<100> doc;
    
    if(notError()){
      doc["sensorError"] = false;
      doc["requestId"] = requestId;
      doc["tank1Low"] = digitalRead(TANK1_LOW);
      doc["tank1High"] = digitalRead(TANK1_HIGH);
      doc["tank2Low"] = digitalRead(TANK2_LOW);
      doc["tank2High"] = digitalRead(TANK2_HIGH);
    }else{
      doc["sensorError"] = true;
    }
    
    char buffer[100]; 
    serializeJson(doc, buffer);
    wifiSerial.write(buffer);
    wifiSerial.flush();
  }
}

void setupPins(){
  pinMode(TANK1_LOW, INPUT_PULLUP);
  pinMode(TANK1_HIGH, INPUT_PULLUP);
  pinMode(TANK2_LOW, INPUT_PULLUP);
  pinMode(TANK2_HIGH, INPUT_PULLUP);
}

boolean notError(){
  if(digitalRead(TANK1_LOW) == HIGH && digitalRead(TANK1_HIGH) == LOW){
    return false;
  }  
  if(digitalRead(TANK2_LOW) == HIGH && digitalRead(TANK2_HIGH) == LOW){
    return false;
  }
  return true;
}
