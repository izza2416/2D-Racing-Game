#ifndef TRACK_H
#define TRACK_H

#include "car.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class Track {
protected:
    int width;
    int height;
    float roadOffset;  // Offset for animation of the road

public:
    Track(int w, int h) : width(w), height(h), roadOffset(0) {}

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    bool iswithinbounds(int r, int c) {
        return (r >= 0 && r < height && c > 0 && c < width - 1);
    }

    bool isObstacle(int r, int c) {
        return ((r == 2 && c == 1) ||
                (r == 5 && c == 5) ||
                (r == 3 && c == 12));
    }

    void display(const vector<Car*>& cars) {
        vector<vector<string>> grid(height, vector<string>(width, " "));

        for (int i = 0; i < height; i++) {
            grid[i][0] = "|";
            grid[i][width - 1] = "|";
        }

        if (height > 2 && width > 3) grid[2][1] = "X";
        if (height > 5 && width > 7) grid[5][5] = "X";
        if (height > 3 && width > 12) grid[3][12] = "X";

        for (int i = 0; i < cars.size(); i++) {
            int r = cars[i]->getRow();
            int c = cars[i]->getCol();

            if (r >= 0 && r < height && c > 0 && c < width - 1) {
                grid[r][c] = cars[i]->getSymbol();
            }
        }

        cout << "\n===== TRACK =====\n";

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cout << grid[i][j];
            }
            cout << endl;
        }
    }

    void drawGraphics(sf::RenderWindow& window,
                      const vector<Car*>& cars,
                      int cellSize) {

        // Road animation effect (move only the road)
        roadOffset += 2.0f;  // Adjust the speed here for faster/slower animation
        if (roadOffset >= cellSize) {
            roadOffset = 0;  // Reset position when the road is out of view
        }

        // Draw the road surface (scrolling effect)
        sf::RectangleShape road;
        road.setSize(sf::Vector2f(width * cellSize, cellSize));
        road.setFillColor(sf::Color(45, 45, 45));
        
        // Drawing the road scrolling effect
        for (int i = -1; i < height + 1; i++) {
            road.setPosition(0, (i * cellSize) - roadOffset);  // Animate the road
            window.draw(road);
        }

        // Static objects: Red left boundary
        sf::RectangleShape leftBoundary;
        leftBoundary.setSize(sf::Vector2f(cellSize, height * cellSize));
        leftBoundary.setPosition(0, 0);
        leftBoundary.setFillColor(sf::Color::Red);
        window.draw(leftBoundary);

        // Static objects: Red right boundary
        sf::RectangleShape rightBoundary;
        rightBoundary.setSize(sf::Vector2f(cellSize, height * cellSize));
        rightBoundary.setPosition((width - 1) * cellSize, 0);
        rightBoundary.setFillColor(sf::Color::Red);
        window.draw(rightBoundary);

        // Static finish line (black and white)
        int finishY = (height - 1) * cellSize;
        int squareSize = cellSize / 2;

        for (int x = 0; x < width * cellSize; x += squareSize) {
            sf::RectangleShape square;
            square.setSize(sf::Vector2f(squareSize, squareSize));
            square.setPosition(x, finishY);

            if ((x / squareSize) % 2 == 0) {
                square.setFillColor(sf::Color::Black);
            }
            else {
                square.setFillColor(sf::Color::White);
            }

            window.draw(square);
        }

        // Draw white dashed lines in the middle of the track (vertical)
        int middleX = width / 2 * cellSize;  // Middle column of the track

        for (int y = 0; y < height * cellSize; y += cellSize * 2) {  // Create dashes
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(5, cellSize));  // Make them thinner vertically
            line.setPosition(middleX - 2, y - roadOffset);  // Position along middle column
            line.setFillColor(sf::Color::White);
            window.draw(line);
        }

        int middleX2 = (width / 2 + 1) * cellSize;  // Second dashed line in the middle

        for (int y = 0; y < height * cellSize; y += cellSize * 2) {  // Create dashes
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(5, cellSize));  // Make them thinner vertically
            line.setPosition(middleX2 - 2, y - roadOffset);  // Position along the other middle column
            line.setFillColor(sf::Color::White);
            window.draw(line);
        }

        // Draw obstacles (cone-shaped)
        int obsRows[3] = {2, 5, 3};
        int obsCols[3] = {1, 5, 12};

        for (int i = 0; i < 3; i++) {
            int r = obsRows[i];
            int c = obsCols[i];

            if (r >= 0 && r < height && c > 0 && c < width - 1) {

                float x = c * cellSize;
                float y = r * cellSize;

                // Cone body (triangle shape)
                sf::ConvexShape cone;
                cone.setPointCount(3);
                cone.setPoint(0, sf::Vector2f(x + cellSize * 0.50f, y + cellSize * 0.10f)); // Top point
                cone.setPoint(1, sf::Vector2f(x + cellSize * 0.20f, y + cellSize * 0.80f)); // Bottom left
                cone.setPoint(2, sf::Vector2f(x + cellSize * 0.80f, y + cellSize * 0.80f)); // Bottom right
                cone.setFillColor(sf::Color(255, 120, 0)); // Orange color
                cone.setOutlineThickness(2);
                cone.setOutlineColor(sf::Color::Black);
                window.draw(cone);

                // White stripe on cone
                sf::RectangleShape stripe;
                stripe.setSize(sf::Vector2f(cellSize * 0.38f, cellSize * 0.08f));
                stripe.setPosition(x + cellSize * 0.31f, y + cellSize * 0.47f);
                stripe.setFillColor(sf::Color::White);
                window.draw(stripe);

                // Cone base
                sf::RectangleShape base;
                base.setSize(sf::Vector2f(cellSize * 0.75f, cellSize * 0.12f));
                base.setPosition(x + cellSize * 0.125f, y + cellSize * 0.80f);
                base.setFillColor(sf::Color(40, 40, 40));  // Dark gray
                window.draw(base);
            }
        }

        // Draw each car on the track
        for (int i = 0; i < cars.size(); i++) {

            float x = cars[i]->getCol() * cellSize;
            float y = cars[i]->getRow() * cellSize;

            sf::Color carColor;

            if (cars[i]->getSymbol() == "P") {
                carColor = sf::Color::Blue;
            }
            else {
                carColor = sf::Color::Green;
            }

            // Main car body
            sf::RectangleShape body;
            body.setSize(sf::Vector2f(cellSize * 0.60f, cellSize * 0.80f));
            body.setPosition(x + cellSize * 0.20f, y + cellSize * 0.10f);
            body.setFillColor(carColor);
            body.setOutlineThickness(2);
            body.setOutlineColor(sf::Color::Black);
            window.draw(body);

            // Car window
            sf::RectangleShape windowPart;
            windowPart.setSize(sf::Vector2f(cellSize * 0.35f, cellSize * 0.25f));
            windowPart.setPosition(x + cellSize * 0.325f, y + cellSize * 0.20f);
            windowPart.setFillColor(sf::Color(180, 220, 255));
            window.draw(windowPart);

            // Wheels
            sf::RectangleShape wheel1;
            wheel1.setSize(sf::Vector2f(cellSize * 0.12f, cellSize * 0.20f));
            wheel1.setPosition(x + cellSize * 0.10f, y + cellSize * 0.22f);
            wheel1.setFillColor(sf::Color::Black);
            window.draw(wheel1);

            sf::RectangleShape wheel2;
            wheel2.setSize(sf::Vector2f(cellSize * 0.12f, cellSize * 0.20f));
            wheel2.setPosition(x + cellSize * 0.10f, y + cellSize * 0.62f);
            wheel2.setFillColor(sf::Color::Black);
            window.draw(wheel2);

            sf::RectangleShape wheel3;
            wheel3.setSize(sf::Vector2f(cellSize * 0.12f, cellSize * 0.20f));
            wheel3.setPosition(x + cellSize * 0.78f, y + cellSize * 0.22f);
            wheel3.setFillColor(sf::Color::Black);
            window.draw(wheel3);

            sf::RectangleShape wheel4;
            wheel4.setSize(sf::Vector2f(cellSize * 0.12f, cellSize * 0.20f));
            wheel4.setPosition(x + cellSize * 0.78f, y + cellSize * 0.62f);
            wheel4.setFillColor(sf::Color::Black);
            window.draw(wheel4);
        }
    }
};

#endif