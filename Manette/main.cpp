// Compilation : g++ main.cpp src/motors.cpp src/pca9685.cpp src/servo.cpp src/manette.cpp -o picarpro -std=c++17 -llgpio
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

    Servo servoDirection(CHANNEL_DIRECTION, 500.0f, 2500.0f, 90.0f);
    Servo servoRotation(CHANNEL_ROTATION, 500.0f, 2500.0f, 90.0f);
    Servo servoIncline(CHANNEL_INCINE, 500.0f, 2500.0f, 90.0f);
    Servo servoPosition(CHANNEL_POSITION, 500.0f, 2400.0f, 90.0f);
    Servo servoOuvert(CHANNEL_OUVERT, 500.0f, 2400.0f, 90.0f);

    if (!Manette::init(port)) return EXIT_FAILURE;

    while (running) {
        EtatManette etat = Manette::read();
        int spd = Motors::getSpeed();
        int angle_direction = servoDirection.getAngle();
        int angle_rotation = servoRotation.getAngle();
        int angle_incline = servoIncline.getAngle();
        int angle_position = servoPosition.getAngle();
        int angle_ouvert = servoOuvert.getAngle();

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
        if (etat.xL >= 24000 && etat.xL <= 40000) {
            servoDirection.center();
        }
        if (etat.xL > 40000) {
            if (etat.xL >= 64000) {
                angle_direction = 30;
            }
            else {
                angle_direction = 90 - (etat.xL-40000)*6/2400;
            }
            servoDirection.setAngle(angle_direction);
        }
        if (etat.xL < 24000) {
            angle_direction = 150 - etat.xL*6/2400;
            servoDirection.setAngle(angle_direction);
        }

        // Servo de rotation de la pince
        if (etat.xR >= 24000 && etat.xR <= 40000) {
            servoRotation.center();
        }
        if (etat.xR > 40000) {
            if (etat.xR >= 64000) {
                angle_rotation = 30;
            }
            else {
                angle_rotation = 90 - (etat.xR-40000)*6/2400;
            }
            servoRotation.setAngle(angle_rotation);
        }
        if (etat.xR < 24000) {
            angle_rotation = 150 - etat.xR*6/2400;
            servoRotation.setAngle(angle_rotation);
        }

        // Servo d'inclinaison de la pince
        if (etat.yR >= 24000  && etat.yR <= 40000) {
            servoIncline.center();
        }
        if (etat.yR > 40000) {
            if (etat.yR >= 64000) {
                angle_incline = 120;
            }
            else {
                angle_incline = 60 + etat.yR*3/2400;
            }
            servoIncline.setAngle(angle_incline);
        }
        if (etat.yR < 24000) {
            angle_incline = 90 + (etat.yR-40000)*9/2400;
            servoIncline.setAngle(angle_incline);
        }

        // Servo de position de la pince
        if (etat.btn & BTN_R1) {
            angle_position += 1;
        }
        if (etat.btn & BTN_R2) {
            angle_position -= 1;
        }
        servoPosition.setAngle(angle_position);

        // Servo d'ouverture de la pince
        if (etat.btn & BTN_L1) {
            angle_ouvert += 1;
        }
        if (etat.btn & BTN_L2) {
            angle_ouvert -= 1;
        }
        servoOuvert.setAngle(angle_ouvert);
        

        if (etat.btn & BTN_SELECT) running = false;
    }

    Motors::stop();
    Manette::cleanup();
    Motors::cleanup();
    PCA9685::cleanup();

    std::cout << "\n[INFO] Arrêt propre.\n";
    return EXIT_SUCCESS;
}
