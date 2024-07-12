#ifndef OCSERVO_H
#define OCSERVO_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "ROBSMotor.h"

enum OCSMode {
    OCS_SERVO,
    OCS_MOTOR
};

enum OCSResponseLevel {
    OCS_RESPOND_READ_ONLY,
    OCS_RESPOND_ALL
};

typedef enum e_SerialPortType {
    SERIAL_HARDWARE,
    SERIAL_SOFTWARE
} SerialPortType;

struct OCSResponse {
    byte prefix[2];
    byte id;
    byte length;
    byte instruction;
    int numberOfParameters;
    byte parameters[16];
    byte checksum;
};

class OCServo {
    private:
        byte id;
        Stream *serial;
        SerialPortType portType;

        byte getChecksum(byte *data, int size);
        byte baudRateToByte(long baudrate);
        OCSResponse bytesToResponse(byte *data, int size);
        OCSResponse readResponse();

    public:
        OCServo(byte id, SoftwareSerial *serialPort);
        OCServo(byte id, HardwareSerial *serialPort);

        void ping();
        void regRead(byte address, byte length);
        OCSResponse regWrite(byte address, int paramsNumber, byte *params);

        OCSResponse setID(byte new_id);
        void setBaudRate(long baudrate);
        OCSResponse setMaxTorque(int torque);
        OCSResponse setMode(int mode);
        OCSResponse setGoalPosition(int angle, long timeMillis = 0);
        OCSResponse setResponseDelay(int delayMicros);
        OCSResponse setResponseLevel(int level);
        OCSResponse setMinAngle(int angle);
        OCSResponse setMaxAngle(int angle);
        OCSResponse setMaxVoltage(int voltage);
        OCSResponse setMinVoltage(int voltage);
        OCSResponse setOperationSpeed(long speed);

        void getBaudRate();

        void begin(long baudrate=1000000);
        void printResponse(OCSResponse response);
};

#endif