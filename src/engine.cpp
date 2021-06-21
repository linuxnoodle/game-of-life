#include "../include/engine.hpp"
#include <SDL2/SDL_render.h>

SDL_Window *window;
SDL_Renderer *renderer;
grid *workingBoard;

TTF_Font *Sans = NULL;

int screenWidth, screenHeight, cellHorizontalSize = 20, cellVerticalSize = 20;
int mouseX = 0, mouseY = 0, previousMouseX = 0, previousMouseY = 0;
float verticalOffset = 0, horizontalOffset = 0, movementSpeed = 3, frameRate = 0;
uint64_t autoplayStartTime, autoplayCurrentTime, frameRateDisplayStartTime;
bool isRunning = true, isDragging = false, isDarkmode = false, isAutoplay = false;

template <typename T>
T boundaryAdd(T a, T b, T maximumValue, T minimumValue){
    if (((b > 0) && (a > (maximumValue - b)))){
        return maximumValue;        
    } else if ((b < 0) && (a < (minimumValue - b))) {
        return minimumValue;
    } else {
        return a + b;
    }
}

void init(int width, int height, bool fullscreen, bool darkmode){
    int flags = (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() != -1){
        workingBoard = new grid();
        screenWidth = width;
        screenHeight = height;
        isDarkmode = darkmode;

        Sans = TTF_OpenFont("include/Sans.ttf", 50);

        if (Sans == NULL){
            SDL_Log("error: %s", TTF_GetError());
            std::exit(1);
        }

        verticalOffset = static_cast<float>((screenHeight - (workingBoard->getHeight() * cellVerticalSize)) * 0.5);
        horizontalOffset = static_cast<float>((screenWidth - (workingBoard->getWidth() * cellHorizontalSize)) * 0.5);

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
        autoplayStartTime = SDL_GetPerformanceCounter();
    } else {
        SDL_Log("Failed to initialize. Errors:\n %s\n %s", SDL_GetError(), TTF_GetError());
        std::exit(1);
    }
}

void update(float deltaTime){
    // Event handling. 
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym){
                case SDLK_ESCAPE: {
                    isRunning = false;
                    break;
                }
                case SDLK_w: {
                    verticalOffset += movementSpeed * deltaTime;
                    break;
                }
                case SDLK_s: {
                    verticalOffset -= movementSpeed * deltaTime;
                    break;
                }
                case SDLK_a: {
                    horizontalOffset += movementSpeed * deltaTime;
                    break;
                }
                case SDLK_d: {
                    horizontalOffset -= movementSpeed * deltaTime;
                    break;
                }
                case SDLK_UP: {
                    int newHeight = boundaryAdd(cellVerticalSize, 5, 150, 5), newWidth = boundaryAdd(cellHorizontalSize, 5, 150, 5);
                    if (newHeight != cellVerticalSize)
                        verticalOffset -= 2.5 * workingBoard->getHeight();
                    if (newWidth != cellHorizontalSize)
                        horizontalOffset -= 2.5 * workingBoard->getWidth();
                    cellHorizontalSize = newWidth;
                    cellVerticalSize = newHeight;
                    break;
                }
                case SDLK_DOWN: {
                    int newHeight = boundaryAdd(cellVerticalSize, -5, 150, 5), newWidth = boundaryAdd(cellHorizontalSize, -5, 150, 5);
                    if (newHeight != cellVerticalSize)
                        verticalOffset += 2.5 * workingBoard->getHeight();
                    if (newWidth != cellHorizontalSize)
                        horizontalOffset += 2.5 * workingBoard->getWidth();
                    cellHorizontalSize = newWidth;
                    cellVerticalSize = newHeight;
                    break;
                }
                case SDLK_RIGHT: {
                    workingBoard = workingBoard->moveForward(); 
                    break;
                }
                case SDLK_LEFT: {
                    workingBoard = workingBoard->moveBackward();
                    break;
                }
                case SDLK_BACKSPACE: {
                    workingBoard->resetBoard();
                    break;
                }
                case SDLK_SPACE: {
                    isAutoplay = !isAutoplay; 
                    isAutoplay && (autoplayStartTime = SDL_GetPerformanceCounter());
                    break;
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT){
                if (mouseX > horizontalOffset && mouseX < horizontalOffset + (cellHorizontalSize * workingBoard->getWidth()) && mouseY > verticalOffset && mouseY < verticalOffset + (cellVerticalSize * workingBoard->getHeight())){
                    // Gets index of the cell that the mouse presses.
                    int posX = ((mouseX - horizontalOffset) - fmod((mouseX - horizontalOffset), cellHorizontalSize)) / cellHorizontalSize, posY = ((mouseY - verticalOffset) - (fmod(mouseY - verticalOffset, cellVerticalSize))) / cellVerticalSize;
                    workingBoard->cells[posY][posX] = !workingBoard->cells[posY][posX];

                    if (workingBoard->cells[posY][posX]){
                        (workingBoard->activeCells.push_back(activeCell{posY, posX}));
                    } else {
                        for (unsigned long int i = 0; i < workingBoard->activeCells.size(); ++i){
                            if (workingBoard->activeCells[i].posX == posX && workingBoard->activeCells[i].posY == posY)
                                   workingBoard->activeCells.erase(workingBoard->activeCells.begin() + i);
                        }
                    }
                }
            } else if (event.button.button == SDL_BUTTON_RIGHT){
                isDragging = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_RIGHT)
                isDragging = false;
            break;
        case SDL_MOUSEMOTION:
            previousMouseX = mouseX;
            previousMouseY = mouseY;
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            if (isDragging){
                horizontalOffset += (mouseX - previousMouseX);
                verticalOffset += (mouseY - previousMouseY);
            }
            break;
    }
    
    autoplayCurrentTime = SDL_GetPerformanceCounter();
    if (isAutoplay && (autoplayCurrentTime - autoplayStartTime) / static_cast<float>(SDL_GetPerformanceFrequency()) >= 0.3){
        autoplayStartTime = autoplayCurrentTime;
        workingBoard = workingBoard->moveForward();
    }
}

