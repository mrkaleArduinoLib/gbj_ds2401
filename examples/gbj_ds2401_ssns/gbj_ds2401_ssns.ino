/*
  NAME:
  List DS2401 silicon serial numbers on the one-wire bus.

  DESCRIPTION:
  The sketch identifies all active silicon serial numbers on the one-wire bus
  and lists parameters for each of them.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_ds2401.h"

#define SKETCH "GBJ_DS2401_SSNS 1.0.1"

const unsigned char PIN_DS2401 = 4; // Pin for one-wire bus

gbj_ds2401 ds = gbj_ds2401(PIN_DS2401);
gbj_ds2401::Address address;
gbj_ds2401::Serial serial;
char buffer[50];

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

String textSerial(gbj_ds2401::Serial serial)
{
  String text = "";
  char data[3];
  for (byte i = 0; i < gbj_ds2401::SERIAL_LEN; i++)
  {
    if (i)
      text += ":";
    sprintf(data, "%02X", serial[i]);
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
  Serial.println("SSNs: " + String(ds.getSSNs()));
  Serial.println("---");
  uint8_t deviceNum = 0;
  while (ds.isSuccess(ds.SSNs()))
  {
    ds.cpyAddress(address);
    ds.cpySerial(serial);
    Serial.println(String(++deviceNum) + ". Id: " + String(ds.getId()));
    Serial.println("Address: " + String(textAddress(address)));
    Serial.println("Serial: " + String(textSerial(serial)));
    Serial.println("---");
  }
  errorHandler();
}

void loop() {}
