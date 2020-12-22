/*
  NAME:
  Test presence of a single DS2401 silicon serial number on the one-wire bus.

  DESCRIPTION:
  The sketch uses definite SSN and checks if it communicates.
  - Replace SSN's ROM in the sketch with your current one.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_ds2401.h"

#define SKETCH "GBJ_DS2401_PRESENT 1.1.0"

const unsigned char PIN_DS2401 = 4; // Pin for one-wire bus

gbj_ds2401 ds = gbj_ds2401(PIN_DS2401);
gbj_ds2401::Address address = {
  0x01, 0x95, 0x9E, 0x44, 0x16, 0x00, 0x00, 0x07 // Replace with current one
};

String textAddress(gbj_ds2401::Address address)
{
  String text = "";
  char data[3];
  for (byte i = 0; i < gbj_ds2401::ADDRESS_LEN; i++)
  {
    if (i)
      text += "-";
    sprintf(data, "%02X", address[i]);
    text += data;
  }
  return text;
}

void errorHandler()
{
  switch (ds.getLastResult())
  {
    case gbj_ds2401::SUCCESS:
      Serial.println("SUCCESS");
      break;

    case gbj_ds2401::END_OF_LIST:
      Serial.println("END_OF_LIST");
      break;

    case gbj_ds2401::ERROR_NO_DEVICE:
      Serial.println("ERROR_NO_DEVICE");
      break;

    case gbj_ds2401::ERROR_CRC_ADDRESS:
      Serial.println("ERROR_CRC_ADDRESS");
      break;

    default:
      Serial.println("Unknown error");
      break;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println(); // Some serial monitors display garbage at the begining
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_ds2401::VERSION);
  Serial.println("---");
  Serial.println("Address: " + String(textAddress(address)));
  if (ds.isSuccess(ds.present(address)))
  {
    Serial.println("Id: " + String(ds.getId()));
  }
  errorHandler();
}

void loop() {}
