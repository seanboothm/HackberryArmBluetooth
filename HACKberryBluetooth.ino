#include "BluetoothSerial.h"
#include <Servo.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//// Start Config ////

const int pinServoIndex = 15;
const int pinServoOther = 2;
const int pinServoThumb = 4;

const int maxServoIndex = 2;
const int maxServoOther = 2;
const int maxServoThumb = 2;

const int minServoIndex = 2;
const int minServoOther = 2;
const int minServoThumb = 2;

//// End Config ////

BluetoothSerial SerialBT;
Servo ServoIndex;
Servo ServoOther;
Servo ServoThumb;

int posServoIndex = 2;
int posServoOther = 2;
int posServoThumb = 2;

void setup() {
  Serial.begin(115200);
  SerialBT.setTimeout(15); // To shorten delay of Serial.readString()

  SerialBT.begin("HACKberry Arm"); //Bluetooth device name

  Serial.println("The device started, now you can pair it with bluetooth!");

  ServoIndex.attach(pinServoIndex);
  ServoOther.attach(pinServoOther);
  ServoThumb.attach(pinServoThumb);
}

void loop() {
  getSerial();
  move();
}

void move(){
  

}

char myBuffer[64];

void getSerial() {
  if(SerialBT.available()) {
    String input = SerialBT.readString();
    strcpy(myBuffer, input.c_str());
    Serial.println("Received " + input);        
    processCommand();
    
    Serial.flush();
  }
}

void processCommand(){  
  if(parseNumber('P', posServoIndex) != posServoIndex) {
    Serial.print("Moving Index to: ");
    posServoIndex = parseNumber('P', posServoIndex);
    Serial.println(posServoIndex);
    ServoIndex.write(posServoIndex);
  }

  if(parseNumber('O', posServoOther) != posServoOther) {
    Serial.print("Moving Other to: ");
    posServoOther = parseNumber('O', posServoOther);
    Serial.println(posServoOther);
    
  ServoOther.write(posServoOther);
  }

  if(parseNumber('T', posServoThumb) != posServoThumb) {
    Serial.print("Moving Thumb to: ");
    posServoThumb = parseNumber('T', posServoThumb);
    Serial.println(posServoThumb);
    
  ServoThumb.write(posServoThumb);

  }
}

// Parse myBuffer for floats after char "code"
float parseNumber(char code,float val) {
  char *ptr=myBuffer;  // start at the beginning of buffer
  while((long)ptr > 1 && (*ptr) && (long)ptr < (long)myBuffer+64) {  // walk to the end
    if(*ptr==code) {  // if you find code on your walk,
      return atof(ptr+1);  // convert the digits that follow into a float and return it
    }
    ptr++;
    //ptr=strchr(ptr,' ')+1;  // take a step from here to the letter after the next space
  }
  return val;  // end reached, nothing found, return default val.
}




/*
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
*/