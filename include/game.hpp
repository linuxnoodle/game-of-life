#pragma once
#include <vector>
#include <cstddef>

class grid {
    private:
        grid *nextState = NULL, *previousState = NULL;
        std::vector<std::vector<bool>> cells;
        int height = 256, width = 256;

    public:
        grid(int width, int height);
        grid();

        /**
         * Takes the current state of the board, and returns what the board would look like at its next step.
         * @return - Next board state.
         */
        std::vector<std::vector<bool>> getNextBoardState();
        
        grid* moveForward();
        grid* moveBackward();
        
        /**
         * Setter for cells.
         * @param newCells - New value for board.
         */
        void overrideGrid(std::vector<std::vector<bool>> newCells, int height, int width);
        void resetBoard();

        int getHeight() { return height; }
        int getWidth() { return width; }
        std::vector<std::vector<bool>> *getCells() { return &cells; }
};
