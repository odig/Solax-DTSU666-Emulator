
#include "globals.h"
#include "victron.h"

void Victron::victronConnect()
{
    if (victron.isConnected(gxIP))
    {
        victronConnected = true;
        return;
    }

    victronConnected = false;

    dpln("Not connected. Trying to connect..");
    victron.connect(gxIP, 502);
}

// | Service Name           | Description                  | Address | Type      | Scale | Range                     | Object Path                | Writable | Unit  |
// |------------------------|------------------------------|---------|-----------|-------|---------------------------|----------------------------|----------|-------|
// | com.victronenergy.grid | Grid L1 - Power              | 2600    | int16     | 1     | -32768 to 32767           | /Ac/L1/Power               | no       | W     |
// | com.victronenergy.grid | Grid L2 - Power              | 2601    | int16     | 1     | -32768 to 32767           | /Ac/L2/Power               | no       | W     |
// | com.victronenergy.grid | Grid L3 - Power              | 2602    | int16     | 1     | -32768 to 32767           | /Ac/L3/Power               | no       | W     |
// | com.victronenergy.grid | Grid L1 - Energy from net    | 2603    | uint16    | 100   | 0 to 655.35               | /Ac/L1/Energy/Forward      | no       | kWh   |
// | com.victronenergy.grid | Grid L2 - Energy from net    | 2604    | uint16    | 100   | 0 to 655.35               | /Ac/L2/Energy/Forward      | no       | kWh   |
// | com.victronenergy.grid | Grid L3 - Energy from net    | 2605    | uint16    | 100   | 0 to 655.35               | /Ac/L3/Energy/Forward      | no       | kWh   |
// | com.victronenergy.grid | Grid L1 - Energy to net      | 2606    | uint16    | 100   | 0 to 655.35               | /Ac/L1/Energy/Reverse      | no       | kWh   |
// | com.victronenergy.grid | Grid L2 - Energy to net      | 2607    | uint16    | 100   | 0 to 655.35               | /Ac/L2/Energy/Reverse      | no       | kWh   |
// | com.victronenergy.grid | Grid L3 - Energy to net      | 2608    | uint16    | 100   | 0 to 655.35               | /Ac/L3/Energy/Reverse      | no       | kWh   |
// | com.victronenergy.grid | Serial                       | 2609    | string[7] |       | 14 characters             | /Serial                    | no       |       |
// | com.victronenergy.grid | Grid L1 - Voltage            | 2616    | uint16    | 10    | 0 to 6553.5               | /Ac/L1/Voltage             | no       | V     |
// | com.victronenergy.grid | Grid L1 - Current            | 2617    | int16     | 10    | -3276.8 to 3276.7         | /Ac/L1/Current             | no       | A     |
// | com.victronenergy.grid | Grid L2 - Voltage            | 2618    | uint16    | 10    | 0 to 6553.5               | /Ac/L2/Voltage             | no       | V     |
// | com.victronenergy.grid | Grid L2 - Current            | 2619    | int16     | 10    | -3276.8 to 3276.7         | /Ac/L2/Current             | no       | A     |
// | com.victronenergy.grid | Grid L3 - Voltage            | 2620    | uint16    | 10    | 0 to 6553.5               | /Ac/L3/Voltage             | no       | V     |
// | com.victronenergy.grid | Grid L3 - Current            | 2621    | int16     | 10    | -3276.8 to 3276.7         | /Ac/L3/Current             | no       | A     |
// | com.victronenergy.grid | Grid L1 - Energy from net    | 2622    | uint32    | 100   | 0 to 42949672.96          | /Ac/L1/Energy/Forward      | no       | kWh   |
// | com.victronenergy.grid | Grid L2 - Energy from net    | 2624    | uint32    | 100   | 0 to 42949672.96          | /Ac/L2/Energy/Forward      | no       | kWh   |
// | com.victronenergy.grid | Grid L3 - Energy from net    | 2626    | uint32    | 100   | 0 to 42949672.96          | /Ac/L3/Energy/Forward      | no       | kWh   |
// | com.victronenergy.grid | Grid L1 - Energy to net      | 2628    | uint32    | 100   | 0 to 42949672.96          | /Ac/L1/Energy/Reverse      | no       | kWh   |
// | com.victronenergy.grid | Grid L2 - Energy to net      | 2630    | uint32    | 100   | 0 to 42949672.96          | /Ac/L2/Energy/Reverse      | no       | kWh   |
// | com.victronenergy.grid | Grid L3 - Energy to net      | 2632    | uint32    | 100   | 0 to 42949672.96          | /Ac/L3/Energy/Reverse      | no       | kWh   |
// | com.victronenergy.grid | Total Energy from net        | 2634    | uint32    | 100   | 0 to 42949672.96          | /Ac/Energy/Forward         | no       | kWh   |
// | com.victronenergy.grid | Total Energy to net          | 2636    | uint32    | 100   | 0 to 42949672.96          | /Ac/Energy/Reverse         | no       | kWh   |
// | com.victronenergy.grid | Grid L1 - Power              | 2638    | int32     | 1     | -2147483648 to 2147483648 | /Ac/L1/Power               | no       | W     |
// | com.victronenergy.grid | Grid L2 - Power              | 2640    | int32     | 1     | -2147483648 to 2147483648 | /Ac/L2/Power               | no       | W     |
// | com.victronenergy.grid | Grid L3 - Power              | 2642    | int32     | 1     | -2147483648 to 2147483648 | /Ac/L3/Power               | no       | W     |
// | com.victronenergy.grid | AC Frequency                 | 2644    | uint16    | 100   | 0 to 655.35               | /Ac/Frequency              | no       | Hz    |

