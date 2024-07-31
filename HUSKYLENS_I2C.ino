/*Notice and Trouble shooting***
 1.Connection and Diagram can be found here
 <https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336#target_23>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ******/

#include "HUSKYLENS.h"

HUSKYLENS huskylens;
const int ledPin = 9; // Define the LED pin (using pin 13 here, change as needed)
const int faceID = 1;  // Replace with the actual ID used for faces

void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    pinMode(ledPin, OUTPUT); // Initialize the LED pin as an output

    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() {
    if (!huskylens.request()) {
        Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    }
    else if (!huskylens.isLearned()) {
        Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    }
    else if (!huskylens.available()) {
        digitalWrite(ledPin, LOW); // Turn off the LED if no object detected
    }
    else
    {
        Serial.println(F("###########"));
        bool faceDetected = false; // Flag to check if face is detected
        
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
            
            // Check if the detected object is a face
            if (result.command == COMMAND_RETURN_BLOCK && result.ID == faceID) // Check if detected object ID matches faceID
            {
                faceDetected = true;
            }
        }
        
        if (faceDetected) {
            digitalWrite(ledPin, HIGH); // Turn on the LED if a face is detected
        } else {
            digitalWrite(ledPin, LOW); // Turn off the LED if no face is detected
        }
    }
}

void printResult(HUSKYLENSResult result) {
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
        digitalWrite(ledPin, HIGH);    
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
    }
    
    else{
        Serial.println(F("Object unknown!"));
    }
}
