# Solax-DTSU666-Emulator

**RS485 Emulator using XY-017 Auto Flow Control Module or similar**

An ESP32-based DTSU666 smart meter emulator with automatic RS485 direction control for industrial IoT applications.
Strongly influenced by [Sungrow_DTSU666_emulator](https://github.com/Egyras/Sungrow_DTSU666_emulator).

## Features
- ESP32-optimized Modbus RTU implementation
- Automatic TX/RX switching with XY-017 module
- Victron MQTT OR ModbusTCP as data source
- OTA update option
- Status website


## Hardware Requirements
- ESP32 development board
- XY-017 TTL to RS485 Converter (Auto Flow Control)
- RS485 bus wiring (A/B lines + GND)
- 3.3V power supply for XY-017 module

## Wiring (ESP32 + XY-017)
| ESP32         | XY-017 Module |
|---------------|---------------|
| GPIO17 (TX2)  | TXD            |
| GPIO16 (RX2)  | RXD            |
| 3.3V          | VCC           |
| GND           | GND           |


## Libraries Used
- **ModbusRTU** by [emelianov]
- **ModbusTCP** by [emelianov]
- **PubSubClient** by [Nick O'Leary]
- **ArduinoJSON** by [Benoit Blanchon]
- **ESP32 Arduino Core** (v2.0.0+)

## ESP32 Specific Configuration
```cpp
// settings.h
const int RX_PIN = 16;  // UART2 RX (GPIO16)
const int TX_PIN = 17;  // UART2 TX (GPIO17)

// Modbus Serial Configuration
const long modbusBaud = 9600;
const int SERIAL_CONFIG = SERIAL_8N1;
```
## Installation
1. Clone this repository
2. Install required libraries through Arduino Library Manager
3. Copy `settings.cpp.sample` to `settings.cpp` and configure `settings.cpp` with your network and MQTT credentials
4. Upload to ESP32
5. Connect RS485 module to TTL pins (default: GPIO16-RX, GPIO17-TX)

## Usage
1. Power up the device
2. Monitor serial port (115200 baud) for initialization status

**MQTT Topics:**
- `smartmeter/data` - Meter data in JSON format
- `smartmeter/status` - Device status (online/offline)

*** Victron topics need to be replaced. There are defined in `settings.h`. Using ModbusTCP is easier.
- `victron/N/XXXXX/grid/YY/Ac/Energy/Forward`
- `victron/N/XXXXX/grid/YY/Ac/Energy/Reverse`
- `victron/N/XXXXX/grid/YY/Ac/Power`
- `victron/N/XXXXX/grid/YY/Ac/L1/Voltage`
- `victron/N/XXXXX/grid/YY/Ac/L2/Voltage`
- `victron/N/XXXXX/grid/YY/Ac/L3/Voltage`
- `victron/N/XXXXX/grid/YY/Ac/L1/Current`
- `victron/N/XXXXX/grid/YY/Ac/L2/Current`
- `victron/N/XXXXX/grid/YY/Ac/L3/Current`
- `victron/N/XXXXX/grid/YY/Ac/L1/Power`
- `victron/N/XXXXX/grid/YY/Ac/L2/Power`
- `victron/N/XXXXX/grid/YY/Ac/L3/Power`
- `victron/N/XXXXX/grid/YY/Ac/Frequency`

**Sample JSON Payload:**
```json
{
  "SML": {
    "total_kwh": 1234,
    "export_total_kwh": 789,
    "Power_curr": 1500,
    "volt_p1": 230,
    "volt_p2": 231,
    "volt_p3": 229,
    "freq": 50,
    "Power_A": 500,
    "Power_B": 600,
    "Power_C": 400
  }
}
```

## Findings
After the Solax inverter reads Holding Register 0xB, it reads additional registers depending on the lowest byte.
This suggests that it uses the content of 0xB to identify the smart meter type.
After tinkering arround I found following behaviour: 

- Register 0x000B (11) MeterType=0x0047 (327) Solax reads Register: 0x0028, Count: 2
- Register 0x000B (11) MeterType=0x0047 (327) Solax reads Register: 0x003E, Count: 2
- Register 0x000B (11) MeterType=0x0067 (104) Solax reads Register: 0x0106, Count: 2
- Register 0x000B (11) MeterType=0x00A0 (161) Solax reads Register: 0x000E, Count: 1
- Register 0x000B (11) MeterType=0x00A4 (165) Solax reads Register: 0x013E, Count: 1
- Register 0x000B (11) MeterType=0x00A6 (104) Solax reads Register: 0x2004, Count: 2 DDSU666(1phase) meter
- Register 0x000B (11) MeterType=0x00A7 (423) Solax reads Register: 0x2012, Count: 8 DTSU666(3phase) meter
- Register 0x000B (11) MeterType=0x00A8 (169) Solax reads Register: 0x000C, Count: 2
- Register 0x000B (11) MeterType=0x00AA (171) Solax reads Register: 0x2012, Count: 16 DTSU666(3phase) meter

## Related Projects

For alternative implementations and community-driven approaches to Sungrow meter communication, you might want to explore:
- [Sungrow-Meter-cheater](https://github.com/Linux-RISC/Sungrow-Meter-cheater)
- [Sungrow_DTSU666_emulator](https://github.com/Egyras/Sungrow_DTSU666_emulator)

## Links
- [Solax Modbus](https://ar.solaxpower.com/uploads/file/dtsu666-user-manual-en.pdf)