# OCServo documentation

__Content:__
- [Structures and Enums](https://github.com/JarikDem-Bot/OCServo/edit/master/docs/readme.md#structures-and-enums)
- [Constructors](https://github.com/JarikDem-Bot/OCServo/edit/master/docs/readme.md#constructors)
- [Instructions](https://github.com/JarikDem-Bot/OCServo/edit/master/docs/readme.md#instructions)
- [Set Values](https://github.com/JarikDem-Bot/OCServo/edit/master/docs/readme.md#set-values)
- [Get Values](https://github.com/JarikDem-Bot/OCServo/edit/master/docs/readme.md#get-values)
- [Other](https://github.com/JarikDem-Bot/OCServo/edit/master/docs/readme.md#other)

## Structures and Enums

- `OCSMode` - enum
  - `OCS_SERVO` - Servo mode (0-360 degrees)
  - `OCS_MOTOR` - Constant torque output mode
- `OCSResponseLevel` - enum
  - `OCS_RESPOND_READ_ONLY` - Servo responds only to read and ping commands
  - `OCS_RESPOND_ALL` - Servo responds to all commands
- `OCSResponse` - struct
  - `byte prefix[2]` - Prefix `0xFF, 0xFF`
  - `byte id` - ID of servo
  - `byte length` - Length of instruction
  - `byte instruction` - Instruction
  - `int numberOfParameters` - Number of parameters 
  - `byte parameters[16]` - Supplementary control information except instruction
  - `byte checksum` - Checksum = ~(ID+Length+Param1+...+ParamN)

## Constructors

- `OCServo (byte id, SoftwareSerial *serialPort)`  
  - `id` - ID of servomotor
  - `serialPort` - SoftwareSerial port servo connected to 
- `OCServo (byte id, HardwareSerial *serialPort)`
  - `id` - ID of servomotor
  - `serialPort` - HardwareSerial port servo connected to 

## Instructions

- `void ping()`
  - Used to read the work state of the servo
- `OCSResponse ocsRead(byte address, byte length)`
  - `address` - Data address
  - `length` - Read length 
  - Used to read the data inside the servo
- `OCSResponse ocsWrite(byte address, int paramsNumber, byte *params)`
  - `address` - Data address
  - `paramsNumber` - Number of parameters
  - `*params` - Pointer to array of parameters 
  - This command is used to write parameters to the servo memory control table

## Set Values

- `OCSResponse setID(byte new_id)`
  - `new_id` - New ID for servo (0-254)
  - Changes ID of servomotor
- `void setBaudRate(long baudrate)`
  - `baudrate` - New baudrate value
  - Changes baudrate of servo (supported baud rates: 1M, 500000, 250000, 128000, 115200, 76800, 57600, 38400)
- `OCSResponse setMaxTorque(int torque)`
  - `torque` - Torque value (0-1000)
  - Sets max output torque (0 - min value, 1000 - max value)
- `OCSResponse setMode(int mode)`
  - `mode` - `OCS_SERVO` or `OCS_MOTOR`
  - Sets motor mode
- `OCSResponse setGoalAngle(int angle, long timeMillis= 0)`
  - `angle` - Angle in degrees (0-360)
  - `timeMillis=0` - Time to reach goal im ms (0-65535). Default is 0 (maximum speed)
  - Sets goal angle for servo. It is possible to specify time in milliseconds.
- `OCSResponse setGoalPosition(int position, long timeMillis = 0)`
  - `position` - Goal position (0-4095) 
  - `timeMillis` - Time to reach goal im ms (0-65535). Default is 0 (maximum speed)
  - Sets goal angle for servo. It is possible to specify time in milliseconds. Same as setGoalAngle, but has more control of angle.
- `OCSResponse setOperationTime(long timeMillis)`
  - `timeMillis` - Time to reach goal im ms (0-65535). Default is 0 (maximum speed)
  - Sets time to reach goal. Will be overwritten by setGoalAngle or setGoalPosition.
- `OCSResponse setResponseDelay(int delayMicros)`
  - `delayMicros` - Delay in microseconds (0-510)
  - Sets response delay. 
- `OCSResponse setResponseLevel(int level)`
  - `level` - `OCS_RESPOND_READ_ONLY` or `OCS_RESPOND_ALL`
  - Sets level of response.
- `OCSResponse setMinAngle(int angle)`
  - `angle` - Angle (0-4095)
  - Sets min angle of servo
- `OCSResponse setMaxAngle(int angle)`
  - `angle` - Angle (0-4095)
  - Sets max angle servo
- `OCSResponse setMaxVoltage(int voltage)`
  - `voltage`
  - .
- `OCSResponse setMinVoltage(int voltage)`
  - `voltage`
  - .
- `OCSResponse setOperationSpeed(long speed)`
  - `speed` - Speed of a servo, AD/ms (0-65535)
  - Sets the speed of the servo. 

## Get Values

- `byte getID()`
  - Returns ID of a servo
  - Returns 255 if error occured
- `long getBaudRate()`
  - Returns current baud rate of a servo
  - Returns -1 if error occured
- `int getMaxTorque()`
  - Returns max torque of a servo
  - Returns -1 if error occured
- `int getMode()`
  - Returns motor mode
  - Returns -1 if error occured.
- `int getGoalAngle()`
  - Returns goal angle (0-360 degrees)
- `int getGoalPosition()`
  - Returns goal position (0-4095)
- `long getOperationTime()`
  - Returns operation time in ms (0-65535)
- `int getResponseDelay()`
  - Returns response delay in microseconds (0-510)
- `int getResponseLevel()`
  - Returns response level (`OCS_RESPOND_READ_ONLY` or `OCS_RESPOND_ALL`)
- `int getMinAngle()`
  - Returns min angle of servo (0-4095)
- `int getMaxAngle()`
  - Returns max angle of servo (0-4095)
- `int getMaxVoltage()`
  - .
- `int getMinVoltage()`
  - .
- `long getOperationSpeed()`
  - Returns operation speed in AD/ms (0-65535)
- `bool reachedGoal()`
  - Returns true if servo reached it's goal. Otherwise returns false 

## Other

- `void begin(long baudrate=1000000)`
  - `baudrate` - Baud rate value. Default is 1M
  - Sets baudrate value for serial port servo connected to.
  - Function `setBaudRate` calls this method inside, so you do not need to do this manually.
- `void printResponse(OCSResponse response)`
  - `response` - `OCSResponse` struct with response.
  - Prints response structure.
