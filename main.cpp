#include <iostream>
#include "game2.h"

using namespace std;

// Entry point for the racing game application.
int main() {
    // Create the game with a 15x10 track.
    Game game(15, 10);

    // Start the main game loop.
    game.startGame();

    return 0;
}