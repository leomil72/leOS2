/*
This is a simple sketch to demonstrate the use of leOS2, a
<l>ittle <e>mbedded <O>perating <S>ystem 2 

leOS2 is not a complete OS nor an RTOS as you usually know but it's a simple
scheduler to schedule little works in background, so that you can forget about
them.

This sketch tests the usage of the method reset() called by another task.

More info on the scheduler and the methods can be found into the README file.

Written by Leonardo Miliani <www.leonardomiliani.com>
	
This code and the leOS2 library are free software; you can redistribute 
and/or modify them under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3.0 of the License,
or (at your option) any later version.

The leOS2 library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

*/

#include "leOS2.h" //include the scheduler
leOS2 myOS; //create a new istance
const byte LED = 13; //LED on pin 13


//setup routine
void setup() {
    myOS.begin(); //initialize the scheduler
    pinMode(LED, OUTPUT); //pin as output
    myOS.addTask(reset, myOS.convertMs(10000)); //add a task to be executed after 10 seconds
    blink3times(); //blink 3 times the LED on pin 13
}


//main loop - it's empty
void loop() {
}


//reset the microcontroller
void reset() {
    myOS.reset();
}


//blink 3 times the LED on pin 13
void blink3times() {
    for (byte i=0; i<3; i++) {
        digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);
    }
}
