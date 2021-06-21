#include "../include/game.hpp"

grid::grid(int width, int height){
    this->height = height;
    this->width = width; 
    cells.resize(height);
    for (int i = 0; i < height; ++i)
        cells[i].resize(width);
}

grid::grid(){
    cells.resize(height);
    for (int i = 0; i < height; ++i)
        cells[i].resize(width);
}

/**
 * Takes the current state of the board, and returns what the board would look like at its next step.
 * @return Next board state.
 */
grid* grid::getNextBoardState(){
    grid *nextGrid = new grid();
    std::vector<std::vector<bool>> nextState(height, std::vector<bool>(width, false)); 
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; j++){
            // Sums the values of the all living cells in order to determine whether the current cell will live or die (with the appropriate masks).
            int neighborSum 
                = ((i != 0 && j != 0) && cells[i - 1][j - 1]) // top-left
                + ((i != 0) && cells[i - 1][j]) // top-center
                + ((i != 0 && j != width - 1) && cells[i - 1][j + 1]) //top-right
                + ((j != 0) && cells[i][j - 1]) // left
                + ((j != width - 1) && cells[i][j + 1]) // right
                + ((i != height - 1 && j != 0) && cells[i + 1][j - 1]) // bottom-left
                + ((i != height - 1) && cells[i + 1][j]) // bottom-center
                + ((i != height - 1 && j != width - 1) && cells[i + 1][j + 1]); // bottom-right

            if (cells[i][j] && (neighborSum != 2 && neighborSum != 3)){
                nextState[i][j] = false;
            } else if (!cells[i][j] && neighborSum == 3){
                nextState[i][j] = true;
                nextGrid->activeCells.push_back(activeCell{i, j});
            } else {
                nextState[i][j] = cells[i][j];
                if (nextState[i][j])
                    nextGrid->activeCells.push_back(activeCell{i, j});
            }
        }
    }

    nextGrid->overrideGrid(nextState, height, width);
    return nextGrid;
}

void grid::resetBoard(){
    std::vector<std::vector<bool>> newCells(height, std::vector<bool>(width, false));
    this->overrideGrid(newCells, height, width);
    this->nextState = NULL;
    this->previousState = NULL;
}

grid* grid::moveForward(){
    grid *nextGrid;
    nextGrid = getNextBoardState();

    this->nextState = nextGrid;
    nextGrid->previousState = this;
    
    return nextGrid;
}

grid* grid::moveBackward(){
    if (this->previousState != NULL){
        return this->previousState;
    } else {
        return this;
    }
}

/**
 * Directly overrides grid values.
 * @param newCells - New value for board.
 */
void grid::overrideGrid(std::vector<std::vector<bool>> newCells, int height, int width){
    activeCells.clear();
    this->cells = newCells;
    // Hopefully find some way to speed this up?
    for (unsigned long int i = 0; i < cells.size(); ++i){
        for (unsigned long int j = 0; j < cells[i].size(); ++j){
            if (cells[i][j])
                activeCells.push_back(activeCell{static_cast<int>(i), static_cast<int>(j)});
        }
    }
    this->height = height;
    this->width = width;
}
