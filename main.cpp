#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <chrono>

/* CONST */
#define WAIT_TIME_MS 100
// Définition des seuils pour la position du volant


/* OUT */
BusOut bargraph(D2,D3,D4,D5,D6,D7,D8,D9);


/* IN */

/* VAR */
DigitalOut trig(D13);
InterruptIn echo(A6);
Timer t_chrono;
int distance_cm;
int mesure_distance;
int duree_us;
char txt[25];
bool flag_echo;

void echo_start() {
    t_chrono.reset();
    t_chrono.start();
}

void echo_stop() {
    t_chrono.stop();
    flag_echo = true;
}

void gestion_bargraph(int distance){
    if(distance < 5){
        bargraph.write(0xFF);
    }
    if(distance > 5 && distance < 10){
        bargraph.write(0xE7);
    }
    if(distance > 10 && distance < 15){
        bargraph.write(0xC3);
    }
    if(distance > 15 && distance < 20) {
        bargraph.write(0x81);
    }
    if(distance > 20){
        bargraph.write(0x00);
    }
}

int main() {
    // Initialisation
    trig = 0;
    t_chrono.reset();
    
    echo.rise(&echo_start); // Associer le front montant du signal écho à echo_start
    echo.fall(&echo_stop);  // Associer le front descendant du signal écho à echo_stop

    while(true) {
        trig = 1;
        wait_us(15);
        trig = 0;
        if(flag_echo){
            printf("Echo stop\n");
            duree_us = t_chrono.read_us();
            distance_cm = duree_us / 58;
            printf("Durée (us): %d\n", duree_us);
            printf("Distance (cm): %d\n", distance_cm);
            gestion_bargraph(distance_cm);
            flag_echo = false;
        }
        thread_sleep_for(100); // Pause de 100 ms pour la prochaine mesure
    }
}


