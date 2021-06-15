#include "../include/engine.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>

SDL_Window *window;
SDL_Renderer *renderer;
grid workingBoard;

int screenWidth, screenHeight, cellHorizontalSize = 20, cellVerticalSize = 20;
float verticalOffset = 0, horizontalOffset = 0;
bool isRunning = true, isDarkmode = false;

template <typename T>
T boundaryAdd(T a, T b, T maximumValue, T minimumValue){
    if (((b > 0) && (a > (maximumValue - b)))){
        return maximumValue;        
    } else if ((b < 0) && (a < (minimumValue - b))){
        return minimumValue;
    } else {
        return a + b;
    }
}
void init(int width, int height, bool fullscreen, bool darkmode, grid &board){
    int flags = (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        workingBoard = board;
        screenWidth = width;
        screenHeight = height;
        isDarkmode = darkmode;

        verticalOffset = static_cast<float>((screenHeight - (workingBoard.getHeight() * cellVerticalSize)) * 0.5);
        horizontalOffset = static_cast<float>((screenWidth - (workingBoard.getWidth() * cellHorizontalSize)) * 0.5);

        window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer){
            if (isDarkmode)
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_Log("Failed to initialize. Error: %s", SDL_GetError());
            std::exit(1);
        }
    } else {
        SDL_Log("Failed to initialize. Error: %s", SDL_GetError());
        std::exit(1);
    }
}

void update(int frameTime){
    // Keyboard handling. 
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
            case SDLK_ESCAPE:
                isRunning = false;
                break;
            case SDLK_w:
            case SDLK_UP:
                verticalOffset += 5 * frameTime;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                verticalOffset -= 5 * frameTime;
                break;
            case SDLK_a:
            case SDLK_LEFT:
                horizontalOffset += 5 * frameTime;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                horizontalOffset -= 5 * frameTime;
                break;
            case SDLK_EQUALS:
            case SDLK_PLUS:
                cellVerticalSize = boundaryAdd(cellVerticalSize, 5, 100, 1);
                cellHorizontalSize = boundaryAdd(cellHorizontalSize, 5, 100, 1);
                break;
            case SDLK_MINUS:
            case SDLK_UNDERSCORE:
                cellVerticalSize = boundaryAdd(cellVerticalSize, -5, 100, 1);
                cellHorizontalSize = boundaryAdd(cellHorizontalSize, -5, 100, 1);
                break;
        }
    }
}

void render(){
    if (isDarkmode)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
    SDL_RenderClear(renderer);

    // Set grid color to black. (white in darkmode)
    if (isDarkmode)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    // Horizontal lines.
    for (int i = 0; i <= workingBoard.getHeight() * cellHorizontalSize; i += cellHorizontalSize){
        SDL_RenderDrawLine(renderer, 0 + horizontalOffset, i + verticalOffset, (workingBoard.getWidth() * cellHorizontalSize) + horizontalOffset, i + verticalOffset);
    }

    // Vertical lines.
    for (int i = 0; i <= workingBoard.getWidth() * cellVerticalSize; i += cellVerticalSize){
        SDL_RenderDrawLine(renderer, i + horizontalOffset, 0 + verticalOffset, i + horizontalOffset, (workingBoard.getHeight() * cellVerticalSize) + verticalOffset);
    }

    SDL_RenderPresent(renderer);
}

void clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
