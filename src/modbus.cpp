#include "globals.h"

uint16_t meterType = 0x00A7;   // Meter type, initialized to 0x00AA (171) DTSU666(3phase) meter

#undef METERSEARCH
#ifdef METERSEARCH
bool meterTypeFound = false; // Flag to check if meter type has been found
int prints = 0;
#endif

void addRegisterIfNeeded(uint16_t reg, uint16_t value = 0)
{
  if (!modbus.Hreg(reg))
  {
    modbus.addHreg(reg, value);
    if (logModbusRegisterActions)
      dpf("[Modbus] Added register %d\n", reg);
  }
}

Modbus::ResultCode cbPreRequest(Modbus::FunctionCode fc, const Modbus::RequestData data)
{
#ifdef METERSEARCH
  static bool printed = false;
  if (data.reg.address == METER_TYPE || data.reg.address == 0x38f)
  {
    if (data.reg.address == METER_TYPE && !meterTypeFound)
    {
      modbus.Hreg(METER_TYPE, meterType);
      dpf("[Modbus] Meter type: 0x%04.4X (%d)\n", meterType, meterType);
      meterType++;

      // Reg: 0x0028, Count: 2 MeterType=0x0047 (327)
      // Reg: 0x003E, Count: 2 MeterType=0x0047 (327)
      // Reg: 0x0106, Count: 2 MeterType=0x0067 (104)
      // Reg: 0x000E, Count: 1 MeterType=0x00A0 (161)
      // Reg: 0x013E, Count: 1 MeterType=0x00A4 (165)
      // Reg: 0x2004, Count: 2 MeterType=0x00A6 (104) DDSU666(1phase) meter
      // Reg: 0x2012, Count: 8 MeterType=0x00A7 (423) DTSU666(3phase) meter
      // Reg: 0x000C, Count: 2 MeterType=0x00A8 (169)
      // Reg: 0x2012, Count: 16 MeterType=0x00AA (171) DTSU666(3phase) meter
    }
    printed = true;
    return Modbus::EX_SUCCESS;
  }
  meterTypeFound = true; // Set the flag to true after the first request
  if (prints++ > 10)
  {
    return Modbus::EX_SUCCESS; // Limit the number of prints to avoid flooding
  }
#endif
  // dpf("PRE Function: %02X\n", fc);
  if (fc == Modbus::FC_READ_REGS || fc == Modbus::FC_READ_INPUT_REGS)
  {
#ifdef METERSEARCH
    dpf("PRE Read Request: Type: %d Reg: 0x%04.4" PRIX16 ", Count: %d MeterType=0x%04.4X (%d)\n", data.regRead.type, data.regRead.address, data.regReadCount, meterType-1, meterType-1);
#else
    dpf("PRE Read Request: Type: %d Reg: 0x%04.4" PRIX16 ", Count: %d\n", data.regRead.type, data.regRead.address, data.regReadCount);
#endif
  }
  if (fc == Modbus::FC_WRITE_REGS)
  {
    dpf("PRE Write Request: Type: %d Reg: 0x%04.4" PRIX16 ", Count: %d\n", data.regWrite.type, data.regWrite.address, data.regWriteCount);
  }

  return Modbus::EX_SUCCESS;
}

Modbus::ResultCode cbRequestSuccess(Modbus::FunctionCode fc, const Modbus::RequestData data)
{
  // dpf("[Modbus] Request Success: Function: %02X, Reg: 0x%04.4" PRIX16 ", Count: %d\n", fc, data.reg.address, data.regCount);
  // dump registers
  //  for (uint16_t i = 0; i < data.regCount; i++)
  //  {
  //    dpf("[Modbus] Register Success 0x%04.4" PRIX16 ": 0x%04.4" PRIX16 "\n", data.reg.address + i, modbus.Hreg(data.reg.address + i));
  //  }

  // dpf("[Modbus] \n");
  // // Dump all registers
  // for (Registers reg : allRegisters) {
  //   dpf("[Modbus]     Register 0x%04.4" PRIX16 ": 0x%04.4" PRIX16 "\n", reg, modbus.Hreg(reg));
  // }

  return Modbus::EX_SUCCESS;
}

// Convert float to two 16-bit Modbus registers (big-endian: MSB first)
void floatToModbusRegisters(uint16_t offset, float value)
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

  modbus.Hreg(offset, registers[0]);
  modbus.Hreg(offset + 1, registers[1]);
}

void updateModbusRegisters()
{
  uint16_t *floatAsRegisters;

  if (!registersAdded)
  {
    modbus.onRequest(cbPreRequest);
    modbus.onRequestSuccess(cbRequestSuccess);

    modbus.addHreg(NETWORK_SELECTION, 0x0001); // Network selection (0: three phase four wire, 1: three phase three wire)
    modbus.addHreg(METER_TYPE, meterType); // Device type coding - Solax DTSU666 meter 170 (0x00AA)

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

  // Write float value as two 16-bit registers
  // The data is stored as a 32-bit float, split into two 16-bit registers
  // The Byte order is Floating Inverse Byte Order (FIBO) (AB CD)

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

void updateAndInitModbusMastereRegisters()
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
        modbus.begin(&Serial2);
        modbus.slave(slaveID);
        modbusInitialized = true;

        dpln("[Modbus] Server started!");
        updateModbusRegisters();
      }
    }
  }
}

void printModbusMasterStatus()
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
    dpf("[SYSTEM] Waiting for data: %d/13 fields received\n",
        meterData.total_import_rx +
            meterData.total_export_rx +
            meterData.total_power_rx +
            meterData.v1_rx + meterData.v2_rx + meterData.v3_rx +
            meterData.c1_rx + meterData.c2_rx + meterData.c3_rx +
            meterData.power_1_rx + meterData.power_2_rx + meterData.power_3_rx +
            meterData.freq_rx);

    // Print the missing fields
    if (!meterData.total_import_rx)
      dpln("Total Import not received");
    if (!meterData.total_export_rx) 
      dpln("Total Export not received");
    if (!meterData.total_power_rx)  
      dpln("Total Power not received");
    if (!meterData.v1_rx)   
      dpln("L1 Voltage not received");
    if (!meterData.v2_rx)
      dpln("L2 Voltage not received");
    if (!meterData.v3_rx)
      dpln("L3 Voltage not received");
    if (!meterData.c1_rx)
      dpln("L1 Current not received");
    if (!meterData.c2_rx)
      dpln("L2 Current not received");
    if (!meterData.c3_rx)
      dpln("L3 Current not received");
    if (!meterData.power_1_rx)
      dpln("L1 Power not received");
    if (!meterData.power_2_rx)
      dpln("L2 Power not received");
    if (!meterData.power_3_rx)
      dpln("L3 Power not received");
    if (!meterData.freq_rx)
      dpln("Frequency not received"); 
  }
}

void handleModbusMaster()
{
  // Handle Modbus with priority
  if (modbusInitialized)
  {
    modbus.task();
  }
}