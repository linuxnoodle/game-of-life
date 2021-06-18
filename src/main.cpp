#include "../include/game.hpp"
#include "../include/engine.hpp"

#define FPS_COUNTER 0 

int main(){
    Uint64 currentTime = SDL_GetPerformanceCounter(), previousTime = 0;
    float deltaTime = 0;

    // Initializes grid with default constructor.
    grid *board = new grid();
   
    // Testing code for displaying.
    /*std::vector<std::vector<bool>> newCells = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };

    board.overrideGrid(newCells, 3, 3);*/

    // Initializes SDL2.
    init(1280, 720, false, false, board);

    while (isRunning){
        previousTime = currentTime;
        currentTime = SDL_GetPerformanceCounter();

        deltaTime = static_cast<float>((currentTime - previousTime) * 1000) / SDL_GetPerformanceFrequency();

        update(deltaTime);
        render();
    
        #if FPS_COUNTER == 1
            Uint64 endTime = SDL_GetPerformanceCounter();
            SDL_Log("FPS: %f", static_cast<float>(SDL_GetPerformanceFrequency()) / (endTime - currentTime));
        #endif
    }

    clean();
    delete board;
}
