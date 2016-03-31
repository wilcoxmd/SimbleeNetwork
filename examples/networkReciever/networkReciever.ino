/*
 * Copyright (c) 2015 RF Digital Corp. All Rights Reserved.
 *
 * The source code contained in this file and all intellectual property embodied in
 * or covering the source code is the property of RF Digital Corp. or its licensors.
 * Your right to use this source code and intellectual property is non-transferable,
 * non-sub licensable, revocable, and subject to terms and conditions of the
 * SIMBLEE SOFTWARE LICENSE AGREEMENT.
 * http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt
 *
 * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 * This heading must NOT be removed from this file.
 */

#include "SimbleeCOM.h"
#include "SimbleeNetwork.h"

SimbleeNetwork SimbleeNetwork;

bool ACKed = false;

void setup() {
  Serial.begin(9600);
  SimbleeNetwork.begin();
  
  SimbleeCOM.mode = LONG_RANGE;
  SimbleeCOM.txPowerLevel = +4; //default value is +4 (-20, -16, -12, -8, -4, 0, +4)
  Serial.println("Hello World!");
  unsigned long myESN = SimbleeCOM.getESN();
  Serial.println(myESN, HEX);
  delay(100);
}

void loop() {
}


void SimbleeCOM_onReceive(unsigned int esn, const char *payload, int len, int rssi)
{
  //check if this message is for us
  bool messageIsForMe = SimbleeNetwork.checkAddress(payload);
  if (messageIsForMe)
  {
    uint16_t data;
    //check to see if this is an ack for a message we just sent
    if (SimbleeNetwork.readDataType(payload) == 1)
    {
      ACKed = true;
    }
    //check for ACK payload data
    //data = SimbleeNetwork.readData(payload);
    //Serial.print("I received ACK message data: "); Serial.println(data, HEX);
    else
    {
      //if not a simple ACK message, rad in the data being sent to us
      data = SimbleeNetwork.readData(payload);
      Serial.print("I received a message: "); Serial.println(data, HEX);
    }
  }
  else
  {
    //Serial.println("That message wasn't for me");
  }
  
}
