/*
This is a simple sketch to demonstrate the use of leOS2, a
<l>ittle <e>mbedded <O>perating <S>ystem 2 

leOS2 is not a complete OS nor an RTOS as you usually know but it's a simple
scheduler to schedule little works in background, so that you can forget about
them.

This sketch demonstrates the blink of 1 LED without using millis() nor delay().
Simply upload the sketch and look at the LED on pin 13.

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

//variables to manage the LED
const byte LED13 = 13;
byte LEDstatus = 0;


//setup routine
void setup() {
    myOS.begin(); //initialize the scheduler
    //pins as output
    pinMode(LED13, OUTPUT);
    //add the tasks
    myOS.addTask(flashLed, myOS.convertMs(1000));
}


//main loop - it's empty
void loop() {}


//this task will flash the LED on pin D13
void flashLed() {
    LEDstatus ^= 1;
    digitalWrite(LED13, LEDstatus);
}


