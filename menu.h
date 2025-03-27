#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef struct {
    int width;
    int height;
} Resolution;

void FPS_visor();
void Menu(int WIDTH, int HEIGHT, bool *isFullscreen, bool *showSettings, bool *showControls, int numResolutions, Resolution *resolutions, int *selectedResolution);

#include "menu.c"
#endif