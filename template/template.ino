// Each arduino code will follow this template

#define LED 4       // We define that in digital pin 4 we will connect a LED
#define ANAL A0     // We define that in analog pin 0 we will connect a analog sensor


void setup() {
    Serial.begin(9600);
}

// Return 0 if door is closed and 1 if door is open
int getDoorStatus() {

}

// Returns 0 if single windows are close or 1 if they are open
int getSingleWindowStatus() {

}

// Returns 0 if double windows are close or 1 if they are open
int getDoubleWindowStatus() {

}

// if mode is 0 it closed the coffee machine and if mode is 1 it activates it untill
// coffee is made. Then it deactivates it.
void useCoffee(int mode) {

}

// if mode is 0 it turn the lights off. If mode is 1 it turn the lights on.
void useLights(int mode) {

}

void loop() {

    // Write main code that handles every function you want.
    // for example the following code used getDoubleWindowStatus
    x= getDoubleWindowStatus();
    if (x== 0) {
        Serial.println("closed")
    }else {
        Serial.println("OPen");
    }
    
    delay(50);
}
