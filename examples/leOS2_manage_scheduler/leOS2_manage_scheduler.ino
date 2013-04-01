/*
This is a simple sketch to demonstrate the use of leOS2, a
<l>ittle <e>mbedded <O>perating <S>ystem 2 

leOS2 is not a complete OS nor an RTOS as you usually know but it's a simple
scheduler to schedule little works in background, so that you can forget about
them.

This sketch will show the ability to halt the scheduler and to restart
it after a while, useful if you want to stop all the tasks at the same
time.

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

#include "leOS2.h"

leOS2 myOS;
const byte led = 13;
byte statusLed = 1;

void setup() {
    pinMode(led, OUTPUT);
    myOS.begin();
    myOS.addTask(flashLed, myOS.convertMs(100));
}

void loop() {
    delay(5000);
    myOS.haltScheduler();
    delay(5000);
    myOS.restartScheduler();
}

void flashLed() {
    digitalWrite(led, statusLed);
    statusLed ^= 1;
}
