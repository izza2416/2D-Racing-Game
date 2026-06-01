#ifndef GAME2_H
#define GAME2_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "AIcar.h"
#include "playercar.h"
#include "track.h"

using namespace std;

// Game manages the track, car objects, and main game loop.
class Game {
private:
    // Store player and AI car objects.
    vector<Car*> cars;
    // Flag that keeps the game loop active until someone wins or the window closes.
    bool gameRunning;
    // The race track layout and obstacle data.
    Track track;
    // Symbol of the car that wins the race.
    string winnerSymbol;

public:
    Game(int trackWidth, int trackHeight)
        : track(trackWidth, trackHeight), gameRunning(true), winnerSymbol("") {
        // Seed random behavior for AI movement.
        srand(time(0));
    }

    ~Game() {
        // Remove all cars from heap memory when the game ends.
        for (int i = 0; i < cars.size(); i++) {
            delete cars[i];
        }
    }

    void initializeGame() {
        // Set up the player and AI cars at their start positions.
        cars.push_back(new PlayerCar(1.0, 0, 1, "P"));
        cars.push_back(new AICar(1.0, 0, 5, "A"));

        // Record initial game state to a log file.
        ofstream outFile("gamedata.txt");
        outFile << "Game initialized with track size "
                << track.getWidth() << "x" << track.getHeight() << endl;
        outFile << "Player car starts at (0, 1) with speed 1.0\n";
        outFile << "AI car starts at (0, 5) with speed 1.0\n";
        outFile.close();
    }

    bool checkCollision(int currentIndex) {
        // Compare the current car position with every other car.
        for (int j = 0; j < cars.size(); j++) {
            if (j != currentIndex) {
                if (cars[currentIndex]->getRow() == cars[j]->getRow() &&
                    cars[currentIndex]->getCol() == cars[j]->getCol()) {
                    return true;
                }
            }
        }
        return false;
    }

    void movePlayer(char command) {
        // Save the player's current location in case the move must be undone.
        int oldRow = cars[0]->getRow();
        int oldCol = cars[0]->getCol();

        PlayerCar* player = dynamic_cast<PlayerCar*>(cars[0]);

        if (player != nullptr) {
            player->moveByKey(command);
        }

        // Validate the new position and cancel invalid moves.
        if (!track.iswithinbounds(cars[0]->getRow(), cars[0]->getCol())) {
            cars[0]->setRow(oldRow);
            cars[0]->setCol(oldCol);
            cout << "Player hit boundary! Move cancelled.\n";
        }
        else if (track.isObstacle(cars[0]->getRow(), cars[0]->getCol())) {
            cars[0]->setRow(oldRow);
            cars[0]->setCol(oldCol);
            cout << "Player hit obstacle! Move cancelled.\n";
        }
        else if (checkCollision(0)) {
            cars[0]->setRow(oldRow);
            cars[0]->setCol(oldCol);
            cout << "Player hit another car! Move cancelled.\n";
        }
    }

    void updateAI() {
        // Update every AI-controlled car.
        // Each AI move is checked for validity after stepping.
        for (int i = 1; i < cars.size(); i++) {
            int oldRow = cars[i]->getRow();
            int oldCol = cars[i]->getCol();

            cars[i]->move();

            // Cancel the AI move if it goes off track, hits an obstacle, or collides.
            if (!track.iswithinbounds(cars[i]->getRow(), cars[i]->getCol()) ||
                track.isObstacle(cars[i]->getRow(), cars[i]->getCol()) ||
                checkCollision(i)) {
                cars[i]->setRow(oldRow);
                cars[i]->setCol(oldCol);
            }
        }
    }

    void checkWinner() {
        // Look for a car that has crossed the finish line.
        // Finish is detected when a car reaches the last row.
        for (int i = 0; i < cars.size(); i++) {
            if (cars[i]->getRow() >= track.getHeight() - 1) {
                winnerSymbol = cars[i]->getSymbol();
                cout << cars[i]->getSymbol() << " wins the race!" << endl;
                gameRunning = false;
            }
        }
    }

    void saveWinner() {
        // Append the game result to the log file.
        ofstream outFile("gamedata.txt", ios::app);

        if (winnerSymbol != "") {
            // Log the winner symbol when the race completes.
            outFile << "Winner of the game is " << winnerSymbol << "\n";
        }
        else {
            // Log the case where the window closed before a finish.
            outFile << "Game closed before winner.\n";
        }

        outFile.close();
    }

    void drawGame(sf::RenderWindow& window, int cellSize) {
        // Render the current track and car positions each frame.
        window.clear();
        track.drawGraphics(window, cars, cellSize);
        window.display();
    }

    void handleInput() {
        // Check keyboard keys and move the player accordingly.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movePlayer('w');
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movePlayer('a');
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movePlayer('s');
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movePlayer('d');
        }
    }

    void startGame() {
        initializeGame();

        const int cellSize = 50;

        sf::RenderWindow window(
            sf::VideoMode(track.getWidth() * cellSize,
                          track.getHeight() * cellSize),
            "2D Racing Game GUI"
        );

        window.setFramerateLimit(10);

        sf::Clock aiClock;
        sf::Clock inputClock;

        // Main game loop: process window events, input, AI, and rendering.
        while (window.isOpen()) {
            sf::Event event;

            while (window.pollEvent(event)) {
                // Handle the window close event from the OS.
                if (event.type == sf::Event::Closed) {
                    window.close();
                    gameRunning = false;
                }
            }

            // Process player input on a short timer.
            if (gameRunning &&
                inputClock.getElapsedTime().asMilliseconds() >= 150) {
                handleInput();
                checkWinner();
                inputClock.restart();
            }

            // Update AI movement less frequently than player input.
            if (gameRunning &&
                aiClock.getElapsedTime().asMilliseconds() >= 700) {
                updateAI();
                checkWinner();
                aiClock.restart();
            }

            drawGame(window, cellSize);

            if (!gameRunning) {
                // End the game cleanly and give the player a short pause.
                saveWinner();
                sf::sleep(sf::seconds(2));
                window.close();
            }
        }

        cout << "Game Over!" << endl;
    }
};

#endif