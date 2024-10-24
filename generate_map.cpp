// generate_map.cpp
// Name: Vansh Joshi
// Date: 10/23/2025

// credit to chatGPT to for making this program

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <map_size>" << endl;
        return 1;
    }

    // Parse map size from command line
    int map_size = atoi(argv[1]);
    
    // Validate map size
    if (map_size <= 0) {
        cerr << "Map size must be positive" << endl;
        return 1;
    }

    // Generate filename based on map size
    string filename = "graph_map_" + to_string(map_size) + ".txt";
    
    // Define terrain types
    vector<char> terrain_types = {'f', 'g', 'G', 'h', 'm', 'r'};
    
    // Create and open output file
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return 1;
    }

    // Write terrain type definitions
    outFile << "6" << endl;               // Number of terrain types
    outFile << "f 3" << endl;             // Forest
    outFile << "g 1" << endl;             // Grass
    outFile << "G 2" << endl;             // Tall grass
    outFile << "h 4" << endl;             // Hills
    outFile << "m 7" << endl;             // Mountains
    outFile << "r 5" << endl;             // River

    // Write map dimensions
    outFile << map_size << " " << map_size << endl;

    // Generate and write random map
    srand(time(0));  // Initialize random seed
    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size; j++) {
            int rand_index = rand() % terrain_types.size();
            outFile << terrain_types[rand_index];
            if (j < map_size - 1) {
                outFile << " ";
            }
        }
        outFile << endl;
    }

    // Write start and end positions
    outFile << "0 0" << endl;                             // Start position
    outFile << map_size-1 << " " << map_size-1 << endl;   // End position

    outFile.close();
    cout << "Generated map file: " << filename << endl;

    return 0;
}