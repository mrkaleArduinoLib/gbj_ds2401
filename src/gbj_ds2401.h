/*
  NAME:
  gbj_ds2401

  DESCRIPTION:
  Library for silicon serial number Dallas Semiconductor DS2401.
  - Library provides a device identifier taken from last (CRC) byte of a
    device's hardware ROM address.
  - Library is primarily aimed for working with all devices on the one-wire bus
    in a loop, so that they need not to be identified by an address in advance.
    Thus, all getters and setters are valid for currently selected SSN
    in a loop.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_ds2401.git
 */
#ifndef GBJ_DS2401_H
#define GBJ_DS2401_H

#if defined(__AVR__)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
  #include <inttypes.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif
#include <OneWire.h>

class gbj_ds2401 : public OneWire
{
public:
  static const String VERSION;

  enum ResultCodes : uint8_t
  {
    SUCCESS = 0,
    END_OF_LIST = 1,
    ERROR_NO_DEVICE = 255,
    ERROR_CRC_ADDRESS = 254,
  };

  enum Params : uint8_t
  {
    ADDRESS_LEN = 8,
    SERNUM_LEN = 6,
  };

  typedef uint8_t Address[Params::ADDRESS_LEN];
  typedef uint8_t Sernum[Params::SERNUM_LEN];

  /*
    Constructor

    DESCRIPTION:
    Constructor creates the class instance object.

    PARAMETERS:
    pinBus - Number of GPIO pin of the microcontroller managing one-wire bus.
      - Data type: non-negative integer
      - Default value: none
      - Limited range: 0 ~ 255

    RETURN: object
  */
  explicit gbj_ds2401(uint8_t pinBus)
    : OneWire(pinBus)
  {
    _bus.pinBus = pinBus;
    devices();
  }

  /*
    Iterate over supported SSNs on the bus

    DESCRIPTION:
    The method selects devices with supported family code and active
    on the one-wire bus one by one for further processing by getters
    and setters.

    PARAMETERS: None

    RETURN: Result code.
  */
  ResultCodes SSNs();

  /*
    Check if device is active on one-wire bus.

    DESCRIPTION:
    The method initiates communication with the SSN upon provided address
    on the one-wire bus and checks if it is active (present) or not.

    PARAMETERS:
    address - SSN's address.
      - Data type: array of non-negative integers
      - Default value: none
      - Limited range: 0 ~ 255[8]

    RETURN: Result code.
  */
  ResultCodes present(const Address address);

  // Public setters
  inline ResultCodes setLastResult(ResultCodes result = ResultCodes::SUCCESS)
  {
    return _status.lastResult = result;
  }

  // Public getters
  inline ResultCodes getLastResult() { return _status.lastResult; }
  inline bool isSuccess() { return _status.lastResult == ResultCodes::SUCCESS; }
  inline bool isSuccess(ResultCodes result)
  {
    setLastResult(result);
    return isSuccess();
  }
  inline bool isError() { return !isSuccess(); }
  inline bool isError(ResultCodes result)
  {
    setLastResult(result);
    return isError();
  }
  inline uint8_t getPin() { return _bus.pinBus; }
  inline uint8_t getSSNs() { return _bus.SSNs; }
  inline uint8_t getFamilyCode() { return _rom.address.family; }
  inline uint8_t getId() { return _rom.address.crc; }
  inline void cpyAddress(Address address)
  {
    memcpy(address, _rom.buffer, Params::ADDRESS_LEN);
  }
  inline void cpySernum(Sernum sernum)
  {
    memcpy(sernum, _rom.address.sernum, Params::SERNUM_LEN);
  }

private:
  enum Limits : uint8_t
  {
    DS2401 = 0x01,
  };

  enum CommandsRom : uint8_t
  {
    SEARCH_ROM = 0xF0,
    READ_ROM = 0x33,
    // READ_ROM = 0x0F,  // For compatibility with DS2400
    MATCH_ROM = 0x55,
    SKIP_ROM = 0xCC,
  };

  union ROM
  {
    uint8_t buffer[Params::ADDRESS_LEN];
    struct Address
    {
      uint8_t family;
      uint8_t sernum[6];
      uint8_t crc;
    } address;
  } _rom;

  struct Bus
  {
    uint8_t pinBus;
    uint8_t SSNs; // The number of temperature SSNs on the bus
  } _bus;

  struct Status
  {
    ResultCodes lastResult;
  } _status;

  ResultCodes devices(); // Statistics of devices on the bus
  ResultCodes cpyRom(const Address address); // Copy address to ROM buffer
  inline void resetRom() { memset(_rom.buffer, 0, Params::ADDRESS_LEN); }
};

#endif
