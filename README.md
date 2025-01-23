<div align="center"><img src="https://github.com/user-attachments/assets/ec4122c9-606f-4953-a0cf-03f3bd8f9b3a" width="30%"></div>

<h1 align="center">OCServo library</h1>

<div align="center">

<a href="/LICENSE">![GitHub](https://img.shields.io/github/license/JarikDem-Bot/OCServo)</a>
<a href="https://github.com/JarikDem-Bot/OCServo">![Static badge](https://img.shields.io/badge/OCServo-green)</a>
<a href="https://www.arduino.cc/">![Static badge](https://img.shields.io/badge/Arduino-blue)</a>

</div>

This library allows you to easily control servomotors of ROBS series of [OCServo manufacturer](https://en.ocservo.com/). 

## Installation

[Tutorial with screen-shots how to download a library from GitHub and installing a ZIP-library](https://forum.arduino.cc/t/tutorial-with-screen-shots-how-to-download-a-library-from-github-and-installing-a-zip-library/994122/1)

## Connection

Connected using UART to RS485 board included to servomotor.
| Arduino | Servo Board |
| ----- | --- |
| Tx1*  | Tx  |
| Rx1*  | Rx  |
| 5V    | 5V  |
| GND   | GND |

\* I used Hardware Serial port 1, but library supports any** Hardware or Software serial port.

\** Default baud rate value for servo is 1M, so port must be able to handle this value.

## Documentation
Detailed documentation can be found [->HERE<-](/docs/readme.md)

Also, check out [examples](/examples)!

## Tested on

Motors models:
- OCServo ROBS-301 servomotor

Boards:
- Arduino Mega 2560 Pro
- STM32F411 BlackPill ([Platformio](https://platformio.org/) and Arduino framework)

## Resources

- [OCServo documentations](https://en.ocservo.com/?page_id=8127)
- [ROBS-301 documentation](https://fcdn.goodq.top/caches/5bccc64925e01bd6f0a7519abd81e6a1/aHR0cDovL2VuLm9jc2Vydm8uY29tL3FmeS1jb250ZW50L3VwbG9hZHMvMjAyMy8wNS8xOWRmOTc4OTAxMDI0NGJlYjk0ZGE1YzQ3MWI0MTI0MC5wZGY_p_p100_p_3D.pdf)
- [OCServo store](https://en.ocservo.com/?page_id=15418&qfyuuid=-1&q_term=44&q_type=products)

## License

[MIT](/LICENSE)
