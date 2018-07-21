******** leOS2.h - <l>ittle <e>mbedded <O>perating <S>ystem 2 *******

leOS2 (little embedded Operating System 2)

leOS2 is a simple scheduler to execute little routines in background,
at specific intervals. leOS2 comes from leOS but instead of using an
internal timer like the latter, it is based on the WatchDog Timer, a
separated counter that is attached to an internal oscillator clocked at
128 kHz.

For more infos, please read the README.txt file.

The latest version of this library can be found at:
http://www.leonardomiliani.com/
or
https://github.com/leomil72

Written by Leonardo Miliani <www.leonardomiliani.com>

More infos can be found in the attached user's guide (PDF).

***********************
Version history

v. 2.3.3:  fixed typo in keywords.txt
v. 2.3.2:  eliminated usage of NULL to fix gcc compiler warnings
v. 2.3.1:  added compatibility with Arduino IDE >= 1.6.7
v. 2.3.0:  fixed a critical bug that lead to infinite execution of task 0 (also if ONETIME)
v. 2.2.3:  set the WDT timer to 2s when the watchdog is used to reset the MCU to avoid infinite resets.
v. 2.2.2:  little bugs fixed
v. 2.2.1:  fixed an issue in modifyTask
v. 2.2.0:  new methods to stop/restart the scheduler
v. 2.1.1:  now a task can start as soon as it has been added to the scheduler
v. 2.1.0:  stable release - fixed a bug on the managemenet of the freezing tasks
v. 2.0.91: code cleaning
v. 2.0.90: a timeout can be specified for MCU reset during initialization
v. 2.0.2:  removed support for Atmega8/A
v. 2.0.1:  first release


******************************************************************************************
******************************************************************************************
How to use it - Methods
-----------------------

Unpack the library and copy it into your "/libraries" folder, that usually is in
your sketchs' folder. Then include the library and create a new istance of it
by adding the following code at the top of your sketch:

------------------------------------------
#include "leOS2.h"
leOS2 myOS

Then you have to initialize the library in the setup() routine:
void setup() {
  myOS.begin([timeoutInTicks]);
  .....
}
------------------------------------------

<timeoutInTicks> is optional. If set to 0, leOS2 will work as usual. If a values
of ticks is specified (maybe also using the new method <convertMs()> if you want to
pass milliseconds instead of ticks - see below) the scheduler will execute
the tasks as usual. But, when a task will freeze, i.e. for a neverending loop,
the ISR will start to count: when it will reach the timeout, a reset will
be executed. This is done using the "interrupt & system reset" mode of the
Watchdog Timer (WDT): every time that the timer will expire, the WDT will raise an
interrupt first, and, if inside the ISR the flag WDIE won't be set again, the next
time that the timer will expire, a system reset will be performed.

Now you can add a task by simply call the method <addTask()>:
------------------------------------------
void setup() {
  ...
  myOS.addTask(yourFunction, scheduleTime[, status]);
  ...
}
------------------------------------------

<yourFunction> must be a routine that is inside your sketch. <scheduleTime> is
the interval between 2 executions of the task, in "ticks". A tick is a
particular measurement of the time that is 16 ms long. So the minimum
available interval is 16 ms and the interval must always be a
multiply of 16 ms. Because:
WDT frequency / max. value of the WDT counter = minimum internal

So, in numbers:
128,000 kHz / 2,048 = 62.5 Hz
1/62.5 = 0.016 s -> 16 ms

To set an interval of about 1 second, you have to specify a schedule time of
~62 ticks (1,000/16=62.5 -> 62). To help you converting between ms and ticks
you can use the method <convertMs()>:

------------------------------------------
myOS.addTask(yourFunction, myOS.convertMs(schedule_time_in_ms)[, status][, start]);
------------------------------------------

The user can choose the status of the task when it adds it to the scheduler.
<status> can be:
PAUSED, for a task that doesn't have to start immediately;
SCHEDULED (default option), for a normal task that has to start after its
scheduling;
ONETINE, for a task that has to run only once.
SCHEDULED_IMMEDIATESTART or simply IMMEDIATESTART, for a task that has to
be executed once it has been added to the scheduler.

An interesting feature is the ability to run one-time tasks. A one-time task
is a task that will be run only once: the scheduler, once it has run the task,
will remove it from the running tasks (it won't be paused, it will be permanently
deleted).


To pause a task, just call the following method:
------------------------------------------
myOS.pauseTask(yourFunction);
------------------------------------------

You can restart it with:
------------------------------------------
myOS.restartTask(yourFunction);
------------------------------------------

To remove a task from the scheduler call this method:
------------------------------------------
myOS.removeTask(yourFunction);
------------------------------------------

To modify a running task, simply call the <modifyTask> method with the new interval
(in ticks) and/or the status of the task, i.e. a normal or a one-time task:
------------------------------------------
myOS.modifyTask(yourFunction, newInterval [, newTaskStatus]);
------------------------------------------

<newTaskStatus> can be <ONETIME> if you want to transform a normal task into a
one-time task, or <SCHEDULED> if you want to transform a one-time task into
a normal task. Keep in mind that in this case the one-time task has still to be executed
because as soon as it is executed, it will be permanently deleted.


To check if a task is running, you have to use the <taskIsRunning()> method:
------------------------------------------
myOS.getTaskStatus(yourFunction);
------------------------------------------

This will return 255 if there was an error (task not found) or a value for the
current status:
PAUSED (or 0) - task is paused/not running
SCHEDULED (or 1) - task is running
ONETIME (or 2) - task is scheduled to run in a near future.

