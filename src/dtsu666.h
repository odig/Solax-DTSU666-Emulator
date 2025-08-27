#pragma once

#include "globals.h"

//#define METERSEARCH // Uncomment to enable meter search mode


enum Registers
{
  // Configuration
  NETWORK_SELECTION = 0x0003, // Network selection (0:three phase four wire, 1:three phase three wire)
  METER_TYPE = 0x000B,        // Device type coding - Solax DTSU666 meter 166 (0x00A6)

  // Additional values (optional)
  VOLTAGE_AB = 0x2000, // Line-to-line AB
  VOLTAGE_BC = 0x2002, // Line-to-line BC
  VOLTAGE_CA = 0x2004, // Line-to-line CA

  // Voltages (Phase)
  VOLTAGE_A = 0x2006, // Voltage L1 (Phase A)
  VOLTAGE_B = 0x2008, // Voltage L2 (Phase B)
  VOLTAGE_C = 0x200A, // Voltage L3 (Phase C)

  // Currents
  CURRENT_A = 0x200C, // Current Phase A
  CURRENT_B = 0x200E, // Current Phase B
  CURRENT_C = 0x2010, // Current Phase C

  // Active Power (W)
  TOTAL_ACTIVE_POWER = 0x2012,   // Total System Active Power
  ACTIVE_POWER_A = 0x2014,       // Active Power Phase A
  ACTIVE_POWER_B = 0x2016,       // Active Power Phase B
  ACTIVE_POWER_C = 0x2018,       // Active Power Phase C
  TOTAL_REACTIVE_POWER = 0x201A, // Total System Reactive Power
  REACTIVE_POWER_A = 0x201C,     // Reactive Power Phase A
  REACTIVE_POWER_B = 0x201E,     // Reactive Power Phase B
  REACTIVE_POWER_C = 0x2020,     // Reactive Power Phase C
  TOTAL_POWER_FACTOR = 0x202A,   // Total System Power Factor
  POWER_FACTOR_A = 0x202C,       // Power Factor Phase A
  POWER_FACTOR_B = 0x202E,       // Power Factor Phase B
  POWER_FACTOR_C = 0x2030,       // Power Factor Phase C

  // Frequency
  FREQUENCY = 0x2044, // Grid frequency

  // Total Active Energy (kWh)
  TOTAL_FORWARD = 0x101E, // Total import energy
  TOTAL_REVERSE = 0x1028, // Total export energy
};

constexpr Registers allRegisters[] = {
    METER_TYPE,
    VOLTAGE_AB, VOLTAGE_BC, VOLTAGE_CA,
    VOLTAGE_A, VOLTAGE_B, VOLTAGE_C,
    TOTAL_REACTIVE_POWER,
    REACTIVE_POWER_A, REACTIVE_POWER_B, REACTIVE_POWER_C,
    TOTAL_POWER_FACTOR,
    POWER_FACTOR_A, POWER_FACTOR_B, POWER_FACTOR_C,
    FREQUENCY,
    TOTAL_FORWARD,
    TOTAL_REVERSE};