bool Victron::hregCallback(Modbus::ResultCode result, uint16_t transactionId, void *arg)
{
    //MeterData meterData;

    if (result != Modbus::EX_SUCCESS)
    {
        dpf("[Victron] Error reading registers: %d\n", result);
        return false;
    }

    // Process the data as needed
    uint16_t *udata = (uint16_t *)regBuff;
    int16_t *idata = (int16_t *)regBuff;
    dpf("[Victron] Registers read successfully for transaction ID: %d\n", transactionId);
    for (size_t i = 0; i < sizeof(regBuff) / sizeof(uint16_t); ++i)
    {
        int registerIndex = i + 2600; // Adjust index to match register addresses
        switch (registerIndex)
        {
        case 2600: // L1 Power
        {
         dtsu666.meterData.power_1 = idata[i] / 1.0f; // Scale as needed
         dtsu666.meterData.power_1_rx = true;
            break;
        }
        case 2601: // L2 Power
        {
         dtsu666.meterData.power_2 = idata[i] / 1.0f; // Scale as needed
         dtsu666.meterData.power_2_rx = true;
            break;
        }
        case 2602: // L3 Power
        {
         dtsu666.meterData.power_3 = idata[i] / 1.0f; // Scale as needed
         dtsu666.meterData.power_3_rx = true;
            break;
        }
        case 2616: // L1 Voltage
        {
         dtsu666.meterData.v1 = udata[i] / 10.0f; // Scale as needed
         dtsu666.meterData.v1_rx = true;
            break;
        }
        case 2617: // L1 Current
        {
         dtsu666.meterData.c1 = idata[i] / 10.0f; // Scale as needed
         dtsu666.meterData.c1_rx = true;
            break;
        }
        case 2618: // L2 Voltage
        {
         dtsu666.meterData.v2 = udata[i] / 10.0f; // Scale as needed
         dtsu666.meterData.v2_rx = true;
            break;
        }
        case 2619: // L2 Current
        {
         dtsu666.meterData.c2 = idata[i] / 10.0f; // Scale as needed
         dtsu666.meterData.c2_rx = true;
            break;
        }
        case 2620: // L3 Voltage
        {
         dtsu666.meterData.v3 = udata[i] / 10.0f; // Scale as needed
         dtsu666.meterData.v3_rx = true;
            break;
        }
        case 2621: // L3 Current
        {
         dtsu666.meterData.c3 = idata[i] / 10.0f; // Scale as needed
         dtsu666.meterData.c3_rx = true;
            break;
        }
        case 2622: // L1 Energy from net
        {
            uint32_t l1Import = (udata[i + 1] + (udata[i] << 16)); // Scale as needed
         dtsu666.meterData.l1_import = l1Import / 100.0f;               // Scale as needed
         dtsu666.meterData.l1_import_rx = true;
            break;
        }
        case 2624: // L2 Energy from net
        {
            uint32_t l2Import = (udata[i + 1] + (udata[i] << 16)); // Scale as needed
         dtsu666.meterData.l2_import = l2Import / 100.0f;               // Scale as needed
         dtsu666.meterData.l2_import_rx = true;
            break;
        }
        case 2626: // L3 Energy from net
        {
            uint32_t l3Import = (udata[i + 1] + (udata[i] << 16)); // Scale as needed
         dtsu666.meterData.l3_import = l3Import / 100.0f;               // Scale as needed
         dtsu666.meterData.l3_import_rx = true;
            break;
        }
        case 2628: // L1 Energy to net
        {
            uint32_t l1Export = (udata[i + 1] + (udata[i] << 16)); // Scale as needed
         dtsu666.meterData.l1_export = l1Export / 100.0f;               // Scale as needed
         dtsu666.meterData.l1_export_rx = true;
            break;
        }
        case 2630: // L2 Energy to net
        {
            uint32_t l2Export = (udata[i + 1] + (udata[i] << 16)); // Scale as needed
         dtsu666.meterData.l2_export = l2Export / 100.0f;               // Scale as needed
         dtsu666.meterData.l2_export_rx = true;
            break;
        }
        case 2632: // L3 Energy to net
        {
            uint32_t l3Export = (udata[i + 1] + (udata[i] << 16)); // Scale as needed
         dtsu666.meterData.l3_export = l3Export / 100.0f;               // Scale as needed
         dtsu666.meterData.l3_export_rx = true;
            break;
        }
        case 2634: // Total Energy from net
        {
            uint32_t totalImport = (udata[i + 1] + (udata[i] << 16)); // Scale as needed
         dtsu666.meterData.total_import = totalImport / 100.0f;            // Scale as needed
         dtsu666.meterData.total_import_rx = true;
            break;
        }
        case 2636: // Total Energy to net
        {
            uint32_t totalExport = (udata[i + 1] + (udata[i] << 16)); // Scale as needed
         dtsu666.meterData.total_export = totalExport / 100.0f;            // Scale as needed
         dtsu666.meterData.total_export_rx = true;
            break;
        }
        case 2644: // Frequency
        {
         dtsu666.meterData.freq = udata[i] / 100.0f; // Scale as needed
         dtsu666.meterData.freq_rx = true;
            break;
        }
        }
        if (dtsu666.meterData.power_1_rx || dtsu666.meterData.power_2_rx || dtsu666.meterData.power_3_rx)
        {
         dtsu666.meterData.total_power = dtsu666.meterData.power_1 + dtsu666.meterData.power_2 + dtsu666.meterData.power_3;
         dtsu666.meterData.total_power_rx = true;
        }
    }

    // Debug output
    dpln("=== Meter Data ===");
    dpf("Import: %.2f kWh | Export: %.2f kWh\n", dtsu666.meterData.total_import, dtsu666.meterData.total_export);
    dpf("Power: %.1f W\n", dtsu666.meterData.total_power);
    dpf("Voltage: %.1f V | %.1f V | %.1f V\n", dtsu666.meterData.v1, dtsu666.meterData.v2, dtsu666.meterData.v3);
    dpf("Current: %.1f A | %.1f A | %.1f A\n", dtsu666.meterData.c1, dtsu666.meterData.c2, dtsu666.meterData.c3);
    dpf("Active Power: %.1f W | %.1f W | %.1f W\n", dtsu666.meterData.power_1, dtsu666.meterData.power_2, dtsu666.meterData.power_3);
    // dpf("Power Factor: %.2f\n", meterData.total_power_factor);
    // dpf("Power Factor: %.2f | %.2f | %.2f\n", meterData.pf1, meterData.pf2, meterData.pf3);
    dpf("Frequency: %.2f Hz\n\n", dtsu666.meterData.freq);

    dtsu666.update();

    return false;
}

void Victron::readVictronModBus()
{
    const uint8_t slaveID = 30; // EM540 typically uses ID 30

    victronConnect(); // Ensure connection to Victron GX device

    if (!victronConnected)
    {
        return;
    }

    dpf("[Victron] Reading Modbus registers %p %d ..\n", regBuff, sizeof(regBuff) / sizeof(uint16_t));
    readHreg(
        gxIP, 
        2600, 
        regBuff, 
        sizeof(regBuff) / sizeof(uint16_t), 
        [this](Modbus::ResultCode result, uint16_t transactionId, void *arg) { return this->hregCallback(result, transactionId, arg); },
        slaveID); // Read all registers
}

void Victron::handleVictronModbusSlave()
{
    static unsigned long lastVictronModbus = 0;
    if (millis() - lastVictronModbus > 3000)
    {
        lastVictronModbus = millis();
        readVictronModBus();
    }

    task();
}

void Victron::setupVictronModbusSlave()
{
    bool rc;
    client();
}
