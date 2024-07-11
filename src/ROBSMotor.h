#ifndef ROBS_MOTOR_H
#define ROBS_MOTOR_H

/* INSTRUCTIONS */
#define PING_INSTRUCTION    0x01
#define READ_INSTRUCTION    0x02
#define WRITE_INSTRUCTION   0x03

/* ADDRESSES */
#define OCS_SERVO_ID        0x05
#define OCS_BAUD_RATE       0x06
#define OCS_RESPONSE_DELAY  0x07
#define OCS_RESPONSE_LEVEL  0x08
#define OCS_MIN_ANGLE       0x09
#define OCS_MAX_ANGLE       0x0B
#define OCS_MAX_VOLTAGE     0x0E
#define OCS_MIN_VOLTAGE     0x0F
#define OCS_MAX_TORQUE      0x10
#define OCS_RUNNING_MODE    0x23
#define OCS_GOAL_POSITION   0x2A
#define OCS_OPERATION_SPEED 0x2E

/* BAUD RATE FLAGS */
#define OCS_BAUDRATE_1M     0x00
#define OCS_BAUDRATE_500K   0x01
#define OCS_BAUDRATE_250K   0x02
#define OCS_BAUDRATE_128K   0x03
#define OCS_BAUDRATE_115200 0x04
#define OCS_BAUDRATE_76800  0x05
#define OCS_BAUDRATE_57600  0x06
#define OCS_BAUDRATE_38400  0x07

/* MOTOR MODES */
#define OCS_MODE_SERVO      0x00
#define OCS_MODE_MOTOR      0x02

#define OCS_RESPONSE_READ   0x00
#define OCS_RESPONSE_ALL    0x01

#endif