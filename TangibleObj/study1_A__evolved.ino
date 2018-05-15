/*
This sketch is designed for the study on 
graded crossmodal perception and interaction.
Last modified: 
29 March 2016
*/
#include <XBee.h>
#include <SoftwareSerial.h>

int redPin = 3;
int greenPin = 4;
int bluePin = 5;
int motorPin = 20;

XBee xbee = XBee(); 
XBeeResponse response = XBeeResponse();
ZBRxResponse rxZB = ZBRxResponse();
// control value
boolean keepActive = true;
int vibDone = 0;
int conDition = 0;
int TempValue = 0;
// function value
int intensityValue = 128;
int controlValue = 0;


void setup()   {
  Serial.begin(9600);
  Serial1.begin(9600); // could also be serial 2, 3, but they are not available currently
  xbee.setSerial(Serial1);
  
  pinMode(motorPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop(){
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE){
        xbee.getResponse().getZBRxResponse(rxZB);
        TempValue = rxZB.getData(0);
        Serial.println("The received original value is: ");
        Serial.println(TempValue);
        if(TempValue == 0){ // if receive 0 from processing, task is done, object need to keep still
        keepActive = false;
        }
        if(TempValue == 10){// if receive 10 from processing, the sub trial under vibration begin
          keepActive = true;
          vibDone = 0;
          intensityValue = 128;// edited in 29. mar 2016.
          Serial.println("xbee activited");
        }
      }
      
     if(TempValue == 11 || TempValue == 12 || TempValue == 13){
        conDition = TempValue;
     }else if (TempValue > 13) {
        intensityValue = TempValue;
     }
     
     // if received value == 9, close all the pins, start new sub-trial
     if(TempValue == 9){
       Serial.println("I got the initialization signal:9");
       conDition = 0;
       keepActive = true;
       vibDone = 0;
       analogWrite(motorPin, 0);
       analogWrite(greenPin, 0);
       analogWrite(bluePin, 0);
     }
        
     if(conDition == 12){  // haptic-the Vibration mode
        Serial.println("I got condition 12");
        if(keepActive){
          Serial.println(intensityValue);
          controlValue = map(intensityValue, 1, 170, 240, 60);
          analogWrite(motorPin, controlValue);
          Serial.println("analogWrite vibrition output is: ");
          Serial.println(controlValue);
        }
        if(keepActive == false && vibDone ==0){
          analogWrite(motorPin, 0);
          delay(100);
          analogWrite(motorPin, 255);
          delay(100);
          analogWrite(motorPin, 0);
          delay(100);
          analogWrite(motorPin, 255);
          delay(100);
          analogWrite(motorPin, 0);
          delay(100);
          analogWrite(motorPin, 255);
          delay(100);
          analogWrite(motorPin, 0);
          vibDone = 1;
        }
      }

    if(conDition == 13){  // visual-the LED mode
      Serial.println("I got visual control signal: 13");
      controlValue = map(intensityValue, 1, 150, 254, 1);// control the LED
      analogWrite(greenPin, controlValue);
      analogWrite(bluePin, controlValue);
      Serial.println("analogWrite visual output is: ");
      Serial.println(controlValue);
    }
  }
 }


