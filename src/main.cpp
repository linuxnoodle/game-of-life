#include "../include/game.hpp"
#include "../include/engine.hpp"
#include "../include/imageprocessing.hpp"
#include <SDL2/SDL_timer.h>

#define FPS_COUNTER 1

int main(){
    Uint32 frameTimeStart, frameTime = 0;

    // Initializes grid with default constructor.
    grid board;

    // Initializes SDL2.
    init(1280, 720, false, true, board);

    while (isRunning){
        frameTimeStart = SDL_GetTicks();
        
        #ifdef FPS_COUNTER
            Uint64 start = SDL_GetPerformanceCounter();
        #endif

        update(frameTime);
        render();
    
        #ifdef FPS_COUNTER
            Uint64 end = SDL_GetPerformanceCounter();
            SDL_Log("FPS: %f", static_cast<float>(SDL_GetPerformanceFrequency()) / (end - start));
        #endif

        frameTime = SDL_GetTicks() - frameTimeStart;
    }

    clean();
}
