#include "../include/engine.hpp"

#define FPS_COUNTER 0 

int main(){
    Uint64 currentTime = SDL_GetPerformanceCounter(), previousTime = 0;
    float deltaTime = 0;

    // Initializes SDL2.
    init(1280, 720, false, false);

    while (isRunning){
        previousTime = currentTime;
        currentTime = SDL_GetPerformanceCounter();

        deltaTime = static_cast<float>((currentTime - previousTime) * 1000) / SDL_GetPerformanceFrequency();

        update(deltaTime);
        render();
    
        #if FPS_COUNTER
            Uint64 endTime = SDL_GetPerformanceCounter();
            SDL_Log("FPS: %f", static_cast<float>(SDL_GetPerformanceFrequency()) / (endTime - currentTime));
        #endif
    }

    clean();
}
