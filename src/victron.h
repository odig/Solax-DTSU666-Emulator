#pragma once
class Victron : private ModbusIP
{
private:
    bool victronConnected = false;
    uint16_t regBuff[45];
    bool hregCallback(Modbus::ResultCode result, uint16_t transactionId, void *arg);

public:
    void setupVictronModbusSlave();
    void victronConnect();
    void handleVictronModbusSlave();
    void readVictronModBus();
};