#include "OCServo.h"

/* CONSTRUCTORS */
OCServo::OCServo(byte id, SoftwareSerial *serialPort) : id(id), serial(serialPort){
    this->portType = SERIAL_SOFTWARE;
}

OCServo::OCServo(byte id, HardwareSerial *serialPort) : id(id), serial(serialPort){
    this->portType = SERIAL_HARDWARE;
}

/* PRIVATE METHODS */
byte OCServo::getChecksum(byte *data, int size) {
    byte checksum = 0;
    for (byte i = 2; i < size; i++)
    {
        checksum += data[i];
    }
    return ~(checksum & 0xFF);
}

byte OCServo::baudRateToByte(long baudrate) {
    byte value = 0x00;

    switch (baudrate) {
        case 500000:
            value = OCS_BAUDRATE_500K;
            break;
        case 250000:
            value = OCS_BAUDRATE_250K;
            break;
        case 128000:
            value = OCS_BAUDRATE_128K;
            break;
        case 115200:
            value = OCS_BAUDRATE_115200;
            break;
        case 76800:
            value = OCS_BAUDRATE_76800;
            break;
        case 57600:
            value = OCS_BAUDRATE_57600;
            break;
        case 38400:
            value = OCS_BAUDRATE_38400;
            break;
        default:
            Serial.println("Invalid baudrate. Using default 1M baudrate.");
        case 1000000:
            value = OCS_BAUDRATE_1M;
            break;
    }

    return value;
}

void OCServo::readResponse() {
    byte response[16];
    int i = 0;

    Serial.print("Response: ");
    unsigned long last_check_time = millis();
    while(true) {
        if(serial->available() > 0) {
            response[i] = serial->read();
            i++;
            last_check_time = millis();
        }
        else if(millis() > last_check_time + 2) {
            break;
        }
    }

    for(int j = 0; j < i; j++) {
        Serial.print(" 0x");
        Serial.print(response[j], HEX);
    }
    Serial.println();
}

/* INSTRUCTIONS */
void OCServo::ping() {
    int size = 6;
    byte request[size] = {
        0xFF, 0xFF,         // Prefix
        id,                 // ID
        0x02,               // Length  
        PING_INSTRUCTION,   // Instruction
                            // No Parameter
        0x00                // Checksum
    };
    request[size-1] = this->getChecksum(request, size-1);

    serial->write(request, size);
    this->readResponse();
}

void OCServo::regRead(byte address, byte length) {
    int size = 8;
    byte request[size] = {
        0xFF, 0xFF,         // Prefix
        id,                 // ID
        0x04,               // Length
        READ_INSTRUCTION,   // Instruction
        address,            // Parameter 1 - read address
        length,             // Parameter 2 - read length
        0x00                // Checksum
    };
    request[size-1] = this->getChecksum(request, size-1);

    serial->write(request, size);
    this->readResponse();
}

void OCServo::regWrite(byte address, int paramsNumber, byte *params) {
    int size = paramsNumber + 7;
    byte request[size] = {
        0xFF, 0xFF,             // Prefix
        id,                     // ID
        (byte)(paramsNumber+3), // Length
        WRITE_INSTRUCTION,      // Instruction
        address                 // Address
    };

    for(int i = 0; i < paramsNumber; i++) {
        request[6+i] = *(params + i);   // Parameters
    }

    request[size-1] = this->getChecksum(request, size-1);

    serial->write(request, size);
    this->readResponse();
}

/* WRITE COMMANDS */
void OCServo::setID(byte new_id) {
    this->regWrite(OCS_SERVO_ID, 1, &new_id);
    id = new_id;
}

void OCServo::setBaudRate(long baudrate) {
    byte value = this->baudRateToByte(baudrate);

    this->regWrite(OCS_BAUD_RATE, 1, &value);

    this->begin(baudrate);
}

void OCServo::setMaxTorque(int torque) {
    byte params[2] = {
        lowByte(torque),
        highByte(torque)
    };
    this->regWrite(OCS_MAX_TORQUE, 2, params);
}

void OCServo::setMode(int mode) {
    byte value;

    switch(mode) {
        default:
            // Fallthrough to OCS_SERVO
        case OCS_SERVO:
            value = OCS_MODE_SERVO;
            break;
        case OCS_MOTOR:
            value = OCS_MODE_MOTOR;
            break;
    } 
    
    this->regWrite(OCS_RUNNING_MODE, 1, &value);
}

void OCServo::setGoalPosition(int angle, long timeMillis /*= 0*/) {
    angle = map(angle, 0, 360, 0, 4095);
    byte params[4] = {
        lowByte(angle),
        highByte(angle),
        lowByte(timeMillis),
        highByte(timeMillis)
    };
    this->regWrite(OCS_GOAL_POSITION, 4, params);
}

void OCServo::setResponseDelay(int delayMicros) {
    if(delayMicros < 0 || delayMicros > 510) {
        Serial.println("Invalid delay value. Using 0 instead.");
        delayMicros = 0;
    }
    byte value = delayMicros / 2;
    this->regWrite(OCS_RESPONSE_DELAY, 1, &value);
}

void OCServo::setResponseLevel(int level) {
    byte value;

    switch (level)
    {
        case OCS_RESPOND_READ_ONLY:
            value = OCS_RESPONSE_READ;
            break;
        default:
            // Fallthrough to OCS_RESPOND_ALL
        case OCS_RESPOND_ALL:
            value = OCS_RESPONSE_ALL;
            break;
    }

    this->regWrite(OCS_RESPONSE_LEVEL, 1, &value);
}

void OCServo::setMinAngle(int angle) {
    byte params[2] = {
        lowByte(angle),
        highByte(angle)
    };
    this->regWrite(OCS_MIN_ANGLE, 2, params);
}

void OCServo::setMaxAngle(int angle) {
    byte params[2] = {
        lowByte(angle),
        highByte(angle)
    };
    this->regWrite(OCS_MAX_ANGLE, 2, params);
}

void OCServo::setMaxVoltage(int voltage) {
    byte params[2] = {
        lowByte(voltage),
        highByte(voltage)
    };
    this->regWrite(OCS_MAX_VOLTAGE, 2, params);
}

void OCServo::setMinVoltage(int voltage) {
    byte params[2] = {
        lowByte(voltage),
        highByte(voltage)
    };
    this->regWrite(OCS_MIN_VOLTAGE, 2, params);
}

void OCServo::setOperationSpeed(long speed) {
    byte params[2] = {
        lowByte(speed),
        highByte(speed)
    };
    this->regWrite(OCS_OPERATION_SPEED, 2, params);
}

/* READ COMMANDS */
void OCServo::getBaudRate() {
    this->regRead(OCS_BAUD_RATE, 1);
}

/* OTHER */
void OCServo::begin(long baudrate/*=1000000*/) {
    switch(this->portType) {
        case SERIAL_HARDWARE:
            static_cast<HardwareSerial*>(serial)->begin(baudrate);
            break;
        case SERIAL_SOFTWARE:
            static_cast<SoftwareSerial*>(serial)->begin(baudrate);
            break;
    }
}
