# Standard-lights-controller
## About
This is a small program designed for Arduino, aimed at controlling auxiliary lights for motorcycles. However, it can be adapted to run on any small IoT computers compatible with Arduino libraries.
### Features
#### On/Off Control: Enables users to turn auxiliary lights on and off conveniently.
#### Interrupt-Based Switch Reading: Utilizes interrupts from switches to trigger the switch reading function, effectively filtering out faulty readings caused by vibrations.
#### Reliable Switch Status Determination: The program reads each switch 1000 times and counts occurrences of status "0" or "1". The status for each switch is set up based on a 95% rule, ensuring reliable switch status determination.
#### Telemetry for Troubleshooting: Provides simple telemetry for troubleshooting purposes, aiding in diagnosing any issues that may arise.

## How to use

### Hardware Setup: Connect the Arduino board to the auxiliary lights and switches according to your specific setup requirements.
### Upload Program: Upload the provided Arduino sketch to your Arduino board.
### Power On: Power on the Arduino board.
### Control Lights: Use the switches to control the auxiliary lights. The program will ensure reliable operation even in vibrating conditions.