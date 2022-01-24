#include "library.h"

int main(int argc, char* argv[]) {
    // SDL Library inicialization
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error intializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // SDL-TTF Library incialization
    if (TTF_Init() != 0) {
        printf("Error intializing TTF: %s\n", TTF_GetError());
        return 1;
    }

    srand(time(0));

    // Display properties
    SDL_DisplayMode display;
    SDL_GetCurrentDisplayMode(0, &display);

    // Game properties
    GameProperties properties = configGame(display, argc <= 1, argv);

    // Game inicialization
    SDL_Window* window = initWindow("2048 Game - Pavel Mikula", properties.width, properties.height);
    SDL_Renderer* renderer = initRenderer(window);
    initIcon(window, "images/icon2.png");

    Game game;
    game.cubes = initCubes(properties);
    game.score = 0;
    game.state = RUNNING;

    fillCubes(game, properties, 2);

    SDL_Event e;
    bool moved = true;
    bool merged = false;
    bool run = true;
    while (run) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: {
                    run = false;
                    break;
                }

                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym) {
                        case SDLK_LEFT:
                        case SDLK_a: {
                            if (game.state != RUNNING) break;

                            moved = true;
                            if (moveBoard(renderer, game, properties, 4)) {
                                merged = true;
                            }

                            break;
                        }

                        case SDLK_RIGHT:
                        case SDLK_d: {
                            if (game.state != RUNNING) break;

                            moved = true;
                            if (moveBoard(renderer, game, properties, 2)) {
                                merged = true;
                            }

                            break;
                        }

                        case SDLK_UP:
                        case SDLK_w: {
                            if (game.state != RUNNING) break;

                            moved = true;
                            if (moveBoard(renderer, game, properties, 1)) {
                                merged = true;
                            }

                            break;
                        }

                        case SDLK_DOWN:
                        case SDLK_s: {
                            if (game.state != RUNNING) break;

                            moved = true;
                            if (moveBoard(renderer, game, properties, 3)) {
                                merged = true;
                            }

                            break;
                        }

                        case SDLK_r: {
                            game.state = RUNNING;
                            game.score = 0;
                            clearCubes(game, properties);
                            fillCubes(game, properties, 2);
                            break;
                        }

                        case SDLK_t: {
                            game.state = WIN;
                            break;
                        }

                        case SDLK_z: {
                            game.state = LOSE;
                            break;
                        }

                        case SDLK_ESCAPE: {
                            run = false;
                            break;
                        }
                    }

                    break;
                }

                default: {
                    break;
                }
            }
        }

        if (moved) {
            moved = false;

            game.state = testCubes(game, properties);

            if (game.state == RUNNING && merged) {
                merged = false;

                fillCubes(game, properties, 1);
            }
        }

        /* Clear screen */
        clearScreen(renderer);

        /* Draw game */
        drawField(renderer, game, properties);
        drawCubes(renderer, game, properties);
        
        if (game.state == WIN) {
            drawWin(renderer, game, properties);
        } else if (game.state == LOSE) {
            drawLose(renderer, game, properties);
        }
        
        drawScore(renderer, game, properties);

        /* Render game */
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/FPS);
    }
    
    // Free resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    free(game.cubes);

    return 0;
}