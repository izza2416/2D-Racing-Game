#ifndef AICAR_H
#define AICAR_H

#include "car.h"
#include <cstdlib>

// AICar moves automatically toward the finish line.
// It can also randomly shift left or right while advancing.
class AICar : public Car {
public:
    AICar(float s, int r, int c, string sym) : Car(s, r, c, sym) {}

    void move() override {
        // Always move down one row each AI update.
        row++;

        // Randomly choose to stay, move left, or move right.
        int choice = rand() % 3;

        if (choice == 0) {
            col--;
        }
        else if (choice == 1) {
            col++;
        }
    }
};

#endif