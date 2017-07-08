#include "Serial.h"
#include <stdio.h>

void sendData(uint8_t command, uint8_t data){
        int i;
        int j;
        uint8_t controlByte;
        uint8_t byteToSend;
        for(j = 0; j < 2; j++) {

                if(j == 0) {
                        // Command
                        controlByte = 1;
                        byteToSend = command;
                }
                if(j == 1) {
                        // Data
                        controlByte = 0;
                        byteToSend = data;
                }
                // gpioWrite(27,0); // Start of write cycle
                // delay(10);


                //Control Byte Send
                gpioWrite(27,(controlByte & ( 1 << 1)) >> 1);//data
                gpioWrite(17,1);//clock
                delayMicroseconds(1000);
                gpioWrite(27,0);//data

                gpioWrite(27,(controlByte & ( 1 << 0)) >> 0);
                gpioWrite(17,0);
                delayMicroseconds(1000);
                gpioWrite(27,0);

                //DataByte send
                for(i = 3; i >=0; i--) {

                        gpioWrite(27,(byteToSend & ( 1 << ((i*2) + 1))) >> ((i*2)+1));//data //
                        gpioWrite(17,1);//clock                                              // Rising Edge
                        delayMicroseconds(1000);                                                    //
                        gpioWrite(27,0);//data                                               //

                        gpioWrite(27,(byteToSend & ( 1 << (i*2))) >> (i*2)); // data         //
                        gpioWrite(17,0);                                               // Falling Edge
                        delayMicroseconds(1000);                                                  //
                        gpioWrite(27,0);                                               //
                }

                // gpioWrite(27,1); // Write High to idle
                delayMicroseconds(5000);

        }
}
