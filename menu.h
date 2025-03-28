#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef struct {
    int width;
    int height;
} Resolution;

void FPS_visor();
void Menu(int WIDTH, int HEIGHT, bool *isFullscreen, bool *showSettings, bool *showControls, bool *showCredit, int numResolutions, Resolution *resolutions, int *selectedResolution, bool *init);
void text_board(int *framesCounter, int *letterCount, int *velocidade);

#include "menu.c"
#include "game.c"
#endif