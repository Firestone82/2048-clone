#include "library.h"

SDL_Window* initWindow(char* gameTitle, int gameWidth, int gameHeight) {
    return SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameWidth, gameHeight, SDL_WINDOW_SHOWN);
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    return SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void initIcon(SDL_Window* window, char* path) {
    SDL_Surface* iconSurface = IMG_Load(path);
    SDL_SetWindowIcon(window, iconSurface);
    SDL_FreeSurface(iconSurface);    
}

GameProperties configGame(SDL_DisplayMode display, bool def, char* argv[]) {
    GameProperties properties;

    properties.cols = def ? DEF_COLS : atoi(argv[1]);
    properties.rows = def ? DEF_ROWS : atoi(argv[2]);
    properties.width = display.w / 2;
    properties.height = display.h - (display.h * 2/5);
    properties.colSpacer = 10;
    properties.rowSpacer = 10;
    properties.font = "fonts/ClearSans-Bold.ttf";

    properties.cubeSize = (properties.height - (properties.rows * properties.rowSpacer)) / properties.rows;

    properties.width = LEFT_OFFSET + RIGHT_OFFSET + (properties.cols * properties.cubeSize) + ((properties.cols - 1) * properties.colSpacer);
    properties.height = TOP_OFFSET + BOTTOM_OFFSET + (properties.rows * properties.cubeSize) + ((properties.rows - 1) * properties.rowSpacer) + SCORE_HEIGHT;

    properties.field.x1 = LEFT_OFFSET;
    properties.field.y1 = TOP_OFFSET + SCORE_HEIGHT;
    properties.field.x2 = LEFT_OFFSET + ((properties.cols - 1) * properties.cubeSize) + ((properties.cols - 1)  * properties.colSpacer);
    properties.field.y2 = TOP_OFFSET + ((properties.rows - 1) * properties.cubeSize) + ((properties.rows - 1)  * properties.rowSpacer) + SCORE_HEIGHT;

    return properties;
}

Cube** initCubes(GameProperties properties) {
    Cube** cubes = (Cube**) malloc(properties.cols * sizeof(Cube*));

    for (int x = 0; x < properties.cols; x++) {
        cubes[x] = (Cube*) malloc(properties.rows * sizeof(Cube));
    }

    for (int x = 0; x < properties.cols; x++) {
        for (int y = 0; y < properties.rows; y++) {
            cubes[x][y].number = 0;
            cubes[x][y].offsetX = 0;
            cubes[x][y].offsetY = 0;
            cubes[x][y].merged = false;
            cubes[x][y].rect.x = LEFT_OFFSET + (x * properties.cubeSize) + (x * properties.colSpacer); 
            cubes[x][y].rect.y = TOP_OFFSET + (y * properties.cubeSize) + (y * properties.rowSpacer) + SCORE_HEIGHT;
            cubes[x][y].rect.w = properties.cubeSize;
            cubes[x][y].rect.h = properties.cubeSize;
        }
    }

    return cubes;
}

void clearCubes(Game game, GameProperties properties) {
    for (int x = 0; x < properties.cols; x++) {
        for (int y = 0; y < properties.rows; y++) {
            game.cubes[x][y].number = 0;
        }
    }
}

void fillCubes(Game game, GameProperties properties, int count) {
    if (DEBUG_ENABLED) {
        int i = 2;

        for (int x = 0; x < properties.cols; x++) {
            for (int y = 0; y < properties.rows; y++) {
                game.cubes[x][y].number = i;
                i *= 2;

                if (i > 2048) {
                    i = 0;
                }
            }
        }
    } else {
        int x = 0;

        while (x < count) {
            int dx = rand() % properties.cols;
            int dy = rand() % properties.rows;

            if (game.cubes[dx][dy].number == 0) {
                game.cubes[dx][dy].number = 2 * ((rand() % 2) + 1);        
                x++;
            }
        }
    }   
}

