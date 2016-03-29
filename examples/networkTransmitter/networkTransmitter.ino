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

SimbleeNetwork simbleeNetwork(3);

char payload[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
int test = 0;
unsigned long myESN;

void setup() {
  SimbleeCOM.begin();

  SimbleeCOM.mode = LONG_RANGE;
  SimbleeCOM.txPowerLevel = +4; //default value is +4 (-20, -16, -12, -8, -4, 0, +4)
  
  Serial.begin(9600);

  myESN = SimbleeCOM.getESN();
  Serial.print("Starting sending... my ESN: ");
  Serial.println(myESN, HEX);
}

void loop() {
  delay(1000);
  test = simbleeNetwork.send(1, 1);
  Serial.println(test);

  
}
  
