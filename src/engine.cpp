#include "../include/engine.hpp"

SDL_Window *window;
SDL_Renderer *renderer;
grid *workingBoard;

int screenWidth, screenHeight, cellHorizontalSize = 20, cellVerticalSize = 20;
int mouseX = 0, mouseY = 0;
float verticalOffset = 0, horizontalOffset = 0, movementSpeed = 3;
uint64_t startTime, currentTime;
bool isRunning = true, isDarkmode = false, isAutoplay = false;

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

void init(int width, int height, bool fullscreen, bool darkmode, grid *board){
    int flags = (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        workingBoard = board;
        screenWidth = width;
        screenHeight = height;
        isDarkmode = darkmode;

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
    } else {
        SDL_Log("Failed to initialize. Error: %s", SDL_GetError());
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
                    cellVerticalSize = boundaryAdd(cellVerticalSize, 5, 150, 2);
                    cellHorizontalSize = boundaryAdd(cellHorizontalSize, 5, 150, 2);
                    break;
                }
                case SDLK_DOWN: {
                    cellVerticalSize = boundaryAdd(cellVerticalSize, -5, 150, 2);
                    cellHorizontalSize = boundaryAdd(cellHorizontalSize, -5, 150, 2);
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
                    isAutoplay && (startTime = SDL_GetPerformanceCounter());
                    break;
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT){
                if (mouseX > horizontalOffset && mouseX < horizontalOffset + (cellHorizontalSize * workingBoard->getWidth()) && mouseY > verticalOffset && mouseY < verticalOffset + (cellVerticalSize * workingBoard->getHeight())){
                    // Gets index of the cell that the mouse presses.
                    int posX = ((mouseX - horizontalOffset) - fmod((mouseX - horizontalOffset), cellHorizontalSize)) / cellHorizontalSize, posY = ((mouseY - verticalOffset) - (fmod(mouseY - verticalOffset, cellVerticalSize))) / cellVerticalSize;
                    (*workingBoard->getCells())[posY][posX] = !(*workingBoard->getCells())[posY][posX];
                }
            }
            break;
        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;
    }
    
    currentTime = SDL_GetPerformanceCounter();
    if (isAutoplay && (currentTime - startTime) / static_cast<float>(SDL_GetPerformanceFrequency()) >= 0.3){
        startTime = currentTime;
        workingBoard = workingBoard->moveForward();
    }
}

// Grid drawing.
void drawGrid(){
    // Set grid color to black. (white in darkmode)
    if (isDarkmode)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
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
    if (isDarkmode)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    std::vector<std::vector<bool>> boardCells = *workingBoard->getCells();
    for (long unsigned int i = 0; i < boardCells.size(); ++i){
        for (long unsigned int j = 0; j < boardCells[i].size(); ++j){
            if (boardCells[i][j]){

                SDL_Rect rect;

                rect.x = j * cellHorizontalSize + horizontalOffset;
                rect.y = i * cellVerticalSize + verticalOffset;
                rect.h = cellVerticalSize;
                rect.w = cellHorizontalSize;

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void render(){
    if (isDarkmode)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // Draws background color.
    SDL_RenderClear(renderer);

    drawGrid();
    drawCells();

    SDL_RenderPresent(renderer);
}

void clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
