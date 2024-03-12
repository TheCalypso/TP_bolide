/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdio>
#include <cstdint>
#include <chrono>

/* CONST */
#define WAIT_TIME_500_MS 500
#define WAIT_TIME_750_MS 750

/* OUT */
PwmOut buzzer(A1);


/* IN */
DigitalIn bp_buzzer(D1, PullUp);

/* Var */


int main()
{
    printf("Running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    // cmd_frein.write(0); // place le niveau bas =  0V sur la broche A2
    
    buzzer.write(0.5);

    while (true)
    {
        if (bp_buzzer == 0){
            buzzer.period_ms(1);
        }
        else {
            buzzer.period_ms(50000);
        }
        thread_sleep_for(2);
        
    }
}
