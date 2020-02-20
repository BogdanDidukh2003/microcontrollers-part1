# LedInterfaceGUI

## Written for University Microcontrollers course (part1)

### Description

Python3 desktop application based on 
[Tkinter](https://docs.python.org/3/library/tkinter.html)
and [Pyserial](https://pythonhosted.org/pyserial/)

This software allows serial communication with microcontroller imitating set of leds controllable via serial port

### Running application

To run LedInterfaceGUI:

1. Make sure you have [Python3](https://www.python.org/downloads/)

2. Set up virtual environment (venv) if prefer (you can skip this step):

<code>python3 -m venv name-of-virtual-env</code>

<code>name-of-virtual-env\Scripts\activate.bat</code> for Windows

<code>source name-of-virtual-env/bin/activate</code> for Unix-like OS

3. Install [Pyserial](https://pythonhosted.org/pyserial/) by simply running a command: 

<code>pip3 install pyserial</code>

### Writing to Serial

After pressing an appropriate button ('Algorithm1' or 'Algorithm2') 
LedInterfaceGUI sends char with the number of algorithm ('1' or '2' by default) to serial port.
Value of char can be checked with microcontroller implementing different logic based on the algorithm number

### Reading from Serial

LedInterfaceGUI is implemented with 8 indicators that can flash depending on algorithm sent by microcontroller to Serial.

Encoded algorithm is represented as a 'b' string based on the following structure:

<code>P0 + "," + P1 + "," + P2 + "," + P3 + "," + P4 + "," + P5 + "," + P6 + "," + P7 + "," + DELAY + "," + SIMULTANEOS_LED_LIGHTING_NUMBER + "\r\n"</code>

Where:
- P0-P7 are numbers of the pins on the board
- DELAY is a delay between lighting of led (as well as time of lighting)
- SIMULTANEOS_LED_LIGHTING_NUMBER is number of leds that will flash at the same time (maximum: number of leds)
- "\r\n" is the ending of the string (automatically included using Serial.println() on Arduino boards)

Note: 
String algorithm representation is automatically converted to a 'b' string while using built-in Serial writing functions on Arduino boards

### P.S.

Tested on Arduino Uno R3, Arduino Mega 2560 with Ubuntu 18.04 LTS

