#pragma once

struct EtatManette {
    int          xL  = 32768;  // Stick gauche
    int          yL  = 32768; 
    int          xR  = 32768;  // Stick droit
    int          yR  = 32768; 
    unsigned int pov = 65535;   // DPad
    unsigned long btn = 0;  // Boutons
};

enum Bouton {
    BTN_TRIANGLE = 0x1,
    BTN_ROND     = 0x2,
    BTN_CROIX    = 0x4,
    BTN_CARRE    = 0x8,
    BTN_L1       = 0x10,
    BTN_R1       = 0x20,
    BTN_L2       = 0x40,
    BTN_R2       = 0x80,
    BTN_SELECT   = 0x100,
    BTN_START    = 0x200,
    BTN_L3       = 0x400,
    BTN_R3       = 0x800,
};
 
namespace Manette {
    bool init(int port); 
    void cleanup();
    EtatManette read();
}