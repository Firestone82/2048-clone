#ifndef UTILS_LIB
    #define UTILS_LIB

    #include "library.h"

    SDL_Texture* getTexture(SDL_Renderer* renderer, char* path);
    TTF_Font* getFont(char* path, int fontSize);

    void clearScreen(SDL_Renderer* renderer);

    void drawCubes(SDL_Renderer* renderer, Game game, GameProperties properties);
    void drawField(SDL_Renderer* renderer, Game game, GameProperties properties);
    void drawScore(SDL_Renderer* renderer, Game game, GameProperties properties);
    void drawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, SDL_Rect rect, char* text);
    void drawWin(SDL_Renderer* renderer, Game game, GameProperties properties);
    void drawLose(SDL_Renderer* renderer, Game game, GameProperties properties);

#endif