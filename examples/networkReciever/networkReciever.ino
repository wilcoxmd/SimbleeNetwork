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

void setup() {
  Serial.begin(9600);
  
  SimbleeCOM.begin();
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
  printf("%d ", rssi);
  printf("0x%08x ", esn);
//  for (int i = 0; i < len; i++)
  printf("%02x ", payload[7]);
  printf("%02x ", payload[6]);
  printf("%02x ", payload[5]);
  printf("%02x ", payload[4]);

  printf("\n");

  Serial.print("Checked address..."); Serial.println(simbleeNetwork.checkAddress(payload));
}
