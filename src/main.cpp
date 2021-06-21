#include "../include/engine.hpp"

int main(){
    Uint64 currentTime = SDL_GetPerformanceCounter(), previousTime = 0;
    float deltaTime = 0, frameRate = 0;

    // Initializes SDL2.
    init(1280, 720, false, false);

    while (isRunning){
        previousTime = currentTime;
        currentTime = SDL_GetPerformanceCounter();

        deltaTime = static_cast<float>((currentTime - previousTime) * 1000) / SDL_GetPerformanceFrequency();

        update(deltaTime);
        render(frameRate);
        frameRate = static_cast<float>(SDL_GetPerformanceFrequency()) / (SDL_GetPerformanceCounter() - currentTime);
    }

    clean();
}
