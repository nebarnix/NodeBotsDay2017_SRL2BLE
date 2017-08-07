/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.
   See the bottom of this file for the license terms.
*/

/*
   Sketch: LedControl.ino

   Description:
     This is a Central sketch that looks for a particular Sevice with a
     certain Characteristic from a Peripheral.  Upon succesful discovery,
     it reads the state of a button and write that value to the
     Peripheral Characteristic.

   Notes:

  BLECharacteristic digitalChar("2a56", BLEWriteWithoutResponse | BLENotify, (unsigned short) 20);
  BLECharacteristic analogChar("2a58", BLEWriteWithoutResponse | BLENotify, (unsigned short) 20);
  BLECharacteristic configChar("2a59", BLEWriteWithoutResponse, (unsigned short) 20);
  BLECharacteristic curieChar("2a5b", BLEWriteWithoutResponse | BLENotify, (unsigned short) 20);

   create switch characteristic and allow remote device to read and write
    - Expected Peripheral Service: bada5555-e91f-1337-a49b-8675309fb099
    - Expected Peripheral Analog Characteristic:   2a58
    - Expected Peripheral Digital Characteristic:  2a56
    - Expected Peripheral Config Characteristic:   2a59
    - Expected Peripheral Curie(?) Characteristic: 2a5b
    - Expected Peripheral sketch:

*/

#include <CurieBLE.h>

String readString;

