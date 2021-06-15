#include "../include/game.hpp"

grid::grid(int width, int height){
    this->height = height;
    this->width = width; 
    cells.resize(height);
    for (int i = 0; i < height; ++i)
        cells[i].resize(width);
}
        
grid::grid(){
    //this->height = 16384;
    //this->width = 16384;
    this->height = 16;
    this->width = 16;
    cells.resize(height);
    for (int i = 0; i < height; ++i)
        cells[i].resize(width);
}

/**
 * Takes the current state of the board, and returns what the board would look like at its next step.
 * @return Next board state.
 */
std::vector<std::vector<bool>> grid::getNextBoardState(){
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
            
            if (cells[i][j] && (neighborSum != 2 || neighborSum != 3)){
                nextState[i][j] = false;
            } else if (!cells[i][j] && neighborSum == 3){
                nextState[i][j] = true;
            } else {
                nextState[i][j] = cells[i][j];
            }
        }
    }

    return nextState;
}

/**
 * Directly overrides grid values.
 * @param newCells - New value for board.
 */
void grid::overrideGrid(std::vector<std::vector<bool>> newCells, int height, int width){
    this->cells = newCells;
    this->height = height;
    this->width = width;
}
