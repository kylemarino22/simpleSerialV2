#include "Serial.h"
#include <Arduino.h>

long prevT, currentT;
int timer;
short input;
int posCounter = 0;
bool commandFlag= false;
int command;

int throttle;
int pitch;
int roll;

void beginSerial(){
        PCICR |= (1 << PCIE0);
        PCMSK0 |= (1 << PCINT2);
}

ISR(PCINT0_vect) {
        delayMicroseconds(500);
        currentT = micros();
        timer = currentT - prevT;


        if(timer > 3000) {
                posCounter = 0;
                input = 0;
        }



        input = input <<1;
        //Serial.println(input);
        if (PINB & B00000010 ) {
                Serial.println("1");
                input +=1;
        }
        else{
                Serial.println("0");
        }
        if(posCounter == 1) {
//    Serial.println("here");
//    Serial.println(input);
                if(input == 1) {
                        //Serial.println("Command");
                        commandFlag = true;

                }
                if(input == 0) {
                        //  Serial.println("Data");
                }
                input = 0;
                //Serial.println("-----");
        }

        if(posCounter == 9) {
                //Serial.println(input);
                if(commandFlag) {
                        command = input;
                }
                else if(command == 1) {
                        throttle = (input * 5) + 1000;
                }
                else if(command == 2) {
                        roll = input - 90;
                }
                else if(command == 3) {
                        pitch = input - 90;
                }
                posCounter = 0;
                input = 0;
                commandFlag = false;
        }
        else{
                posCounter++;
        }

        prevT = currentT;

}
