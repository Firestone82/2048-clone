#ifndef GAME_LIB
    #define GAME_LIB

    #include "library.h"

    #define DEBUG_ENABLED 0

    // GAME
    #define DEF_COLS 4
    #define DEF_ROWS 4

    // OFFSET
    #define SCORE_HEIGHT 75
    #define TOP_OFFSET 25
    #define RIGHT_OFFSET 25
    #define BOTTOM_OFFSET 25
    #define LEFT_OFFSET 25

    typedef enum {
        RUNNING,
        WIN,
        LOSE
    } State;

    typedef struct {
        int x1;
        int y1;
        int x2;
        int y2;
    } GameField;

    #pragma pack (1) // Proste to funguje
    typedef struct {
        int number;
        int offsetX;
        int offsetY;
        short action;
        SDL_Rect rect;
        bool merged;
    } Cube;

    typedef struct {
        int score;
        Cube** cubes;
        State state;
    } Game;

    typedef struct {
        int width;
        int height;
        int cols;
        int colSpacer;
        int rows;
        int rowSpacer;
        int cubeSize;
        char* font;
        GameField field;
    } GameProperties;

    SDL_Window* initWindow(char* gameTitle, int gameWidth, int gameHeight);
    SDL_Renderer* initRenderer(SDL_Window* window);
    void initIcon(SDL_Window* window, char* path);
    GameProperties configGame(SDL_DisplayMode display, bool def, char* argv[]);
    Cube** initCubes(GameProperties properties);
    void clearCubes(Game game, GameProperties properties);
    void fillCubes(Game game, GameProperties properties, int count);

    int moveBoard(SDL_Renderer* renderer, Game game, GameProperties properties, int direction);

#endif