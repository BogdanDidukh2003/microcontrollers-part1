# SerialGUI

## Written for University Microcontrollers course (part1)

### Description

Python desktop application based on 
[Tkinter](https://docs.python.org/3/library/tkinter.html)
and [Pyserial](https://pythonhosted.org/pyserial/)

This software allows serial communication with microcontroller for requesting data via serial port

### Running application

To run SerialGUI:

1. Make sure you have [Python](https://www.python.org/downloads/) installed

2. Set up virtual environment (venv) if prefer (you can skip this step):

<code>python -m venv name-of-virtual-env</code>

<code>name-of-virtual-env\Scripts\activate.bat</code> for Windows

<code>source name-of-virtual-env/bin/activate</code> for Unix-like OS

3. Install required libraries by simply running a command: 

<code>pip install -r requirements.txt</code>

### Writing to Serial

After pressing an appropriate button ('From Slave 1' or 'From Slave 2') 
SerialGUI sends char with the number of a slave ('1' or '2' by default) to serial port.
Value of char can be checked with microcontroller implementing different logic based on the received number

### P.S.

Tested on Ubuntu 18.04 LTS
