#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <chrono>

/* CONST */
#define WAIT_TIME_MS 2
// Définition des seuils pour la position du volant


/* OUT */
BusOut bargraph(D2,D3,D4,D5,D6,D7,D8,D9);


/* IN */

/* VAR */
int frontLeds;
bool sens, flag_bargraph; 
Ticker t_bargraph;

void gestion_bargraph(){
    if(sens == 1) {
            frontLeds = frontLeds*2;
        } else {
            frontLeds = frontLeds/2;
        }
        if (frontLeds == 0x80) sens = 0;
        if (frontLeds == 0x01) sens = 1;
        flag_bargraph =  1;
}
int main() {
    bargraph.write(0);
    frontLeds = 0x01;
    sens = 1;
    t_bargraph.attach_us(&gestion_bargraph, 800000);

    while (true) {
        if(flag_bargraph == 1) {
            bargraph.write(frontLeds);
            flag_bargraph = 0;
        }
        thread_sleep_for(WAIT_TIME_MS);
    }
}
