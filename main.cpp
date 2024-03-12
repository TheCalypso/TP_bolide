/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#define WAIT_TIME_MS 500 
// DigitalOut led1(LED1);
DigitalOut cmd_frein(A2);

int main()
{
    printf("Running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    cmd_frein.write(0); // place le niveau bas =  0V sur la broche A2

    while (true)
    {
        // led1 = !led1;
        // thread_sleep_for(WAIT_TIME_MS);
        cmd_frein.write(0);
        thread_sleep_for(WAIT_TIME_MS);
        cmd_frein.write(1);
        thread_sleep_for(WAIT_TIME_MS);
    }
}
