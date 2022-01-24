# 2048 Game by Pavel Mikula
---

## Requirements
 - SDL2
 - SDL2 TTF addon
 - SDL2 Image addon

## Building
1) Install SDL libs
2) Run make file or by command
   - ```make```
   - ```gcc *.c -o main -Wall -Wno-unused-variable -Wno-unused-parameter -ISDL2/include -LSDL2/lib -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf```

## Usage
 - Arguments
 	- W - Number of colls
 	    - default: 4, min: 2
 	- H - Number of rows
 	    - default: 4, min: 2
 - Start command
    - ```./main W H```
        - Example1 - ```./main```
        - Example2 - ```./main 8 8```
 - Binds
    - WASD, Arrows - Block movement
    - R - Game restart
    - T - Victory screen
    - Z - Game Over screen
    - ESC - Game quit
