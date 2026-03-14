#pragma once

class Map {
public:
    static const int WIDTH = 10; // Map width
    static const int HEIGHT = 10;  // Map height

    int grid[HEIGHT][WIDTH];

    Map();                // constructor initializes the grid here
    bool isWall(int x, int y); // returns true if the cell at (x, y) is a wall
};