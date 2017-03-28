/**********************************************
* IEEE University of Ioannina Student Branch
*
* The following csketch is developed by IEEE 
* UOI Student Branch and is under Apache2 Licence
*
* Security System v1.0 
**********************************************/

// Digital Pins Declaration
#define cafePin 2
#define PIRPin 3
#define vibrationPin 4
#define tempPin 5
#define buzzPin 6
#define echoPin 7
#define trigPin 8

// analog Pins declaration
#define soundPin A0;

// Include any libraries that are necessary for the sketch here
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte sensorPin = 3;

//IPAddress ip(192, 168, 1, 177);
IPAddress ip(10, 7, 4, 148);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

// Ultrasonic Distance Sensor
int maximumRange = 200;   // Maximum range needed 
int minimumRange = 0;     // Minimum range needed 
long duration, distance;  // Duration used to calculate distance  

int micSound;
int MOVEMENT;
int DOOR = 0;   // 0 is closed 1 is open

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Define input and output sensors
  pinMode(buzzPin, OUTPUT);
  pinMode(sensorPin,INPUT);
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);
  pinMode(vibrationPin, INPUT);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void door() {
  digitalWrite(trigPin, LOW);   
  delayMicroseconds(2);   
  digitalWrite(trigPin, HIGH);  
  delayMicroseconds(10);     
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58.2;
  
  if( distance == -1 ) {
    // Something is going with the door
    DOOR = -1;
  }else if( (distance >=3) && (distance <=4)) {
    // Door is close
    DOOR = 0;
  }else {
    // Door is open
    DOOR = 1;
  }

  
}

void PIR() {
  MOVEMENT = digitalRead(sensorPin);  
}

void viber() {

}


void microphone() {
  micSound = analogRead(A0);   //connect mic sensor to Digital 5       
  Serial.println(micSound,DEC);//print the sound value to serial               
  delay(100); 
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("IEEE University of Ioannina Student Branch");
          client.println("<br />");
          client.println("<br />");
          
          door();
          if ( DOOR == 0 ) {
            client.print("Office door is CLOSED...");
          }else if(DOOR == 1) {
            client.print("Office door is OPEN,,.");
    
            digitalWrite(buzzPin, HIGH);
            delay(300);
            digitalWrite(buzzPin, LOW); 
            delay(100); 
            digitalWrite(buzzPin, HIGH);
            delay(500);
            digitalWrite(buzzPin, LOW); 
            delay(5000); 
          }
          client.println("<br />");

  
          PIR();
          if(MOVEMENT == 1) {
            client.println("Movement detected...");
          }else if(MOVEMENT == 0) {
            client.println("No movement...");
          }
          client.println("<br />");
          
          microphone();
          client.println("Microphone sound:");
          client.println(micSound);
          client.println("<br />");
          client.println("</html>");
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
