#include "gbj_ds2401.h"
const String gbj_ds2401::VERSION = "GBJ_DS2401 1.0.0";

gbj_ds2401::ResultCodes gbj_ds2401::devices()
{
  setLastResult();
  // Count all devices on the bus
  _bus.SSNs = 0;
  while (search(_rom.buffer))
  {
    if (getFamilyCode() == Limits::DS2401 &&
        _rom.address.crc == crc8(_rom.buffer, Params::ADDRESS_LEN - 1))
    {
      _bus.SSNs++;
    }
  }
  reset_search();
  if (_bus.SSNs == 0)
    setLastResult(ResultCodes::ERROR_NO_DEVICE);
  return getLastResult();
}

gbj_ds2401::ResultCodes gbj_ds2401::SSNs()
{
  static uint8_t iterations;
  while (search(_rom.buffer))
  {
    if (getFamilyCode() != Limits::DS2401)
      continue;
    iterations++;
    return getLastResult();
  }
  if (iterations)
  {
    setLastResult(ResultCodes::END_OF_LIST);
  }
  else
  {
    setLastResult(ResultCodes::ERROR_NO_DEVICE);
  }
  iterations = 0;
  reset_search();
  return getLastResult();
}

gbj_ds2401::ResultCodes gbj_ds2401::present(const Address address)
{
  bool isPresent = false;
  target_search(Limits::DS2401);
  while (search(_rom.buffer))
  {
    if (_rom.address.crc == crc8(_rom.buffer, Params::ADDRESS_LEN - 1) &&
        _rom.address.crc == address[Params::ADDRESS_LEN - 1])
    {
      isPresent = true;
      break;
    }
  }
  reset_search();
  setLastResult(ResultCodes::ERROR_NO_DEVICE);
  if (isPresent)
    setLastResult(ResultCodes::SUCCESS);
  return getLastResult();
}

gbj_ds2401::ResultCodes gbj_ds2401::cpyRom(const Address address)
{
  setLastResult();
  resetRom();
  // Check ROM CRC
  if (address[Params::ADDRESS_LEN - 1] !=
      crc8(address, Params::ADDRESS_LEN - 1))
    return setLastResult(ResultCodes::ERROR_CRC_ADDRESS);
  // Copy address to buffer
  memcpy(_rom.buffer, address, Params::ADDRESS_LEN);
  return getLastResult();
}