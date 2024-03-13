#include "mbed.h"
#include <cstdint>
#include <cstdio>
#include <chrono>

/* CONST */
#define WAIT_TIME_MS 500
// Définition des seuils pour la position du volant
#define SEUIL_VOLANT_DROIT 40000 // Ajusté pour détection de virage à droite
#define SEUIL_VOLANT_GAUCHE 20000 // Ajusté pour détection de virage à gauche
#define SEUIL_CENTRE_HAUT 32000 // Ajusté pour la zone haute du centre
#define SEUIL_CENTRE_BAS 28000 // Ajusté pour la zone basse du centre

/* OUT */
DigitalOut cmd_cli_D(A4);
DigitalOut cmd_cli_G(A3);
Ticker t_cli;

/* IN */
DigitalIn bp_frein(D11, PullUp);
DigitalIn bp_cli_D(D10, PullUp);
DigitalIn bp_cli_G(D12, PullUp);
AnalogIn volant(A5);

/* Var */
bool etatbp_frein, etatbp_cli_D, etatbp_cli_G;
bool etat_cli;
int sm_state, next_sm_state;
uint16_t position_volant;
char txt[30];

void clignoter() {
    etat_cli = !etat_cli;
}

int main() {
    printf("Running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    etat_cli = 0;
    t_cli.attach_us(&clignoter, WAIT_TIME_MS * 1000);

    sm_state = next_sm_state = 0;
    cmd_cli_D.write(0);
    cmd_cli_G.write(0);

    while (true) {
        position_volant = volant.read_u16();
        // sprintf(txt, "position volant: %d\n", position_volant);
        // printf("%s", txt);

        etatbp_frein = bp_frein.read();
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
                if(etatbp_frein == 0 || (position_volant <= SEUIL_CENTRE_HAUT && position_volant >= SEUIL_CENTRE_BAS)) next_sm_state = 0; // Désactivation
                break;
            case 2: // Clignotant gauche activé par bouton
                cmd_cli_G.write(etat_cli);
                if(etatbp_frein == 0 || (position_volant <= SEUIL_CENTRE_HAUT && position_volant >= SEUIL_CENTRE_BAS)) next_sm_state = 0; // Désactivation
                break;
        }

        sm_state = next_sm_state;
        thread_sleep_for(20);
    }
}