// Grid drawing.
void drawGrid(){
    // Set grid color to black. (white in darkmode)
    if (isDarkmode){
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    // Horizontal lines.
    for (int i = 0; i <= workingBoard->getHeight() * cellHorizontalSize; i += cellHorizontalSize){
        SDL_RenderDrawLine(renderer, 0 + horizontalOffset, i + verticalOffset, (workingBoard->getWidth() * cellHorizontalSize) + horizontalOffset, i + verticalOffset);
    }

    // Vertical lines.
    for (int i = 0; i <= workingBoard->getWidth() * cellVerticalSize; i += cellVerticalSize){
        SDL_RenderDrawLine(renderer, i + horizontalOffset, 0 + verticalOffset, i + horizontalOffset, (workingBoard->getHeight() * cellVerticalSize) + verticalOffset);
    }
}

// Draws active cells.
void drawCells(){
    if (isDarkmode){
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    std::vector<activeCell> activeCells = workingBoard->activeCells;
    for (unsigned long int i = 0; i < activeCells.size(); ++i){ 
        SDL_Rect rect;

        rect.x = activeCells[i].posX * cellHorizontalSize + horizontalOffset;
        rect.y = activeCells[i].posY * cellVerticalSize + verticalOffset;
        rect.h = cellVerticalSize;
        rect.w = cellHorizontalSize;

        SDL_RenderFillRect(renderer, &rect);
    }
}

void render(float framerate){
    SDL_Color fontColor;
    if ((SDL_GetPerformanceCounter() - frameRateDisplayStartTime) / static_cast<float>(SDL_GetPerformanceFrequency()) >= 0.15){
        frameRate = framerate;
        frameRateDisplayStartTime = SDL_GetPerformanceCounter();
    }
    
    if (isDarkmode){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        fontColor = {255, 255, 255};
    } else {
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        fontColor = {0, 0, 0};
    }

    // Draws background color.
    SDL_RenderClear(renderer);
    
    drawGrid();
    drawCells();
    
    std::string text = std::to_string(frameRate);
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, text.c_str(), fontColor);
    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    
    SDL_Rect messageRect{10, 10, static_cast<int>(text.size() * 10), 30};
    
    if (isDarkmode){
        SDL_SetRenderDrawColor(renderer, 90, 90, 90, 100);
    } else {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 100);
    }

    SDL_RenderFillRect(renderer, &messageRect);
    SDL_RenderCopy(renderer, Message, NULL, &messageRect);

    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(Message);
}

void clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
