// Compilation : g++ main.cpp src/motors.cpp src/servo.cpp src/manette.cpp -o picarpro -std=c++17 -llgpio
//               sudo ./picarpro 5005

#include <iostream>
#include <csignal>
#include <atomic>
#include "src/motors.h"
#include "src/manette.h"
#include "src/pca9685.h"
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
    if (!PCA9685::init(SERVO_I2C_BUS, SERVO_I2C_ADDR, SERVO_FREQ)) return EXIT_FAILURE;

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        return EXIT_FAILURE;
    }
    int port = std::stoi(argv[1]);
    if (!Manette::init(port)) return EXIT_FAILURE;

    while (running) {
        EtatManette etat = Manette::read();
        int spd = Motors::getSpeed();
        int angle_direction = servoDirection.getAngle();

        // Moteurs
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

        // Servo de direction
        Servo servoDirection(CHANNEL_DIRECTION, 500.0f, 2400.0f, 90.0f);
        if (etat.xL >= 24000 && etat.xL <= 40000) {
            servoDirection.center();
        }
        if (etat.xL > 40000) {
            if (etat.xL >= 64000) {
                angle_direction = 30;
            }
            else {
                anangle_directiongle = 90 - (etat.xL-40000)*6/2400;
            }
            servoDirection.setAngle(angle_direction);
        }
        if (etat.xL < 24000) {
            angle_direction = 120 - etat.xL*6/2400;
            servoDirection.setAngle(angle_direction);
        }

        if (etat.btn & BTN_SELECT) running = false;
    }

    Motors::stop();
    Servo::center();
    Manette::cleanup();
    Motors::cleanup();
    PCA9685::cleanup();

    std::cout << "\n[INFO] Arrêt propre.\n";
    return EXIT_SUCCESS;
}