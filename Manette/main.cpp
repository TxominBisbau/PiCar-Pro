#include <iostream>
#include <csignal>
#include <atomic>
#include "src/motors.h"
#include "src/manette.h"
#include "src/servo.h"

std::atomic<bool> running(true);
void signalHandler(int) { running = false; }

int main(int argc, char** argv) {
    std::cout << "=== PiCar Pro — Contrôle manette ===\n";
    std::cout << "  Stick gauche : Avancer & Tourner\n";
    std::cout << "  START : Stop/Start \n\n";

    std::signal(SIGINT,  signalHandler);
    std::signal(SIGTERM, signalHandler);

    if (!Motors::init()) return EXIT_FAILURE;
    if (!Servo::init())  return EXIT_FAILURE;
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        return EXIT_FAILURE;
    }
    int port = std::stoi(argv[1]);
    if (!Manette::init(port)) return EXIT_FAILURE;

    while (running) {
        EtatManette etat = Manette::read();
        int spd = Motors::getSpeed();
        int angle = Servo::getAngle();

        if (etat.btn & BTN_START || (etat.yL >= 24000 && etat.yL <= 40000)) {
            Motors::stop();
        }
        if (etat.yL > 40000) {
            if (etat.yL >= 64000){
                spd = 100;
            }
            else {
                spd = (etat.yL - 40000) / 240;
            }
            Motors::backward(spd);
        }
        if (etat.yL < 24000) {
            spd = (24000 - etat.yL) / 240;
            Motors::forward(spd);
        }

        if (etat.xL >= 24000 && etat.xL <= 40000) {
            Servo::center();
        }
        if (etat.xL > 40000) {
            if (etat.xL >= 64000) {
                angle = 60;
            }
            else {
                angle = 90 - (etat.xL-40000)*3/2400;
            }
            Servo::setAngle(angle);
        }
        if (etat.xL < 24000) {
            angle = 120 - etat.xL*3/2400;
            Servo::setAngle(angle);
        }

        if (etat.btn & BTN_SELECT) running = false;
    }

    Motors::stop();
    Servo::center();
    Manette::cleanup();
    Motors::cleanup();
    Servo::cleanup();

    std::cout << "\n[INFO] Arrêt propre.\n";
    return EXIT_SUCCESS;
}