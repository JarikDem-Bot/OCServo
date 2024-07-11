#ifndef OCSERVO_H
#define OCSERVO_H

#include <Arduino.h>
#include <SoftwareSerial.h>

/* INSTRUCTIONS */
#define PING_INSTRUCTION 0x01
#define READ_INSTRUCTION 0x02
#define WRITE_INSTRUCTION 0x03

/* ADDRESSES */
#define OCS_SERVO_ID 0x05
#define OCS_BAUD_RATE 0x06

#define OCS_MAX_TORQUE 0x10

#define OCS_MODE 0x23

#define OCS_TARGET_POSITION 0x2A

/* BAUD RATE FLAGS */
#define OCS_BAUDRATE_1M 0x00
#define OCS_BAUDRATE_500K 0x01
#define OCS_BAUDRATE_250K 0x02
#define OCS_BAUDRATE_128K 0x03
#define OCS_BAUDRATE_115200 0x04
#define OCS_BAUDRATE_76800 0x05
#define OCS_BAUDRATE_57600 0x06
#define OCS_BAUDRATE_38400 0x07

/* MOTOR MODES */
#define OCS_MODE_SERVO 0x00
#define OCS_MODE_MOTOR 0x02

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