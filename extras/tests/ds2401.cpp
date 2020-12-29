/*
  NAME:
  Unit tests of library "gbj_DS2401" with connected real device.

  DESCRIPTION:
  The test suite provides test cases for all relevant public methods.
  - The test runner is Unity Project - ThrowTheSwitch.org.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include <Arduino.h>
#include <unity.h>
#include <gbj_ds2401.h>

// Basic setup
const unsigned char PIN_ONEWIRE = 4; // Pin for one-wire bus
gbj_ds2401 ds = gbj_ds2401(PIN_ONEWIRE);
gbj_ds2401::Address address = {
  0x01, 0x95, 0x9E, 0x44, 0x16, 0x00, 0x00, 0x07 // Replace with current one
};

void test_setup_familycode(void)
{
  TEST_ASSERT_EQUAL_HEX8(0x01, address[0]);
}

void test_setup_crc(void)
{
  TEST_ASSERT_EQUAL_HEX8(address[ds.ADDRESS_LEN - 1],
                         ds.crc8(address, ds.ADDRESS_LEN - 1));
}

void test_bus_ssns(void)
{
  TEST_ASSERT_TRUE(ds.getSSNs() >= 1);
}

void test_device_present(void)
{
  TEST_ASSERT_EQUAL_UINT8(ds.SUCCESS, ds.present(address));
}

void test_device_familycode(void)
{
  TEST_ASSERT_EQUAL_UINT8(address[0], ds.getFamilyCode());
}

void test_device_id(void)
{
  TEST_ASSERT_EQUAL_UINT8(address[ds.ADDRESS_LEN - 1], ds.getId());
}

void test_device_pin(void)
{
  TEST_ASSERT_EQUAL_UINT8(PIN_ONEWIRE, ds.getPin());
}

void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_setup_familycode);
    RUN_TEST(test_setup_crc);

    RUN_TEST(test_bus_ssns);

    RUN_TEST(test_device_present);
    RUN_TEST(test_device_familycode);
    RUN_TEST(test_device_id);
    RUN_TEST(test_device_pin);

    UNITY_END();
}

void loop(){}