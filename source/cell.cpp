#include "cell.h"

Cell::Cell(int row, int column) {
    this->row = row;
    this->column = column;
    this->northEdge = false;
    this->eastEdge = false;
    this->southEdge = false;
    this->westEdge = false;
}

Cell::~Cell() {
}

void Cell::connectTo(Cell *cell) {
    if (!cell) return;
    if (this->row == cell->row) {
        if (this->column - 1 == cell->column) {
            this->westEdge = false;
            cell->eastEdge = false;
        } else if (this->column + 1 == cell->column) {
            this->eastEdge = false;
            cell->westEdge = false;
        } else {
            return;
        }
    } else if (this->column == cell->column) {
        if (this->row - 1 == cell->row) {
            this->northEdge = false;
            cell->southEdge = false;
        } else if (this->row + 1 == cell->row) {
            this->southEdge = false;
            cell->northEdge = false;
        } else {
            return;
        }
    } else {
        return;
    }
}

bool Cell::isConnectedTo(Cell *cell) {
    if (this->row == cell->row) {
        if (this->column - 1 == cell->column) {
            if (!this->westEdge && !cell->eastEdge) {
                return true;
            }
        }
        if (this->column + 1 == cell->column) {
            if (!this->eastEdge && !cell->westEdge) {
                return true;
            }
        }
    }
    if (this->column == cell->column) {
        if (this->row - 1 == cell->row) {
            if (!this->northEdge && !cell->southEdge) {
                return true;
            }
        }
        if (this->row + 1 == cell->row) {
            if (!this->southEdge && !cell->northEdge) {
                return true;
            }
        }
    }
    return false;
}

int Cell::equals(Cell *cell) {
    return this->row == cell->row && this->column == cell->column;
}

int Cell::getWalls() {
    return this->northEdge + this->westEdge + this->southEdge + this->eastEdge;
}
