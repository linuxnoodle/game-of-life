#pragma once
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

class grid {
    private:
        std::vector<std::vector<bool>> cells;
        //int height = 16384, width = 16384;
        int height = 16, width = 16;

    public:
        grid(int width, int height);
        grid();

        /**
         * Takes the current state of the board, and returns what the board would look like at its next step.
         * @return - Next board state.
         */
        std::vector<std::vector<bool>> getNextBoardState();
        
        /**
         * Setter for cells.
         * @param newCells - New value for board.
         */
        void overrideGrid(std::vector<std::vector<bool>> newCells, int height, int width);

        int getHeight() { return height; }
        int getWidth() { return width; }
};
