#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <stack>
#include <vector>
#include <random>
#include "player.h"
#include "imposter.h"

class Maze {

public:
    int rows;
    int columns;
    std::vector<std::vector<char>> maze;
    std::vector<std::vector<std::pair<int, int>>> adjList;

    Maze(int R = 5, int C = 5);

    ~Maze();

    std::string computePath(Player *player, Imposter *imposter);

    bool detectWin(Player *pPlayer);

private:
    static int getIdx(int x, int y, const std::vector<std::pair<int, std::pair<int, int>>> &cell_list);

    void displayMaze() const;

};

#endif //MAZE_H