The WatchDog is also used to reset the microcontroller by software. If you need
to do that, call the method <reset()> at any time and from any place of your
sketch instead of using the usual codes based on WatchDog:
------------------------------------------
myOS.reset();
------------------------------------------


Introduced with leOS2 2.2.0 there are 2 new methods that permit to stop/restart
the scheduler: they are useful if you need to stop all the running tasks at the
same time. 
The method below stops the scheduler and freezes all the tasks preserving their current
intervals:
------------------------------------------
myOS.haltScheduler();
------------------------------------------

This one restarts the scheduler resuming all the tasks that were running.
------------------------------------------
myOS.restartScheduler();
------------------------------------------


**BE CAREFUL**
the user is asked to check his code to avoid strange logical situations when
he pauses a task. I.e.: if the task that has been paused alternated the output of
a pin and that pin drove an external circuit, the user should check if the status
of the pin after the task has been paused is safe and compatible with his needs.


******************************************************************************************
******************************************************************************************
32-/64-bits math
----------------

Starting from version 0.1.1, the user can choose between 32-bits and 64-bits math.
Using 32-bits math, the maximum interval that can be chosen is limited to 49.7 days.
The code won't run into an overflow error for intervals that have to start after the 49.7
days limit (this has has been fixed since version 1.0.1) but the user can not set an
interval longer than this period.

While using 64-bits math the maximum that can be choosen is only limited by the
user's fantasy, due to the fact that the 64-bits counter will overflow after
584,942,417 years (default maximum interval is 1 hour, but you can change this
value editing the <leOS2::addTask> method inside the <leOS2.cpp> file).

So, why use 32-bits math if 64-bits seems to be better? Because 64-bits math adds
some overload for the compiler so the size of the final sketch is usually bigger
than if using 32-bits math. So, choose the one that fits your sketch needs. 

To switch between 32-bits and 64-bits math just comment/uncomment the following line
that is present at the beginning of the code inside the leOS2.h file:
------------------------------------------
#define SIXTYFOUR_MATH
------------------------------------------


******************************************************************************************
******************************************************************************************
How it works leOS2
------------------

The scheduler makes use of a special counter that is used by the WatchDog circuit.
The WatchDog is a circuit that can raise an interrupt or a system reset after a specific
time. Usually the WatchDog is used to reset the microcontroller: this is useful if, for 
example the code freezes during a particular task, i.e. during datas that should had to
come over an ethernet connection. If the timer expires, the WatchDog will reset the 
microcontroller. This timer is incremented using the 128 kHz internal oscillator: the 
clock is divided by a prescaler circuit set to factor 2,048 so that the minimum interval
is:
128,000/2,048=62.5 Hz
1/62.5 = 0.016s -> 16 ms

Starting from version 2.0.90, leOS2 can be initialized to set the WDT in "interrupt and 
system reset" mode. In this modality, WDT first raises an interrupt and then, at the next
timeout, it resets the microcontroller. The sequence can be halted by setting to “1” the
bit flag WDIE just after the interrupt has been raised so that the next timeout the WDT
will raise an interrupt again. This is done inside the scheduler. The user can pass to
leOS2 a timeout value during the initialization of the scheduler: leOS2 will use that
value to monitor if a task has freezed during its execution. Every time that the scheduler
is called, it checks if a task is running: if yes, a counter, that has been initiliazed
with the timeout value set by the user, is decremented. If its value if greater than zero,
the WDIE bit is set to “1”; when it reaches zero, the scheduler does set the WDIE bit to
“0”. This is intercepted by WDT the next time that its timer will expire: if the WDT sees
that the WDIE bit is at “0”, it will reset the microcontroller.

Remember that the usage of leOS2 will interfere with any code or library that uses the
WatchDog.


******************************************************************************************
******************************************************************************************
Supported microcontrollers
--------------------------

leOS2 can work on almost every Atmel microcontroller that is supported by the GNU gcc AVR
compiler and the Arduino IDE (through specific cores). At the moment, the only MCU
supported by the Arduino IDE but NOT supported by leOS2 is the Atmega8/A due to the fact
that the WDT of this chip isn't able to generate an interrupt signal but it can only raise
a reset signal.


Notice for Arduino Yun users:
while the ATmega32U4 core on that board is the same of the Arduino Leonardo, there is a
very different environment behind it. The Yun is a particular board with a bridge that
connects this core with a SoC that runs a Linux-based OS. You can use the leOS2 library
on the ATmega32U4 but the reset function will not work because it only resets the 32U4,
not both the chips.


******************************************************************************************
******************************************************************************************
WARNING - IMPORTANT ADVICE FOR ARDUINO MEGA/MEGA2560 OWNERS
-----------------------------------------------------------

The original bootloader flashed into the models of the Arduino MEGA and MEGA2560
boards doesn’t deactivate the watchdog at the microcontroller’s startup so the board will
freeze itself in a neverending loop caused by eternal resets called by the WDT. To solve
this problem, users that want to use leOS2 have to change the bootloader with one that it
isn’t affected by this issue. The bootloader can be downloaded by this page:
https://github.com/arduino/Arduino-stk500v2-bootloader/tree/master/goodHexFiles


******************************************************************************************
******************************************************************************************
Licence
-------

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public	License as published by the Free Software
Foundation; either version 3.0 of the License, or (at your option) any later
version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


******************************************************************************************
******************************************************************************************
Document revision
-----------------

16th revision: 2016/11/06