typedef struct _MeterData
{
  float total_import = 0;       // Total imported energy in kWh
  float total_export = 0;       // Total exported energy in kWh
  float total_power = 0;        // Current total power consumption in W
  float total_power_factor = 0; // Total system power factor
  float v1 = 0;                 // Voltage of Phase A in V
  float v2 = 0;                 // Voltage of Phase B in V
  float v3 = 0;                 // Voltage of Phase C in V
  float c1 = 0;                 // Current of Phase A in A
  float c2 = 0;                 // Current of Phase B in A
  float c3 = 0;                 // Current of Phase C in A
  float pf1 = 0;                // Power factor of Phase A
  float pf2 = 0;                // Power factor of Phase B
  float pf3 = 0;                // Power factor of Phase C
  float freq = 0;               // Grid frequency in Hz
  float power_1 = 0;            // Active power of Phase A in W
  float power_2 = 0;            // Active power of Phase B in W
  float power_3 = 0;            // Active power of Phase C in W
  float l1_import = 0;          // L1 imported energy in kWh
  float l2_import = 0;          // L2 imported energy in kWh
  float l3_import = 0;          // L3 imported energy in kWh
  float l1_export = 0;          // L1 exported energy in kWh
  float l2_export = 0;          // L2 exported energy in kWh
  float l3_export = 0;          // L3 exported energy in kWh

  bool total_import_rx = false;
  bool total_export_rx = false;
  bool total_power_rx = false;
  bool v1_rx = false;
  bool v2_rx = false;
  bool v3_rx = false;
  bool c1_rx = false;
  bool c2_rx = false;
  bool c3_rx = false;
  bool freq_rx = false;
  bool power_1_rx = false;
  bool power_2_rx = false;
  bool power_3_rx = false;
  bool l1_import_rx = true;
  bool l2_import_rx = true;
  bool l3_import_rx = true;
  bool l1_export_rx = true;
  bool l2_export_rx = true;
  bool l3_export_rx = true;

  bool allDataReceived()
  {
    return total_import_rx &&
           total_export_rx &&
           total_power_rx &&
           v1_rx && v2_rx && v3_rx &&
           c1_rx && c2_rx && c3_rx &&
           power_1_rx && power_2_rx && power_3_rx &&
           l1_import_rx && l2_import_rx && l3_import_rx &&
           l1_export_rx && l2_export_rx && l3_export_rx &&
           freq_rx;
  }

  int meterDataReceivedCount()
  {
    return total_import_rx +
           total_export_rx +
           total_power_rx +
           v1_rx + v2_rx + v3_rx +
           c1_rx + c2_rx + c3_rx +
           power_1_rx + power_2_rx + power_3_rx +
           l1_import_rx + l2_import_rx + l3_import_rx +
           l1_export_rx + l2_export_rx + l3_export_rx +
           freq_rx;
  }

  void initDataAndSetToReceived()
  {
    total_import = 0;
    total_export = 0;
    total_power = 0;
    v1 = 0;
    v2 = 0;
    v3 = 0;
    c1 = 0;
    c2 = 0;
    c3 = 0;
    pf1 = 0;
    pf2 = 0;
    pf3 = 0;
    freq = 0;
    power_1 = 0;
    power_2 = 0;
    power_3 = 0;
    l1_import = 0;
    l2_import = 0;
    l3_import = 0;
    l1_export = 0;
    l2_export = 0;
    l3_export = 0;

    total_import_rx = true; 
    total_export_rx = true; 
    total_power_rx = true; 
    v1_rx = true; 
    v2_rx = true; 
    v3_rx = true; 
    c1_rx = true; 
    c2_rx = true; 
    c3_rx = true; 
    freq_rx = true; 
    power_1_rx = true; 
    power_2_rx = true; 
    power_3_rx = true; 
    l1_import_rx = true; 
    l2_import_rx = true; 
    l3_import_rx = true; 
    l1_export_rx = true; 
    l2_export_rx = true; 
    l3_export_rx = true;
  }

  void printMeterReceivedStatus() {
    dpf("[SYSTEM] Waiting for data: %d/19 fields received state is: %s\n",
      meterDataReceivedCount(),
      allDataReceived() ? "OK" : "NOT OK"    
    );

    // Print the missing fields
    if (!total_import_rx)
      dpln("Total Import not received");
    if (!total_export_rx) 
      dpln("Total Export not received");
    if (!total_power_rx)  
      dpln("Total Power not received");
    if (!v1_rx)   
      dpln("L1 Voltage not received");
    if (!v2_rx)
      dpln("L2 Voltage not received");
    if (!v3_rx)
      dpln("L3 Voltage not received");
    if (!c1_rx)
      dpln("L1 Current not received");
    if (!c2_rx)
      dpln("L2 Current not received");
    if (!c3_rx)
      dpln("L3 Current not received");
    if (!power_1_rx)
      dpln("L1 Power not received");
    if (!power_2_rx)
      dpln("L2 Power not received");
    if (!power_3_rx)
      dpln("L3 Power not received");
    if (!l1_import_rx)  
      dpln("L1 Import not received");
    if (!l2_import_rx)  
      dpln("L2 Import not received");
    if (!l3_import_rx)    
      dpln("L3 Import not received");
    if (!l1_export_rx)
      dpln("L1 Export not received");
    if (!l2_export_rx)  
      dpln("L2 Export not received");
    if (!l3_export_rx)
      dpln("L3 Export not received");
    if (!freq_rx)
      dpln("Frequency not received"); 
  }
} MeterData;

class DTSU666 : private ModbusRTU
{
public:
    bool modbusInitialized;
    MeterData meterData;
    DTSU666(int id);
    void update();
    void printStatus();
    void handle();

private:
    #ifdef METERSEARCH
        bool meterTypeFound = false; // Flag to check if meter type has been found
        int prints = 0;
    #endif

    uint16_t meterType = 0x00A7; // Meter type, initialized to 0x00AA (171) DTSU666(3phase) meter
    int slaveId = 1; // Slave ID of the meter, initialized to 0

    void addRegisterIfNeeded(uint16_t reg, uint16_t value = 0);
    ResultCode cbPreRequest(FunctionCode fc, const RequestData data);
    ResultCode cbRequestSuccess(FunctionCode fc, const RequestData data);
    void floatToModbusRegisters(uint16_t offset, float value);
    void updateModbusRegisters();
};
