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
  BLE.scanForUuid("bada5555-e91f-1337-a49b-8675309fb099");
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

  unsigned char FWD[] = {8,0,7,1,5,0,4,1};
  //FWL FWR RVR RVL
  unsigned char REV[] = {8,1,7,0,5,1,4,0};
  unsigned char LFT[] = {8,1,7,0,5,0,4,1};
  unsigned char RGT[] = {8,0,7,1,5,1,4,0};
  unsigned char STP[] = {8,0,7,0,5,0,4,0};
   
  unsigned char CRL[] = {9,64,0,3,64,0};
  unsigned char SLW[] = {9,127,0,3,127,0};
  unsigned char FST[] = {9,255,0,3,255,0};

  unsigned char SIN[] = {6,10};
  unsigned char SOT[] = {6,170};
  
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
   while (peripheral.connected()) //wait for a string to come! 
      {
      delay(3);  //delay to allow buffer to fill 
      if (Serial.available() > 0) 
          {
          char c = Serial.read();  //gets one byte from serial buffer          
          if(c == '\n' || c == '\r' || c == ';')
            break;
          readString += c; //makes the string readString
          }
      }
  if(readString == "FWD")
    {
    Serial.println("Going forward!");
    digitalChar.setValue(FWD,8); //initilize to crawl    
    }
  else if(readString == "REV")
    {
    Serial.println("Going backward!");
    digitalChar.setValue(REV,8); //initilize to crawl     
    }
  else if(readString == "LFT")
    {
    Serial.println("Going left!");
    digitalChar.setValue(LFT,8); //initilize to crawl     
    }
  else if(readString == "RGT")
    {
    Serial.println("Going right!");
    digitalChar.setValue(RGT,8); //initilize to crawl     
    }
  else if(readString == "STP")
    {
    Serial.println("Stopping!");
    digitalChar.setValue(STP,8); //initilize to crawl     
    }
  else if(readString == "SLW")
    {
    Serial.println("Going Slowly!");
    analogChar.setValue(SLW,6); //initilize to crawl     
    }
  else if(readString == "CRL")
    {
    Serial.println("Going REALLY Slowly!");
    analogChar.setValue(CRL,6); //initilize to crawl     
    }
  else if(readString == "FST")
    {
    Serial.println("Going FAST!");
    analogChar.setValue(FST,6); //initilize to crawl     
    }
  else if(readString == "SIN")
    {
    Serial.println("Undeploying Servo!");
    analogChar.setValue(SIN,2); //initilize to crawl     
    }
  else if(readString == "SOT")
    {
    Serial.println("Deploying Servo!");
    analogChar.setValue(SOT,2); //initilize to crawl     
    }    
  else
    {
    Serial.println("Unrecognized command :(");
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



