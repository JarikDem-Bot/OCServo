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
            // Fallthrough to default 1M
        case 1000000:
            value = OCS_BAUDRATE_1M;
            break;
    }

    return value;
}

long OCServo::byteToBaudRate(byte value) {
    long result;

    switch(value) {
        case OCS_BAUDRATE_1M:
            result = 1000000;
            break;
        case OCS_BAUDRATE_500K:
            result = 500000;
            break;
        case OCS_BAUDRATE_250K:
            result = 250000;
            break;
        case OCS_BAUDRATE_128K:
            result = 128000;
            break;
        case OCS_BAUDRATE_115200:
            result = 115200;
            break;
        case OCS_BAUDRATE_76800:
            result = 76800;
            break;
        case OCS_BAUDRATE_57600:
            result = 57600;
            break;
        case OCS_BAUDRATE_38400:
            result = 38400;
            break;
        default:
            result = -1;
            break;
    }

    return result;
}

OCSResponse OCServo::bytesToResponse(byte *data, int size) {
    OCSResponse response;

    response.prefix[0] = *(data);
    response.prefix[1] = *(data+1);
    response.id = data[2];
    response.length = data[3];
    response.instruction = data[4];
    response.numberOfParameters = response.length - 2;
    for(int i = 0; i < response.numberOfParameters; i++) {
        response.parameters[i] = data[5+i];
    }
    response.checksum = data[response.numberOfParameters + 5];

    return response;
}

OCSResponse OCServo::readResponse() {
    byte response[23];  // 16 params + 7 other fields
    for (int x = 0; x < 23; x++) response[x] = 0x00; // Clear response array (just in case)
    int i = 0;

    unsigned long last_check_time = millis();
    while(true) {
        if(serial->available() > 0) {
            response[i] = serial->read();
            i++;
            last_check_time = millis();
        }
        else if(millis() > last_check_time + 4) {
            break;
        }
    }

    OCSResponse result = this->bytesToResponse(response, i);
    return result;
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

OCSResponse OCServo::regRead(byte address, byte length) {
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
    return this->readResponse();
}

OCSResponse OCServo::regWrite(byte address, int paramsNumber, byte *params) {
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
    return this->readResponse();
}

/* WRITE COMMANDS */
OCSResponse OCServo::setID(byte new_id) {
    OCSResponse resp =  this->regWrite(OCS_SERVO_ID, 1, &new_id);
    id = new_id;
    return resp;
}

void OCServo::setBaudRate(long baudrate) {
    byte value = this->baudRateToByte(baudrate);

    this->regWrite(OCS_BAUD_RATE, 1, &value);

    this->begin(baudrate);
}

OCSResponse OCServo::setMaxTorque(int torque) {
    byte params[2] = {
        lowByte(torque),
        highByte(torque)
    };
    return this->regWrite(OCS_MAX_TORQUE, 2, params);
}

OCSResponse OCServo::setMode(int mode) {
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
    
    return this->regWrite(OCS_RUNNING_MODE, 1, &value);
}

OCSResponse OCServo::setGoalPosition(int angle, long timeMillis /*= 0*/) {
    angle = map(angle, 0, 360, 0, 4095);
    byte params[4] = {
        lowByte(angle),
        highByte(angle),
        lowByte(timeMillis),
        highByte(timeMillis)
    };
    return this->regWrite(OCS_GOAL_POSITION, 4, params);
}

OCSResponse OCServo::setResponseDelay(int delayMicros) {
    if(delayMicros < 0 || delayMicros > 510) {
        // Invalid delay, using default 0
        delayMicros = 0;
    }
    byte value = delayMicros / 2;
    return this->regWrite(OCS_RESPONSE_DELAY, 1, &value);
}

OCSResponse OCServo::setResponseLevel(int level) {
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

    return this->regWrite(OCS_RESPONSE_LEVEL, 1, &value);
}

OCSResponse OCServo::setMinAngle(int angle) {
    byte params[2] = {
        lowByte(angle),
        highByte(angle)
    };
    return this->regWrite(OCS_MIN_ANGLE, 2, params);
}

OCSResponse OCServo::setMaxAngle(int angle) {
    byte params[2] = {
        lowByte(angle),
        highByte(angle)
    };
    return this->regWrite(OCS_MAX_ANGLE, 2, params);
}

OCSResponse OCServo::setMaxVoltage(int voltage) {
    byte params[2] = {
        lowByte(voltage),
        highByte(voltage)
    };
    return this->regWrite(OCS_MAX_VOLTAGE, 2, params);
}

OCSResponse OCServo::setMinVoltage(int voltage) {
    byte params[2] = {
        lowByte(voltage),
        highByte(voltage)
    };
    return this->regWrite(OCS_MIN_VOLTAGE, 2, params);
}

OCSResponse OCServo::setOperationSpeed(long speed) {
    byte params[2] = {
        lowByte(speed),
        highByte(speed)
    };
    return this->regWrite(OCS_OPERATION_SPEED, 2, params);
}

/* READ COMMANDS */
long OCServo::getBaudRate() {
    OCSResponse response = this->regRead(OCS_BAUD_RATE, 1);
    if (response.numberOfParameters != 1) {
        return -1;
    }
    return this->byteToBaudRate(response.parameters[0]);
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

void OCServo::printResponse(OCSResponse response) {
    Serial.print("Prefix: 0x");
    Serial.print(response.prefix[0], HEX);
    Serial.print(" 0x");
    Serial.println(response.prefix[1], HEX);

    Serial.print("ID: 0x");
    Serial.println(response.id, HEX);

    Serial.print("Length: 0x");
    Serial.println(response.length, HEX);

    Serial.print("Instruction: 0x");
    Serial.println(response.instruction, HEX);

    Serial.print("Parameters:");
    for(int i = 0; i < response.numberOfParameters; i++) {
        Serial.print(" 0x");
        Serial.print(response.parameters[i], HEX);
    }
    Serial.println();

    Serial.print("Checksum: 0x");
    Serial.println(response.checksum, HEX);
}
