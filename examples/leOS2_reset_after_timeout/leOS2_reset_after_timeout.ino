/*
This is a simple sketch to demonstrate the use of leOS2, a
<l>ittle <e>mbedded <O>perating <S>ystem 2 

leOS2 is not a complete OS nor an RTOS as you usually know but it's a simple
scheduler to schedule little works in background, so that you can forget about
them.

This sketch demonstrates the use of the "interrupt + system reset" mode.
In this configuration, when the WDT expires, first an interrupt is raised, then
a system reset is executed. If, inside the ISR, the flag WDIE is set, at the
next timeout the WDT won't reset the micro but will raise another interrupt.
And so on. But if the scheduler encountes a task that freezes the MCU, a counter 
internal at the ISR will be decremented. When it will reach 0, the reset 
won't be set anymore and the WDT will reset the microcontroller.

To test the example, set up a circuit with 3 LEDs (and the corresponding resistors)
and connect them to pins D7, D8, and D9.

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
const byte LED = 7;
byte LEDstatus = 0;
const byte LED2 = 8;
const byte LED3 = 9;

//setup routine
void setup() {
    //initialize the scheduler
    //myOS.begin(); //without a timeout, the MCU can be freezed by a neverending task
    myOS.begin(myOS.convertMs(2000)); //with a timeout, the WDT will reset the micro if a task will freeze the CPU
    
    //pins as output
    pinMode(LED, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    //add the tasks
    myOS.addTask(flashLed, myOS.convertMs(500));
    myOS.addTask(freeze, myOS.convertMs(5000));
    start(); //hello world
}


//main loop - it's empty
void loop() {
    digitalWrite(LED3, HIGH);
    delay(150);
    digitalWrite(LED3, LOW);
    delay(150);
}


//hello world
void start() {
    for (byte i = 0; i<2; i++) {
        digitalWrite(LED2, HIGH);
        delay(125);
        digitalWrite(LED2, LOW);
        delay(125);
    }
}


//this task will flash a LED 
void flashLed() {
    digitalWrite(LED, LEDstatus);
    LEDstatus ^= 1;
}


//this task will freeze the micro
void freeze() {
    while(1) {};
}