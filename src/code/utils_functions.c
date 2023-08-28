#include "library.h"

SDL_Texture* getTexture(SDL_Renderer* renderer, char* path) {
    SDL_Surface* imageSurface = IMG_Load(path);
    SDL_Texture* image = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    return image;
}

TTF_Font* getFont(char* path, int fontSize) {
    return TTF_OpenFont(path, fontSize);
}

void clearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 250, 248, 239, 255);
    SDL_RenderClear(renderer);
}

void drawCubes(SDL_Renderer* renderer, Game game, GameProperties properties) {
    SDL_Texture* square = getTexture(renderer, "src/images/square.png");

    for (int x = 0; x < properties.cols; x++) {
        for (int y = 0; y < properties.rows; y++) {
            Cube cube = game.cubes[x][y];

            if (cube.number == 0) continue;

            char text[10];
            sprintf(text, "%d", cube.number);

            switch (cube.number) {
                case 2:     SDL_SetTextureColorMod(square, 238, 228, 218);  break;
                case 4:     SDL_SetTextureColorMod(square, 237, 224, 200);  break;
                case 8:     SDL_SetTextureColorMod(square, 242, 177, 121);  break;
                case 16:    SDL_SetTextureColorMod(square, 245, 149, 99);   break;
                case 32:    SDL_SetTextureColorMod(square, 246, 124, 95);   break;
                case 64:    SDL_SetTextureColorMod(square, 246, 94, 59);    break;
                case 128:   SDL_SetTextureColorMod(square, 237, 207, 114);  break;
                case 256:   SDL_SetTextureColorMod(square, 237, 204, 97);   break;
                case 512:   SDL_SetTextureColorMod(square, 237, 200, 80);   break;
                case 1024:  SDL_SetTextureColorMod(square, 237, 197, 63);   break;
                case 2048:  SDL_SetTextureColorMod(square, 237, 194, 46);   break;
                default:    SDL_SetTextureColorMod(square, 205, 193, 180);  break;
            }

            SDL_RenderCopy(renderer, square, NULL, &cube.rect);  

            if (cube.number != 0) {
                SDL_Color color;
                TTF_Font* font;

                if (cube.number == 2 || cube.number == 4) {
                    color.r = 119;
                    color.g = 110;
                    color.b = 101;
                } else {
                    color.r = 249;  
                    color.g = 242;
                    color.b = 246;
                }

                if (cube.number > 1000) {
                    font = getFont(properties.font, cube.rect.w * 1/3);
                } else if (cube.number > 100) {
                    font = getFont(properties.font, cube.rect.w * 1/2.5);
                } else if (cube.number > 10) {
                    font = getFont(properties.font, cube.rect.w * 1/2);
                } else {
                    font = getFont(properties.font, cube.rect.w * 1/2);
                }
                 
                int textWidth = 0;
                int textHeight = 0;
                TTF_SizeText(font, text, &textWidth, &textHeight);

                SDL_Rect rect = {
                    cube.rect.x + (cube.rect.w - textWidth) / 2.0,
                    cube.rect.y + ((cube.rect.h - textHeight) / 2.0),
                    textWidth,
                    textHeight
                };

                drawText(renderer, font, color, rect, text);
            }
        }
    }

    SDL_DestroyTexture(square);
}

void drawField(SDL_Renderer* renderer, Game game, GameProperties properties) {
    SDL_Texture* square = getTexture(renderer, "src/images/square.png");

    int cordX = properties.field.x1 - properties.colSpacer;
    int cordY = properties.field.y1 - properties.rowSpacer;
    int width = properties.field.x2 + properties.cubeSize - cordX + properties.colSpacer;
    int height = properties.field.y2 + properties.cubeSize - cordY + properties.rowSpacer;

    SDL_Rect fieldShadow = { cordX + 5, cordY + 5, width, height };
    SDL_SetRenderDrawColor(renderer, 140, 132, 125, 255);
    SDL_RenderFillRect(renderer, &fieldShadow);

    SDL_Rect field = { cordX, cordY, width, height };
    SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
    SDL_RenderFillRect(renderer, &field);

    SDL_Rect scoreFieldShadow = { TOP_OFFSET - properties.colSpacer + 5, LEFT_OFFSET - properties.rowSpacer + 5, width, SCORE_HEIGHT - BOTTOM_OFFSET};
    SDL_SetRenderDrawColor(renderer, 140, 132, 125, 255);
    SDL_RenderFillRect(renderer, &scoreFieldShadow);

    SDL_Rect scoreField = { TOP_OFFSET - properties.colSpacer, LEFT_OFFSET - properties.rowSpacer, width, SCORE_HEIGHT - BOTTOM_OFFSET};
    SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
    SDL_RenderFillRect(renderer, &scoreField);

    for (int x = 0; x < properties.cols; x++) {
        for (int y = 0; y < properties.rows; y++) {
            SDL_Rect rect = {
                LEFT_OFFSET + (x * properties.cubeSize) + (x * properties.colSpacer),
                TOP_OFFSET + (y * properties.cubeSize) + (y * properties.rowSpacer) + SCORE_HEIGHT,
                properties.cubeSize,
                properties.cubeSize
            };

            SDL_SetTextureColorMod(square, 205, 193, 180);
            SDL_RenderCopy(renderer, square, NULL, &rect);  
        }
    }

    SDL_DestroyTexture(square);
}

