#ifndef OCSERVO_H
#define OCSERVO_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "ROBSMotor.h"

enum OCSMode {
    OCS_SERVO,
    OCS_MOTOR
};

typedef enum e_SerialPortType {
    SERIAL_HARDWARE,
    SERIAL_SOFTWARE
} SerialPortType;

class OCServo {
    private:
        byte id;
        Stream *serial;
        SerialPortType portType;

        byte getChecksum(byte *data, int size);
        byte baudRateToByte(long baudrate);
        void readResponse();

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
        void setTargetAngle(int angle, int time_ms = 0);
        
        void getBaudRate();

        void begin(long baudrate=1000000);
};

#endif