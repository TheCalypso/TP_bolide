#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <chrono>

/* CONST */
#define WAIT_TIME_MS 2000
// Définition des seuils pour la position du volant


/* OUT */
DigitalOut cmd_frein(A2);
DigitalOut led1(LED1);
PwmOut buzzer(A1);


/* IN */
InterruptIn bp_frein(D11, PullUp);
InterruptIn bp_buzzer(D1, PullUp);

void rise_bp_frein(){
    cmd_frein.write(0);
}

void fall_bp_frein(){
    cmd_frein.write(1);
}

void rise_bp_buzzer(){
    buzzer.period_ms(50000);
}

void fall_bp_buzzer(){
    buzzer.period_ms(1);
}

int main() {
    cmd_frein.write(0);
    buzzer.write(0.5);
    
    bp_frein.rise(&rise_bp_frein);
    bp_frein.fall(&fall_bp_frein);

    bp_buzzer.rise(&rise_bp_buzzer);
    bp_buzzer.fall(&fall_bp_buzzer);

    while (true) {
        led1=!led1;
        thread_sleep_for(WAIT_TIME_MS);
    }
}
