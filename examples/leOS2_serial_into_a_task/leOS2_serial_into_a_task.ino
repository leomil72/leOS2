/*
This is a simple sketch to demonstrate the use of leOS2, a
<l>ittle <e>mbedded <O>perating <S>ystem 2 

leOS2 is not a complete OS nor an RTOS as you usually know but it's a simple
scheduler to schedule little works in background, so that you can forget about
them.

This sketch will add 3 tasks that have to be run at different intervals.
To test the sketch, connect 2 LEDs (with their corresponding resistors)
to pins D7 and D8, then connect the Arduino and open the serial monitor at
19200 bps: the 3rd sketch will print something on the serial.

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

//variables to manage the LEDs
const byte LED1 = 7;
const byte LED2 = 8;
byte LED1status = 1;
byte LED2status = 1;
unsigned int testVar = 0;

//setup routine
void setup() {
    myOS.begin(); //initialize the scheduler
    //pins as output
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    //initialize the serial comm. and wait 2 secs. before to start the tasks
    Serial.begin(19200);
    delay(2000);
    //add the tasks
    myOS.addTask(flashLed1, myOS.convertMs(500));
    myOS.addTask(flashLed2, myOS.convertMs(250));
    myOS.addTask(printData, myOS.convertMs(1000));
}


//main loop - it's empty
void loop() {}


//this task will flash the 1st LED
void flashLed1() {
    LED1status ^= 1;
    digitalWrite(LED1, LED1status);
}


//this task will flash the 2nd LED
void flashLed2() {
    LED2status ^= 1;
    digitalWrite(LED2, LED2status);
}

void printData() {
    Serial.println(testVar, DEC);
    testVar++;
}
