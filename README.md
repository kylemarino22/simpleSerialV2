# simpleSerialV2
A lightweight, yet slow, serial communication method.

# Implementation
###### Hardware Connection
This is relatively simple: connect a clock line between any pins (pin 10 on arduino and pin 17 on a raspberry pi), and a data line between any pins (pin 9 on arduino and pin 27 on a raspberry pi).

###### Master Code
To run this protocol on your master device, call this function:
`sendData(int command, int data);`
The command and data parameters can be whatever you desire, as long as it is between 0-255. The command parameter is only used to specify what the data is to be used for on the slave side.

###### Slave Code
On the slave side, run this function in the setup:
`beginSerial();`
This sets up the interrupt settings for the arduino. These **WILL** change based on your chip, so be aware of that. The default runs the interrupt on pin 10 on an arduino. Also in the ISR, pin 9 is being read by the line:
`if (PINB & B00000010 ) {`
If you with to change this to a different pin, you have to change these values. `PINB` is the block of pins and `B00000010` is pin 9. If you aren't sure how this works, search on google. 

Since there is no easy way to get the data out from the ISR, you have to use global variables. I have three currently being used:
- throttle
- roll
- pitch

These each have their own command number, and is written to whenever the master sends a new value. You can replace these variables with your own, or even add some more. Just change the variables in the header file as well.

# How it works
For a serial communicaiton protocol, this is about as simple as it gets. (Besides my first protocol)
The master drives a clock pulse (pin 17 for default). For a bit transfer, the data line powered during a clock edge. The protocol works by sending out 10 bit "packets" where the first bits indicate the type of data (currently I have two: command and data), and the last 8 bits are the data being sent. The data line is shifted by 90 degrees so that each read has the optimal position. (This is only necessary when being used at faster speeds).
```
        _______         _______         _______         _______         _______       
clk ___|       |_______|       |_______|       |_______|       |_______|       |_____

            _____           ________________        _______                  _______ 
data ______|     |_________|                |______|       |________________|       |  

        |      |       |       |       |       |       |       |       |       |
        0      1       0       1       1       0       1       0       0       1      
        --------       ---------------------------------------------------------
          type                                 data

```
For the type bits, 00 indicates a regular data, and 01 indicates a command.

On the slave side, there is an ISR that activates on every change on the clock line. The data pin is compared, and if it is one, a temporary variable gets shifted to the left. This repeats until the end of the data. 

To distinguish between packets, there is a 3+ clock delay between packets. The ISR tracks the time betwen pulses, and if it is greater than 2 clocks, it starts a new packet.

# Issues
There are a few timing issues based on the contents of the main loop, and I have yet to work that out. Those issues only become apparent when running the bus at significantly faster speeds. 
