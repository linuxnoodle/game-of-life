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
   
    // Pretty shoddy implementation of multithreading, but this is the only approach that doesn't randomly segfault.
    #pragma omp parallel for 
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
    
    nextGrid->overrideGrid(nextState, nextGrid->activeCells, height, width);
    return nextGrid;
}

void grid::resetBoard(){
    std::vector<std::vector<bool>> newCells(height, std::vector<bool>(width, false));
    std::vector<activeCell> newActiveCells;
    this->overrideGrid(newCells, newActiveCells, height, width);
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
void grid::overrideGrid(std::vector<std::vector<bool>> newCells, std::vector<activeCell> newActiveCells, int height, int width){
    this->cells = newCells;
    this->activeCells = newActiveCells;
    this->height = height;
    this->width = width;
}
