// Tzivaras Vasiis
// vtzivaras@gmail.com

#define echoPin 7          // Echo Pin
#define trigPin 8          // Trigger Pin
#define doorLightPin 6     // Door light Pin
#define photointerPin = 5  // Photo Interapter Pin
#include <dht.h>

dht DHT;
int LED = 13;
#define DHT11_PIN 5

int maximumRange = 200;    // Maximum range needed
int minimumRange = 0;      // Minimum range needed
long duration, distance;   // Duration used to calculate distance

int DOOR_STATUS = 0;           // 1 if Door is closed 0 if opened
int DOOR_LIGHT = 0;            // 1 lights on and 0 lights off
int PHOTOINTER = 0;

void setup() {
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(doorLightPin, OUTPUT);
 pinMode(5, INPUT);
}

void updateDoorStatus() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  if( (distance >= 4) && (distance <= 8) ) {
    // Closed door
    DOOR_STATUS = 0;
    DOOR_LIGHT = 0;
  }else{
    // Opened door
    DOOR_STATUS = 1;
    DOOR_LIGHT = 1;
  } 
}

void checkTemp(int t) {
  if(t>25) {
      // Turn LED on
      digitalWrite(LED, HIGH);
      //Serial.println("LED is on");
  }else {
      // Turn LED off
      digitalWrite(LED, LOW);
      //Serial.println("LED is off");
  }
}

void updateTempHum() {
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  checkTemp(DHT.temperature);  
}

void loop() {
  updateDoorStatus();
 
   
  // Door status
  if(DOOR_STATUS == 1) {
    Serial.print("Door is open\t");
  }else {
    Serial.print("Door is closed\t");
  }
 
  // Door light
  if(DOOR_STATUS == 1) {
    Serial.print("Lights ON\t");
    digitalWrite(doorLightPin, DOOR_LIGHT);
  }else {
    Serial.print("Lights off\t");
    digitalWrite(doorLightPin, DOOR_LIGHT);
  }
  
  PHOTOINTER = digitalRead(5);
  Serial.print("PhotoInter: ");
  Serial.print(PHOTOINTER);
  updateTempHum();
  
 //Delay 50ms before next reading.
 delay(500);
}