State testCubes(Game game, GameProperties properties) {
    for (int x = 0; x < properties.cols; x++) {
        for (int y = 0; y < properties.rows; y++) {
            if (game.cubes[x][y].number == 2048) {
                return WIN;
            } else if (game.cubes[x][y].number == 0) {
                return RUNNING;
            }
        }
    }

    return LOSE;
}

int moveBoard(SDL_Renderer* renderer, Game game, GameProperties properties, int direction) {
    int score = 0;

    switch (direction) {
        case 1: { // UP
            for (int x = 0; x < properties.cols; x++) {
                for (int y = 1; y <= properties.rows - 1; y++) {
                    if (y == 0) continue;
                    if (game.cubes[x][y].number == 0) continue;

                    if (game.cubes[x][y-1].number == 0) {
                        game.cubes[x][y-1].number = game.cubes[x][y].number;
                        game.cubes[x][y].number = 0;
                        y -= 2;
                        continue;
                    }

                    if (game.cubes[x][y].number == game.cubes[x][y-1].number && game.cubes[x][y-1].merged == false) {
                        game.cubes[x][y].number = 0;
                        game.cubes[x][y-1].number *= 2;
                        game.cubes[x][y-1].merged = true;
                        score += game.cubes[x][y-1].number;
                    }
                }
            }

            break;
        }

        case 2: { // RIGHT
            for (int y = 0; y < properties.cols; y++) {
                for (int x = properties.rows - 2; x >= 0; x--) {
                    if (x == properties.cols - 1) continue;
                    if (game.cubes[x][y].number == 0) continue;

                    if (game.cubes[x+1][y].number == 0) {
                        game.cubes[x+1][y].number = game.cubes[x][y].number;
                        game.cubes[x][y].number = 0;
                        x += 2;
                        continue;
                    }

                    if (game.cubes[x][y].number == game.cubes[x+1][y].number && game.cubes[x+1][y].merged == false) {
                        game.cubes[x][y].number = 0;
                        game.cubes[x+1][y].number *= 2;
                        game.cubes[x+1][y].merged = true;
                        score += game.cubes[x+1][y].number;
                    }
                }
            }

            break;
        }

        case 3: { // DOWN
            for (int x = 0; x < properties.cols; x++) {
                for (int y = properties.rows - 2; y >= 0; y--) {
                    if (y == properties.rows - 1) continue;
                    if (game.cubes[x][y].number == 0) continue;

                    if (game.cubes[x][y+1].number == 0) {
                        game.cubes[x][y+1].number = game.cubes[x][y].number;
                        game.cubes[x][y].number = 0;
                        y += 2;
                        continue;
                    }

                    if (game.cubes[x][y].number == game.cubes[x][y+1].number && game.cubes[x][y+1].merged == false) {
                        game.cubes[x][y].number = 0;
                        game.cubes[x][y+1].number *= 2;
                        game.cubes[x][y+1].merged = true;
                        score += game.cubes[x][y+1].number;
                    }
                }
            }

            break;
        }

        case 4: { // LEFT
            for (int y = 0; y < properties.rows; y++) {
                for (int x = 1; x <= properties.cols - 1; x++) {
                    if (x == 0) continue;
                    if (game.cubes[x][y].number == 0) continue;

                    if (game.cubes[x-1][y].number == 0) {
                        game.cubes[x-1][y].number = game.cubes[x][y].number;
                        game.cubes[x][y].number = 0;
                        x -= 2;
                        continue;
                    }

                    if (game.cubes[x][y].number == game.cubes[x-1][y].number && game.cubes[x-1][y].merged == false) {
                        game.cubes[x][y].number = 0;
                        game.cubes[x-1][y].number *= 2;
                        game.cubes[x-1][y].merged = true;
                        score += game.cubes[x-1][y].number;
                    }
                }
            }

            break;
        }
        
        default: {
            break;
        }
    }

    for (int x = 0; x < properties.cols; x++) {
        for (int y = 0; y < properties.rows; y++) {
            game.cubes[x][y].merged = false;
        }
    }
    
    return score;
}