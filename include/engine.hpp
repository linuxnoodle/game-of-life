#pragma once
#include "../include/game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdlib>
#include <cstdint>

extern bool isRunning;

/**
 * Initializes SDL2.
 * @param x - Window x-offset
 * @param y - Window y-offset
 * @param fullscreen - Changes whether it is fullscreen or not. 
 * @param darkmode - Changes color scheme.
 * @param board - Board from which data is displayed.
 */
void init(int width, int height, bool fullscreen, bool darkmode, grid &board);

/**
 * Updates entities.
 * @param frameTime - Delay between frames, used to make movement framerate-independent.
 */
void update(int frameTime);
void render();
void clean();