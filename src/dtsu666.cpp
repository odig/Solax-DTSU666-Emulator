#include "globals.h"
#include "dtsu666.h"

DTSU666::DTSU666(int id) : ModbusRTU()
{
  modbusInitialized = false;
  slaveId = id; // Set the slave ID for the Modbus RTU client
}

void DTSU666::addRegisterIfNeeded(uint16_t reg, uint16_t value)
{
  if (!Hreg(reg))
  {
    addHreg(reg, value);
    if (logModbusRegisterActions)
      dpf("[Modbus] Added register %d\n", reg);
  }
}

DTSU666::ResultCode DTSU666::cbPreRequest(FunctionCode fc, const RequestData data)
{
#ifdef METERSEARCH
  static bool printed = false;
  if (data.reg.address == METER_TYPE || data.reg.address == 0x38f)
  {
    if (data.reg.address == METER_TYPE && !meterTypeFound)
    {
      Hreg(METER_TYPE, meterType);
      dpf("[Modbus] Meter type: 0x%04.4X (%d)\n", meterType, meterType);
      meterType++;
    }
    printed = true;
    return EX_SUCCESS;
  }
  meterTypeFound = true;
  if (prints++ > 10)
  {
    return EX_SUCCESS;
  }
#endif
  if (fc == FC_READ_REGS || fc == FC_READ_INPUT_REGS)
  {
#ifdef METERSEARCH
    dpf("PRE Read Request: Type: %d Reg: 0x%04.4" PRIX16 ", Count: %d MeterType=0x%04.4X (%d)\n", data.regRead.type, data.regRead.address, data.regReadCount, meterType - 1, meterType - 1);
#else
    dpf("PRE Read Request: Type: %d Reg: 0x%04.4" PRIX16 ", Count: %d\n", data.regRead.type, data.regRead.address, data.regReadCount);
#endif
  }
  if (fc == FC_WRITE_REGS)
  {
    dpf("PRE Write Request: Type: %d Reg: 0x%04.4" PRIX16 ", Count: %d\n", data.regWrite.type, data.regWrite.address, data.regWriteCount);
  }

  return EX_SUCCESS;
}

DTSU666::ResultCode DTSU666::cbRequestSuccess(FunctionCode fc, const RequestData data)
{
  return EX_SUCCESS;
}

void DTSU666::floatToModbusRegisters(uint16_t offset, float value)
{
  uint16_t registers[2];
  union
  {
    float f;
    uint8_t bytes[4];
  } floatUnion;

  addRegisterIfNeeded(offset);
  addRegisterIfNeeded(offset + 1);

  floatUnion.f = value;

  registers[0] = (floatUnion.bytes[3] << 8) | floatUnion.bytes[2];
  registers[1] = (floatUnion.bytes[1] << 8) | floatUnion.bytes[0];

  Hreg(offset, registers[0]);
  Hreg(offset + 1, registers[1]);
}

void DTSU666::updateModbusRegisters()
{
  if (!meterData.allDataReceived())
  {
    dpln("[Modbus] Waiting for meter data...");
    return;
  }

  if (!registersAdded)
  {
    onRequest([this](FunctionCode fc, const RequestData data)
              { return this->cbPreRequest(fc, data); });
    onRequestSuccess([this](FunctionCode fc, const RequestData data)
                     { return this->cbRequestSuccess(fc, data); });

    addHreg(NETWORK_SELECTION, 0x0001);
    addHreg(METER_TYPE, meterType);

    floatToModbusRegisters(REACTIVE_POWER_A, 0.0);
    floatToModbusRegisters(REACTIVE_POWER_B, 0.0);
    floatToModbusRegisters(REACTIVE_POWER_C, 0.0);
    floatToModbusRegisters(TOTAL_REACTIVE_POWER, 0.0);

    floatToModbusRegisters(POWER_FACTOR_A, 0.001);
    floatToModbusRegisters(POWER_FACTOR_B, 0.001);
    floatToModbusRegisters(POWER_FACTOR_C, 0.001);
    floatToModbusRegisters(TOTAL_POWER_FACTOR, 0.001);

    registersAdded = true;

    dpln("[Modbus] Initialized registers");
  }

  floatToModbusRegisters(VOLTAGE_AB, meterData.v1 * 10.0);
  floatToModbusRegisters(VOLTAGE_BC, meterData.v2 * 10.0);
  floatToModbusRegisters(VOLTAGE_CA, meterData.v3 * 10.0);

  floatToModbusRegisters(VOLTAGE_A, meterData.v1 * 10.0);
  floatToModbusRegisters(VOLTAGE_B, meterData.v2 * 10.0);
  floatToModbusRegisters(VOLTAGE_C, meterData.v3 * 10.0);

  floatToModbusRegisters(CURRENT_A, meterData.c1 * 1000.0);
  floatToModbusRegisters(CURRENT_B, meterData.c2 * 1000.0);
  floatToModbusRegisters(CURRENT_C, meterData.c3 * 1000.0);

  floatToModbusRegisters(ACTIVE_POWER_A, meterData.power_1 * 10.0);
  floatToModbusRegisters(ACTIVE_POWER_B, meterData.power_2 * 10.0);
  floatToModbusRegisters(ACTIVE_POWER_C, meterData.power_3 * 10.0);

  floatToModbusRegisters(FREQUENCY, meterData.freq * 100.0);

  floatToModbusRegisters(TOTAL_ACTIVE_POWER, meterData.total_power * 10.0);
  floatToModbusRegisters(TOTAL_FORWARD, meterData.total_import);
  floatToModbusRegisters(TOTAL_REVERSE, meterData.total_export);
}

void DTSU666::update()
{
  if (enableModbus)
  {
    if (modbusInitialized)
      updateModbusRegisters();
    else
    {
      if (meterData.allDataReceived())
      {
        Serial2.begin(modbusBaudRate, SERIAL_CONFIG, RX_PIN, TX_PIN);
        begin(&Serial2);
        slave(slaveId);
        modbusInitialized = true;

        dpf("[Modbus] Server started! for slave ID %d\n", slaveId);
        updateModbusRegisters();
      }
    }
  }
}

void DTSU666::printStatus()
{
  if (modbusInitialized)
  {
    static bool printed = false;
    if (!printed)
    {
      dpln("[SYSTEM] Modbus operational");
      printed = true;
    }
  }
  else
  {
    meterData.printMeterReceivedStatus();
  }
}

void DTSU666::handle()
{
  if (modbusInitialized)
  {
    task();
  }
}
