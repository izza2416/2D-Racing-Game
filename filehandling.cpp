#include <fstream>
#include "game2.h"

// Simple standalone file writer for game initialization data.
int main() {
    // Open the output file for writing game state information.
    ofstream outFile("gamedata.txt");

    // Write the initial track dimensions to the file.
    outFile << "Game initialized with track size "
            << track.getWidth() << "x" << track.getHeight() << endl;

    // Close the file when done.
    outFile.close();
    return 0;
}
