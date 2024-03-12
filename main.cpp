/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdio>
#include <cstdint>
#include <chrono>

#define WAIT_TIME_MS 500
/* OUT */

// DigitalOut led1(LED1);
// DigitalOut cmd_frein(A2);

DigitalOut cmd_phare(D0);


/* IN */
// DigitalIn bp_frein(D11, PullUp);
AnalogIn photor(A0);

/* Var */
bool etat_bp_frein;
uint16_t luminosite;
char txt[30];

int main()
{
    printf("Running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    // cmd_frein.write(0); // place le niveau bas =  0V sur la broche A2

    while (true)
    {
        luminosite = photor.read_u16();
        sprintf(txt, "valeur_lum: %d\n", luminosite);
        printf("%s", txt);

        if(luminosite < 30000){
            cmd_phare.write(1);
        } else {
            cmd_phare.write(0);
        }

        thread_sleep_for(WAIT_TIME_MS);
    }
}
