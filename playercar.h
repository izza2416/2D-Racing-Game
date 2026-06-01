#ifndef PLAYERCAR_H
#define PLAYERCAR_H

#include "car.h"

class PlayerCar : public Car {
public:
    PlayerCar(float s, int r, int c, string sym) : Car(s, r, c, sym) {}

    void move() override {
        // No automatic move logic for the player car.
        // Input is handled by the Game class instead.
    }

    void moveByKey(char command) {
        // Convert keyboard commands into car position changes.
        if (command == 'w') {
            row++;
        }
        else if (command == 's') {
            row--;
        }
        else if (command == 'a') {
            col--;
        }
        else if (command == 'd') {
            col++;
        }
    }
};

#endif