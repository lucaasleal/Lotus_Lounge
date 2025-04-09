#include "game.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool text_board(int *framesCounter, int *letterCount, int *velocidade, const char *text[], int numLines) {
    BeginDrawing();
    ClearBackground(BLACK);

    Texture2D texture = LoadTexture("assets/imagens/text_board.png");
    Font fonte = LoadFont("assets/fonts/Pixelate-Regular.ttf");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Rectangle sourceRec = { 0, 0, texture.width, texture.height };
    Rectangle destRec = { 0, 0, screenHeight, screenHeight };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);

    (*framesCounter)++;
    if (*framesCounter % *velocidade == 0) (*letterCount)++;

    int totalLetters = 0;
    for (int i = 0; i < numLines; i++) {
        totalLetters += strlen(text[i]);
    }

    int lettersDrawn = 0;
    int yOffset = 0;
    int fontSize = 30;
    int lineSpacing = 40;
    int startY = screenHeight / 4;

    for (int i = 0; i < numLines; i++) {
        int len = strlen(text[i]);
        if (*letterCount >= lettersDrawn + len) {
            DrawTextEx(fonte, text[i], (Vector2){screenWidth / 10, startY + yOffset}, fontSize, 2, LIGHTGRAY);
        } else {
            char buffer[1024] = {0};
            strncpy(buffer, text[i], *letterCount - lettersDrawn);
            DrawTextEx(fonte, buffer, (Vector2){screenWidth / 10, startY + yOffset}, fontSize, 2, LIGHTGRAY);
            break;
        }
        lettersDrawn += len;
        yOffset += lineSpacing;
    }

    EndDrawing();

    return *letterCount >= totalLetters + 10;  // +20 só pra garantir que dê tempo de ver o final
}
