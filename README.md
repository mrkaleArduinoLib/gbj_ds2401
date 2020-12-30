<a id="library"></a>
# gbj_DS2401
Library for Dallas Semiconductor DS2401 one-wire silicone serial number (hereinafter referred to as "SSN").
- Library utilizes a detailed error handling.
- Library provides a _device identifier_ taken from last byte (<abbr title="Cyclic Redundancy Code">CRC</abbr>) of a device's hardware <abbr title="Read Only Memory">ROM</abbr>.
- Library is primarily aimed for working with all SSNs on the one-wire bus in a loop, so that they need not to be identified by an address in advance. Thus, all getters and setters are valid for currently selected device in a loop.

<a id="dependency"></a>
## Dependency

- **OneWire**: Library for communication on one-wire library #1 in PlatformIO Library Manager available at [https://platformio.org/lib/show/1/OneWire](https://platformio.org/lib/show/1/OneWire).

#### Particle platform
- **Particle.h**: Includes alternative (C++) data type definitions.

#### ESP32/ESP8266 platform
- **Arduino.h**: Main include file for the Espressif platform.

#### Arduino platform
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.


<a id="tests"></a>
## Unit testing

The subfolder `tests` in the folder `extras`, i.e., `gbj_ds2401/extras/test`, contains testing files, usually just one, with unit tests of library [gbj_DS2401](#library) executable by [Unity](http://www.throwtheswitch.org/unity) test runner. Each testing file should be placed in an individual test folder of a particular project, usually in the structure `test/<testname>/<testfile>`.
- **ds2401.cpp**: Test suite providing test cases for all relevant public methods with connected real silicon serial number. Its ROM address should be set in the code of the test file directly.


<a id="constants"></a>
## Constants

- **gbj\_ds2401::VERSION**: Name and semantic version of the library.


<a id="params"></a>
#### Parameters for buffers

- **gbj\_ds2401::ADDRESS\_LEN**  (`gbj_ds2401::Params::ADDRESS_LEN`): Number of bytes in the device's ROM.
- **gbj\_ds2401::SERNUM\_LEN** (`gbj_ds2401::Params::SERNUM_LEN`): Number of bytes in the device's serial number.


<a id="results"></a>
#### Result and error codes

- **gbj\_ds2401::SUCCESS** (`gbj_ds2401::ResultCodes::SUCCESS`): Successful processing of the recent function.
- **gbj\_ds2401::END\_OF\_LIST** (`gbj_ds2401::ResultCodes::END_OF_LIST`): Last device on the bus has been processed.
- **gbj\_ds2401::ERROR\_NO\_DEVICE** (`gbj_ds2401::ResultCodes::ERROR_NO_DEVICE`): No device on the one-wire bus detected.
- **gbj\_ds2401::ERROR\_CRC\_ADDRESS** (`gbj_ds2401::ResultCodes::ERROR_CRC_ADDRESS`): Bad hardware address of a device.


<a id="interface"></a>
## Interface
It is possible to use functions from the parent library [OneWire](#dependency), which is extended by the [gbj_DS2401](#library).


##### Custom data types

- [gbj_ds2401::Address](#address)
- [gbj_ds2401::Sernum](#Sernum)


##### Main functions

- [gbj_ds2401()](#constructor)
- [present()](#present)
- [SSNs()](#SSNs)


##### Utilities

- [cpyAddress()](#cpyAddress)
- [cpySernum()](#cpySernum)
- [getLastResult()](#getLastResult)
- [isError()](#isResult)
- [isSuccess()](#isResult)
- [setLastResult()](#setLastResult)


<a id="setters"></a>
#### Setters

- [setLastResult()](#setLastResult)


<a id="getters"></a>
#### Getters

- [cpyAddress()](#cpyAddress)
- [cpySernum()](#cpySernum)
- [getFamilyCode()](#getFamilyCode)
- [getId()](#getId)
- [getLastResult()](#getLastResult)
- [getPin()](#getPin)
- [getSSNs()](#getSSNs)
- [isError()](#isResult)
- [isSuccess()](#isResult)


<a id="address"></a>
## Address

#### Description
Custom data type determining the byte array for device hardware address (ROM).
- The size of the address text is determined by the constant [gbj\_ds2401::ADDRESS\_LEN](#params).

#### Syntax
    gbj_ds2401::Address address

#### Example
```cpp
gbj_ds2401::Address address = {0x01, 0xA7, 0x62, 0x44, 0x16, 0x00, 0x00, 0x03};
```
#### See also
[Back to interface](#interface)


<a id="Sernum"></a>
## Sernum

#### Description
Custom data type determining the byte array for device hardware serial number.
- It is an inner part of the device ROM without the first family code byte and the last CRC byte.
- The size of the address text is determined by the constant [gbj\_ds2401::SERNUM\_LEN](#params).
- The device's serial might be considered as a <abbr title="Media Access Control">MAC</abbr> address of it.

#### Syntax
    gbj_ds2401::Sernum sernum

#### See also
[Address](#Address)

[Back to interface](#interface)


<a id="constructor"></a>
## gbj_ds2401()

#### Description
Constructor creates the class instance object.
- Constructor resets the one-wire bus.
- Constructor counts all SSNs on the bus.
- The result is available by the getter [getSSNs()](#getSSNs).

#### Syntax
    gbj_ds2401(uint8_t pinBus)

#### Parameters
<a id="prm_pinBus"></a>
- **pinBus**: Number of GPIO pin of the microcontroller managing one-wire bus.
  - *Valid values*: non-negative integer 0 to 255
  - *Default value*: none

#### Returns
Object controlling the SSN.

[Back to interface](#interface)


<a id="present"></a>
## present()

#### Description
The method initiates communication with the SSN upon provided address on the one-wire bus and checks if it is active (present) or not.

#### Syntax
    gbj_ds2401::ResultCodes present(gbj_ds2401::Address address)

#### Parameters
<a id="prm_address"></a>
- **address**: Array variable with a device ROM identifying a device.
  - *Valid values*: array of non-negative integers 0 to 255 with length defined by the constant [gbj\_ds2401::ADDRESS\_LEN](#params)
  - *Default value*: none

#### Returns
Result code from [Result and error codes](#results).
  - In case of active SSN it is [gbj\_ds2401::SUCCESS](#results).
  - In case of inactive, unaccessable SSN it is [gbj\_ds2401::ERROR\_NO\_DEVICE](#results).

[Back to interface](#interface)


<a id="SSNs"></a>
## SSNs()

#### Description
The method selects devices with DS2401 family code and are active on the one-wire bus one by one for further processing by getters and setters.
- The method returns success result code until there is an active device on the bus.
- After selecting the last device the method resets searching process.

#### Syntax
    gbj_ds2401::ResultCodes SSNs()

#### Parameters
None

#### Returns
Result code about selecting recent device on the bus defined by one of [Result and error codes](#results).

#### Example
``` cpp
gbj_ds2401 ds = gbj_ds2401(4);
void setup()
{
  while (ds.isSuccess(ds.SSNs()))
  {
    ...
  }
}
```

#### See also
[isSuccess()](#isResult)

[Back to interface](#interface)


<a id="getFamilyCode"></a>
## getFamilyCode()

#### Description
The method returns product identifier as the very first byte of the SSNs ROM.

#### Syntax
    uint8_t getFamilyCode()

#### Parameters
None

#### Returns
Device specific internal code for product identification of the device. For DS2401 it is always 01.

[Back to interface](#interface)


<a id="getId"></a>
## getId()

#### Description
The method returns identifier of the selected device as the CRC code of the device ROM from its last byte.

#### Syntax
    uint8_t getId()

#### Parameters
None

#### Returns
Short device identifier unique for all devices on the one-wire bus.

#### See also
[getFamilyCode()](#getFamilyCode)

[Back to interface](#interface)


<a id="getPin"></a>
## getPin()

#### Description
The method returns number of microcontroller pin mastering the one-wire bus.
- It is the [pinBus](#prm_pinBus) parameter of the [constructor](#constructor).

#### Syntax
    uint8_t getPin()

#### Parameters
None

#### Returns
Microcontroller pin number controlling one-wire bus.

#### See also
[gbj_ds2401()](#constructor)

[Back to interface](#interface)


<a id="getSSNs"></a>
## getSSNs()

#### Description
The method returns number of active (communicating) DS2401 electronic registration numbers on the one-wire bus.

#### Syntax
    uint8_t getSSNs()

#### Parameters
None

#### Returns
Number of DS2401 SSNs on the one-wire bus.

[Back to interface](#interface)


<a id="setLastResult"></a>
## setLastResult()

#### Description
The method sets or initializes the internal status of recent processing on the one-wire bus to input value.
- Without input parameter the method initializes internal status to success result code with class constant [gbj\_ds2401::SUCCESS](#results).
- The method without input parameter is usually called right before any operation on the bus in order to reset the internal status or in methods without bus communication.

#### Syntax
    gbj_ds2401::ResultCodes setLastResult()
    gbj_ds2401::ResultCodes setLastResult(gbj_ds2401::ResultCodes result)

#### Parameters
<a id="prm_result"></a>
- **result**: Desired result code that should be set as a recent result code.
  - *Valid values*: Some of [Result or error codes](#results).
  - *Default value*: [gbj\_ds2401::SUCCESS](#results)

#### Returns
New (actual) result code of recent operation from [Result and error codes](#results).

#### See also
[getLastResult()](#getLastResult)

[Back to interface](#interface)


<a id="getLastResult"></a>
## getLastResult()

#### Description
The method returns a result code of the recent operation on the one-wire bus. It is usually called for error handling in a sketch.

#### Syntax
    gbj_ds2401::ResultCodes getLastResult()

#### Parameters
None

#### Returns
Result code of the recent processing from [Result and error codes](#results).

#### See also
[setLastResult()](#setLastResult)

[Back to interface](#interface)


<a id="isSuccess"></a>
## isSuccess(), isError()

#### Description
The method returns a logical flag whether the recent operation on the one-wire bus was successful or failed respectivelly.
- The corresponding result code can be obtained by the method [getLastResult()]((#getLastResult).

#### Syntax
    bool isSuccess()
    bool isError()

#### Parameters
None

#### Returns
Flag about successful or failed processing of the recent operation on the bus.

#### See also
[getLastResult()](#getLastResult)

[Back to interface](#interface)


<a id="cpyAddress"></a>
## cpyAddress()

#### Description
The method copies the ROM into a provided input byte array.

#### Syntax
    void cpyAddress(gbj_ds2401::Address address)

#### Parameters
- **address**: Array variable for receiving device ROM.

#### Returns
Populated input array with device ROM.

#### Example
```cpp
gbj_ds2401::Address address;
void setup()
{
  while (ds.isSuccess(ds.SSNs()))
  {
    ds.cpyAddress(address);
  }
}
```

#### See also
[Address](#address)

[Back to interface](#interface)


<a id="cpySernum"></a>
## cpySernum()

#### Description
The method copies the current cache of the scratchpad into a provided input byte array.

#### Syntax
    void cpySernum(gbj_ds2401::Sernum sernum)

#### Parameters
- **sernum**: Array variable for receiving device serial number as a part of ROM.

#### Returns
Populated input array with device serial number.

#### Example
```cpp
gbj_ds2401::Sernum sernum;
void setup()
{
  while (ds.isSuccess(ds.SSNs()))
  {
    ds.cpySernum(sernum);
  }
}
```

#### See also
[Sernum](#Sernum)

[Back to interface](#interface)
