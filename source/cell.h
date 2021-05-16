#ifndef CELL_H
#define CELL_H

class Cell {
public:
    bool northEdge;
    bool eastEdge;
    bool westEdge;
    bool southEdge;
    int row;
    int column;

    Cell(int row = 0, int column = 0);

    ~Cell();

    void connectTo(Cell *cell);

    bool isConnectedTo(Cell *cell);

    int equals(Cell *cell);

    int getWalls();
};

#endif //CELL_H
