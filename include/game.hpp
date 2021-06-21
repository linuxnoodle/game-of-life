#pragma once
#include <vector>
#include <iostream>
#include <cstddef>

struct activeCell {
    int posY = 0, posX = 0;
};

class grid {
    private:
        grid *nextState = NULL, *previousState = NULL;
        int height = 1024, width = 1024;

    public:
        std::vector<std::vector<bool>> cells;
        std::vector<activeCell> activeCells;
        
        grid(int width, int height);
        grid();

        /**
         * Takes the current state of the board, and returns what the board would look like at its next step.
         * @return - Next board state.
         */
        grid* getNextBoardState();
        
        grid* moveForward();
        grid* moveBackward();
        
        /**
         * Setter for cells.
         * @param newCells - New value for board.
         */
        void overrideGrid(std::vector<std::vector<bool>> newCells, int height, int width);
        void resetBoard();
        
        // Getters
        int getHeight() { return height; }
        int getWidth() { return width; }
};
