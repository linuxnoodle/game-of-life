# Conway's Game of Life
Just implementing Conway's game of life in SDL2.
# Simplified rules:
- If a living cell doesn't have 2 or three neighbors, it dies.
- If a dead cell has 3 neighbors, it becomes alive.
# Controls
- `w` - Shifts camera up.
- `a` - Shifts camera to the left.
- `s` - Shifts camera down.
- `d` - Shifts camera to the right.
- `↓` - Zooms out.
- `↑` - Zooms in.
- `→` - Moves state forward. (generates next state) 
- `←` - Moves state backward. (up to the initial state)
- `Backspace` - Resets board.
- `Spacebar` - Starts autoplaying. (3 frames per second) 
# Building
```
git clone https://github.com/linuxnoodle/game-of-life
cd game-of-life/
make
```
# TODO:
- [X] Make backend for computing the next state of the grid.
- [X] Draw a grid for holding the cells.
- [X] Add movement and scaling to the grid.
- [X] Allow the user to change the state of cells.
- [X] Allow the user to traverse to the next state, and to the previous states. (probably will finally use a doubly-linked list)
- [X] Autoplay.
- [X] Darkmode.
- [ ] Zoom in on the center instead of on the top-left corner.
- [ ] Further optimizations for runtime. (possibly using an opengl texture for the grid?)

