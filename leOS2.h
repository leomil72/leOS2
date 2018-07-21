/*
	leOS2.h - <l>ittle <e>mbedded <O>perating <S>ystem 2

    leOS2 is a simple scheduler to execute little routines in background,
    at specific intervals. leOS2 comes from leOS but instead of using
    an internal timer like leOS, it is based on the WatchDog Timer,
    a separated counter that is attached to an internal oscillator
    clocked at 128 kHz.

    For more infos, please read the README.txt file.

	Written by Leonardo Miliani <www DOT leonardomiliani DOT com>

    The latest version of this library can be found at:
    http://www.leonardomiliani.com/
    or
    https://github.com/leomil72

	Current version: 2.3.1 - 2016/02/20
    (for a complete history of the previous versions, see the README file)

  	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public
	License as published by the Free Software Foundation; either
	version 3.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    You should have received a copy of the GNU General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef leOS2_H
#define leOS2_H


//library version
#define leOS2_VERSION 233


//this library is compatible both with Arduino <=0023 and Arduino >=100
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


//uncomment this line if you want to use 64-bit math (more Flash consumption) - default is 32-bits math
//(read the README.txt file for more details about the difference between 32- & 64-bits math)

//#define SIXTYFOUR_MATH


//check MCU compatibility - leOS2 won't work on Atmega8
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega8A__)
#error Sorry, this MCU is not supported (lack of interrupt vector for WDT)!
#endif


//constants
const uint8_t PAUSED = 0;
const uint8_t SCHEDULED = 1; //0b00000001
const uint8_t SCHEDULED_IMMEDIATESTART = 5; //0b00000101
const uint8_t IMMEDIATESTART = SCHEDULED_IMMEDIATESTART; //alias for previous
const uint8_t ONETIME = 2;
const uint8_t NONE = 255;



//leOS2 class
class leOS2 {
	public:
		//public methods
		leOS2();
        void begin(uint16_t resetTimeout = 0);
		uint8_t addTask(void (*)(void), unsigned long, uint8_t taskStatus = SCHEDULED);
		uint8_t removeTask(void (*)(void));
		uint8_t pauseTask(void (*)(void));
        uint8_t restartTask(void (*)(void));
		uint8_t modifyTask(void (*)(void), unsigned long, uint8_t oneTimeTask = NONE);
		uint8_t getTaskStatus(void (*)(void));
        uint32_t convertMs(uint32_t);
        void haltScheduler(void);
		void restartScheduler(void);
        void reset(void);
	private:
        //private methods
        void setWDT();
        uint8_t setTask(void (*)(void), uint8_t, unsigned long taskInterval = NONE);
};


#endif
