#include "menu.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void text_board(int *framesCounter, int *letterCount, int *velocidade){
    BeginDrawing();
    const char text[] = "Andre eh um corninho!";

    ClearBackground(RAYWHITE);
    Texture2D texture = LoadTexture("imagens\\text_board.png");

    // Pegando o tamanho da tela
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Definindo as áreas de origem e destino
    Rectangle sourceRec = { 0, 0, texture.width, texture.height }; // Toda a imagem
    Rectangle destRec = { 0, 0, screenHeight, screenHeight}; // Ocupa a tela inteira
    Vector2 origin = { 0, 0 };
    Font fonte = LoadFont(".//assets//fonts//Pixelate-Regular.ttf");
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
    (*framesCounter)++;
    if (*framesCounter % *velocidade == 0 && *letterCount < (int)strlen(text)) {
        (*letterCount)++;
    }
    DrawTextEx(fonte, TextSubtext(text, 0, *letterCount), (Vector2){screenWidth / 10, screenHeight / 2}, 40, 2, LIGHTGRAY);
    EndDrawing();
}
