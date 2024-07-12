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
        void regWrite(byte address, int paramsNumber, byte *params);

        void setID(byte new_id);
        void setBaudRate(long baudrate);
        void setMaxTorque(int torque);
        void setMode(int mode);
        void setGoalPosition(int angle, long timeMillis = 0);
        void setResponseDelay(int delayMicros);
        void setResponseLevel(int level);
        void setMinAngle(int angle);
        void setMaxAngle(int angle);
        void setMaxVoltage(int voltage);
        void setMinVoltage(int voltage);
        void setOperationSpeed(long speed);

        void getBaudRate();

        void begin(long baudrate=1000000);
        void printResponse(OCSResponse response);
};

#endif