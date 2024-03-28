#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <chrono>

/* CONST */
#define WAIT_TIME_MS 100
// Définition des seuils pour la position du volant

/* CONST */
#define SEUIL_VOLANT_DROIT 40000 // Ajusté pour détection de virage à droite
#define SEUIL_VOLANT_GAUCHE 20000 // Ajusté pour détection de virage à gauche
#define SEUIL_CENTRE_HAUT 32000 // Ajusté pour la zone haute du centre
#define SEUIL_CENTRE_BAS 28000 // Ajusté pour la zone basse du centre

/* OUT */
BusOut bargraph(D2,D3,D4,D5,D6,D7,D8,D9);


/* IN */
InterruptIn bp_frein(D11, PullUp);
InterruptIn bp_buzzer(D1, PullUp);
InterruptIn echo(A6);
DigitalIn bp_cli_D(D10, PullUp);
DigitalIn bp_cli_G(D12, PullUp);
AnalogIn volant(A5);
AnalogIn photor(A0);

/* OUT */
PwmOut cmd_frein(A2);
DigitalOut trig(D13);
PwmOut buzzer(A1);
DigitalOut cmd_cli_D(A4);
DigitalOut cmd_cli_G(A3);
Ticker t_cli;
DigitalOut cmd_phare(D0);


/* VAR */
Timer t_chrono;
int distance_cm;
int mesure_distance;
int duree_us;
char txt[25];
bool flag_echo;
bool etat_bp_frein = 1, etatbp_cli_D, etatbp_cli_G;
bool etat_cli;
int sm_state, next_sm_state;
uint16_t position_volant;
uint16_t luminosite;

void clignoter() {
    etat_cli = !etat_cli;
}

void echo_start() {
    t_chrono.reset();
    t_chrono.start();
}

void echo_stop() {
    t_chrono.stop();
    flag_echo = true;
}

void rise_bp_frein(){
    etat_bp_frein = 1;
    cmd_frein.write(0);
}

void fall_bp_frein(){
    etat_bp_frein = 0;
    cmd_frein.write(1);
}

void rise_bp_buzzer(){
    buzzer.suspend();
}

void fall_bp_buzzer(){
    buzzer.resume();
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

void init_clignotant(){
    etat_cli = 0;
    t_cli.attach_us(&clignoter, WAIT_TIME_MS * 1000);

    sm_state = next_sm_state = 0;
    cmd_cli_D.write(0);
    cmd_cli_G.write(0);
}

void init_radar_de_recul(){
    trig = 0;
    t_chrono.reset();
    
    echo.rise(&echo_start);
    echo.fall(&echo_stop);
}

void init_freins_phares(){
    cmd_frein.period_ms(1);
    bp_frein.rise(&rise_bp_frein);
    bp_frein.fall(&fall_bp_frein);
}

void init_klaxon(){
    buzzer.write(0.5);
    buzzer.period_ms(1);
    buzzer.suspend();
    bp_buzzer.rise(&rise_bp_buzzer);
    bp_buzzer.fall(&fall_bp_buzzer);
}

void gestion_clignotants(){
    position_volant = volant.read_u16();
    // sprintf(txt, "position volant: %d\n", position_volant);
    // printf("%s", txt);

    etat_bp_frein = bp_frein.read();
    etatbp_cli_D = bp_cli_D.read();
    etatbp_cli_G = bp_cli_G.read();

    // Gestion des états selon la position du volant et les boutons
    switch(sm_state) {
        case 0: // État initial, clignotants éteints
            cmd_cli_G.write(0);
            cmd_cli_D.write(0);
            if(etatbp_cli_D == 0) next_sm_state = 1; // Activation par bouton droit
            else if(etatbp_cli_G == 0) next_sm_state = 2; // Activation par bouton gauche
            break;
        case 1: // Clignotant droit activé par bouton
            cmd_cli_D.write(etat_cli);
            if(etat_bp_frein == 0 || (position_volant <= SEUIL_CENTRE_HAUT && position_volant >= SEUIL_CENTRE_BAS)) next_sm_state = 0; // Désactivation
            break;
        case 2: // Clignotant gauche activé par bouton
            cmd_cli_G.write(etat_cli);
            if(etat_bp_frein == 0 || (position_volant <= SEUIL_CENTRE_HAUT && position_volant >= SEUIL_CENTRE_BAS)) next_sm_state = 0; // Désactivation
            break;
    }
    sm_state = next_sm_state;
}
void gestion_radar_de_recul(){
    trig = 1;
    wait_us(15);
    trig = 0;
    if(flag_echo){
        duree_us = t_chrono.read_us();
        distance_cm = duree_us / 58;
        gestion_bargraph(distance_cm);
        flag_echo = false;
    }
}

void gestion_allumage_phare(){
    luminosite = photor.read_u16();
    // sprintf(txt, "%d\n", etat_bp_frein);
    // printf("%s", txt);

    if(luminosite < 30000){
        cmd_phare.write(1);
        // sprintf(txt, "%d", etat_bp_frein);
        // printf("%s", txt);
        if (etat_bp_frein == 1){
            cmd_frein.write(0.1);
        }
    } else {
        cmd_phare.write(0);
        if (etat_bp_frein == 1){
            cmd_frein.write(0);
        }
    }
}

int main() {
    init_radar_de_recul();
    init_freins_phares();
    init_klaxon();
    init_clignotant();

    while(true) {
        gestion_radar_de_recul();
        gestion_clignotants();
        gestion_allumage_phare();
        thread_sleep_for(2);
    }
}

