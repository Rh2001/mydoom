#include "Map.h"

Map::Map() {
    int temp[HEIGHT][WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,0,0,1},
        {1,0,1,0,0,0,1,0,0,1},
        {1,0,0,0,1,0,0,0,0,1},
        {1,0,1,0,1,0,1,0,0,1},
        {1,0,1,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1},
    };

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            grid[y][x] = temp[y][x];
}

bool Map::isWall(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return true;
    return grid[y][x] == 1;
}