void setup() {
  Serial.begin(9600);

  // initialize the BLE hardware
  BLE.begin();

  Serial.println("NodeBot BLE Serial Translator - Nebarnix");

  // start scanning for peripherals
  
  BLE.setTxPower(4); //set MAX POWER!
  Serial.println("Power set to 4dbm");
  BLE.scanForUuid("bada5555-e91f-1337-a49b-8675309fb099");
  Serial.println("Scanning for UID bada5555-e91f-1337-a49b-8675309fb099");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();
  
  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("bada5555-e91f-1337-a49b-8675309fb099");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // create switch characteristic and allow remote device to read and write
  BLECharacteristic digitalChar = peripheral.characteristic("2a56");
  BLECharacteristic analogChar = peripheral.characteristic("2a58");
  BLECharacteristic configChar = peripheral.characteristic("2a59");
  BLECharacteristic curieChar = peripheral.characteristic("2a5b");

if (!digitalChar) {
    Serial.println("Peripheral does not have digital characteristic!");
    peripheral.disconnect();
    return;
  } 
  //else if (!digitalChar.canWrite()) {
  //  Serial.println("Peripheral does not have a writable digital characteristic!");
   // peripheral.disconnect();
   // return;
 // }

  if (!analogChar) {
    Serial.println("Peripheral does not have analog characteristic!");
    peripheral.disconnect();
    return;
  } 
  //else if (!analogChar.canWrite()) {
  //  Serial.println("Peripheral does not have a writable analog characteristic!");
  //  peripheral.disconnect();
  //  return;
 // }

  if (!configChar) {
    Serial.println("Peripheral does not have config characteristic!");
    peripheral.disconnect();
    return;
  } 
 // else if (!configChar.canWrite()) {
 //   Serial.println("Peripheral does not have a writable config characteristic!");
  //  peripheral.disconnect();
  //  return;
 // }

  if (!curieChar) {
    Serial.println("Peripheral does not have curie characteristic!");
    peripheral.disconnect();
    return;
  } 
  //else if (!curieChar.canWrite()) {
//    Serial.println("Peripheral does not have a writable curie characteristic!");
//    peripheral.disconnect();
//    return;
//  }
//  
  unsigned char fixspeed = 0;
  unsigned char SpeedAssemetric = 0; //If the speed is not symmetric between the two tires, IE a 'dirty' state
  unsigned char SpeedState=0; //0 - crawl, 1 - slow, 2 - fast
  unsigned char FWD[] = {8,0,7,1,5,0,4,1};
  //FWL FWR RVR RVL
  unsigned char REV[] = {8,1,7,0,5,1,4,0};
  unsigned char LFT[] = {8,1,7,0,5,0,4,1};
  unsigned char RGT[] = {8,0,7,1,5,1,4,0};
  unsigned char STP[] = {8,0,7,0,5,0,4,0};
   
  unsigned char CRL[] = {9,64,0,3,64,0};
  unsigned char CRLL[] = {9,32,0,3,127,0};
  unsigned char CRLR[] = {9,127,0,3,32,0};
  
  unsigned char SLW[] = {9,127,0,3,127,0};
  unsigned char SLWL[] = {9,64,0,3,200,0};
  unsigned char SLWR[] = {9,200,0,3,64,0};
  
  unsigned char FST[] = {9,255,0,3,255,0};
  unsigned char FSTL[] = {9,100,0,3,255,0};
  unsigned char FSTR[] = {9,255,0,3,100,0};

  unsigned char SIN[] = {6,5};
  unsigned char SOT[] = {6,180};
  
  unsigned char SERVO_CONFIG1[] = {244, 6, 4};
  unsigned char SERVO_CONFIG2[] = {112, 6, 0, 0, 180, 0};

  Serial.println("Speed initialized to crawl");
  Serial.println("Waiting for your command:");
  configChar.setValue(SERVO_CONFIG1,3); //initilize to crawl
  configChar.setValue(SERVO_CONFIG2,6); //initilize to crawl
  analogChar.setValue(CRL,6); //initilize to crawl
  while (peripheral.connected()) {
   // while the peripheral is connection
   
   
   //Serial.println("Writing bytes to Peripheral!");
   
    
 
   //if(!digitalChar.setValue(FWD_D,8))
   //   Serial.println("Write Failed...");
   
   //if(!analogChar.setValue(FWD_A,6))
   //      Serial.println("Write Failed...");
   readString = "";
   unsigned long timeout = millis();
   while (peripheral.connected() ) //wait for a string to come but allow a timeout! 
      {
      //delay(3);  //delay to allow buffer to fill 
      if (Serial.available() > 0) 
          {
          char c = Serial.read();  //gets one byte from serial buffer          
          if(c == '\n' || c == '\r' || c == ';')
            {
            //Don't do this if we're emitting multiple commands per line!
            //while(Serial.available()) Serial.read(); //Clear out any commands piling up in the input buffer (experimental)(only use with constant stream of commands)
            break;
            }
          readString += c; //makes the string readString
          }
      if((millis()-timeout) > 250 )    
        {
        readString = "NUL"; //Uncomment this if you're emitting a constant stream of commands. Use 'emit on changed' for pagenodes to avoid buffer backup
        break;
        }
      }

  fixspeed = 0;   
  if(readString == "FWD")
    {
    Serial.println("Fwd");
    digitalChar.setValue(FWD,8);     
    fixspeed = 1; //Requires even speed
    }
  else if(readString == "RVR")
    {
    Serial.println("Rev+Rht");
    digitalChar.setValue(REV,8);
    SpeedAssemetric = 1; //dirty the assymetric wheel flag
    switch(SpeedState)
      { 
        case 0: analogChar.setValue(CRLR,6); break;
        case 1: analogChar.setValue(SLWR,6); break;
        case 2: analogChar.setValue(FSTR,6); break;        
      }        
    }
  else if(readString == "RVL")
    {
    Serial.println("Rev+Lft");
    digitalChar.setValue(REV,8);        
    SpeedAssemetric = 1; //dirty the assymetric wheel flag
    switch(SpeedState)
      { 
        case 0: analogChar.setValue(CRLL,6); break;
        case 1: analogChar.setValue(SLWL,6); break;
        case 2: analogChar.setValue(FSTL,6); break;        
      }
    }
  else if(readString == "FWR")
    {
    Serial.println("Fwd+Rht");
    digitalChar.setValue(FWD,8);     
    SpeedAssemetric = 1; //dirty the assymetric wheel flag
    switch(SpeedState)
      { 
        case 0: analogChar.setValue(CRLR,6); break;
        case 1: analogChar.setValue(SLWR,6); break;
        case 2: analogChar.setValue(FSTR,6); break;        
      }
    }
  else if(readString == "FWL")  
    {
    Serial.println("Fwd+Lft");
    digitalChar.setValue(FWD,8); 
    SpeedAssemetric = 1; //dirty the assymetric wheel flag
    switch(SpeedState)
      { 
        case 0: analogChar.setValue(CRLL,6); break;
        case 1: analogChar.setValue(SLWL,6); break;
        case 2: analogChar.setValue(FSTL,6); break;        
      }    
    }
  else if(readString == "REV")
    {
    Serial.println("Rev");
    digitalChar.setValue(REV,8);
    fixspeed = 1; //Requires even speed
    }
  else if(readString == "LFT")
    {
    Serial.println("Lft");
    digitalChar.setValue(LFT,8);
    fixspeed = 1; //Requires even speed
    }
  else if(readString == "RGT")
    {
    Serial.println("Rght");
    digitalChar.setValue(RGT,8);
    fixspeed = 1; //Requires even speed
    }
  else if(readString == "STP")
    {
    Serial.println("Rrch");
    digitalChar.setValue(STP,8); 
    }
  else if(readString == "SLW")
    {
    SpeedState = 1;
    Serial.println("Slow");
    analogChar.setValue(SLW,6); 
    }
  else if(readString == "CRL")
    {
    SpeedState = 0;
    Serial.println("crawl");
    analogChar.setValue(CRL,6); //initilize to crawl     
    }
  else if(readString == "FST")
    {
    SpeedState = 2;  
    Serial.println("Zoom");
    analogChar.setValue(FST,6);
    }
  else if(readString == "SIN")
    {
    Serial.println("Undeploying Servo!");
    analogChar.setValue(SIN,2); 
    }
  else if(readString == "SOT")
    {
    Serial.println("Deploying Servo!");
    analogChar.setValue(SOT,2);
    }    
  else if(readString == "NUL");
  else
    {
    Serial.print(readString);
    Serial.println(" <- Unrecognized command :(");
    }
  
  if(fixspeed == 1 && SpeedAssemetric == 1) //if we just came out of a turn, even up the wheels again
    {
    Serial.println("Fix Spd");
    switch(SpeedState)
      { 
        case 0: analogChar.setValue(CRL,6); break;
        case 1: analogChar.setValue(SLW,6); break;
        case 2: analogChar.setValue(FST,6); break;        
      }
      SpeedAssemetric = 0;
    } 
  }
  
   
    
  Serial.println("Peripheral disconnected");
}

/*
  Arduino BLE Central LED Control example
  Copyright (c) 2016 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/



