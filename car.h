#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <string>
using namespace std;

// Base car class used for both player and AI cars.
// It stores position, speed, and a display symbol.
class Car {
protected:
    float speed;
    int row, col;
    string symbol;

public:
    Car(float s, int r, int c, string sym) {
        speed = s;
        row = r;
        col = c;
        symbol = sym;
    }

    virtual ~Car() {}

    int getRow() const { return row; }
    int getCol() const { return col; }
    string getSymbol() const { return symbol; }

    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }

    // Derived classes must implement their own move behavior.
    virtual void move() = 0;

    void display() {
        // Print car state for debugging or console output.
        cout << "Car " << symbol << " at (" << row << ", " << col
             << ") with speed " << speed << endl;
    }
};

#endif