void drawScore(SDL_Renderer* renderer, Game game, GameProperties properties) {
    TTF_Font* font = getFont(properties.font, SCORE_HEIGHT * 2/3);

    char text[255];
    sprintf(text, "Score: %d", game.score);

    int textWidth = 0;
    int textHeight = 0;
    TTF_SizeText(font, text, &textWidth, &textHeight);

    SDL_Color color = {119, 110, 101, 0};
    SDL_Rect rect = { 
        (properties.width / 2) - (textWidth / 2), 
        TOP_OFFSET - properties.rowSpacer - ((textHeight - (SCORE_HEIGHT - BOTTOM_OFFSET)) / 2), 
        textWidth, 
        textHeight
    };

    drawText(renderer, font, color, rect, text);
}

void drawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, SDL_Rect rect, char* text) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, message, NULL, &rect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(message);
    TTF_CloseFont(font);
}

void drawWin(SDL_Renderer* renderer, Game game, GameProperties properties) {
    SDL_Texture* square = getTexture(renderer, "src/images/fill.png");
    SDL_Rect squareRect = { 0, 0, properties.width, properties.height };
    SDL_SetTextureAlphaMod(square, 175);
    SDL_RenderCopy(renderer, square, NULL, &squareRect);
    SDL_DestroyTexture(square);

    SDL_Color color = { 119, 110, 101, 255 };
    TTF_Font* font = NULL;
    SDL_Rect rect;
    int textWidth = 0;
    int textHeight = 0;

    font = getFont(properties.font, 75);
    TTF_SizeText(font, "YOU WIN", &textWidth, &textHeight);
    rect.x = (properties.width / 2) - (textWidth / 2);
    rect.y = (properties.height / 2) - textHeight;
    rect.w = textWidth; 
    rect.h = textHeight;

    drawText(renderer, font, color, rect, "YOU WIN");

    font = getFont(properties.font, 35);
    TTF_SizeText(font, "Press R to restart", &textWidth, &textHeight);
    rect.x = (properties.width / 2) - (textWidth / 2); 
    rect.y = (properties.height / 2) + textHeight;
    rect.w = textWidth;
    rect.h = textHeight;

    drawText(renderer, font, color, rect, "Press R to restart");
}

void drawLose(SDL_Renderer* renderer, Game game, GameProperties properties) {
    SDL_Texture* square = getTexture(renderer, "src/images/fill.png");
    SDL_Rect squareRect = { 0, 0, properties.width, properties.height };
    SDL_SetTextureAlphaMod(square, 175);
    SDL_RenderCopy(renderer, square, NULL, &squareRect);
    SDL_DestroyTexture(square);

    SDL_Color color = { 119, 110, 101, 0 };
    TTF_Font* font = NULL;
    SDL_Rect rect;
    int textWidth = 0;
    int textHeight = 0;

    font = getFont(properties.font, 75);
    TTF_SizeText(font, "GAME OVER", &textWidth, &textHeight);
    rect.x = (properties.width / 2) - (textWidth / 2);
    rect.y = (properties.height / 2) - textHeight;
    rect.w = textWidth; 
    rect.h = textHeight;

    drawText(renderer, font, color, rect, "GAME OVER");

    font = getFont(properties.font, 35);
    TTF_SizeText(font, "Press R to restart", &textWidth, &textHeight);
    rect.x = (properties.width / 2) - (textWidth / 2); 
    rect.y = (properties.height / 2) + textHeight;
    rect.w = textWidth;
    rect.h = textHeight;

    drawText(renderer, font, color, rect, "Press R to restart");
}
