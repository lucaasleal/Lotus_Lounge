#include "menu.h"
#include "raylib.h"
#include <stdio.h>

void FPS_visor() {
    int fps = GetFPS();
    char fpsText[32];
    snprintf(fpsText, sizeof(fpsText), "FPS: %d", fps);
    DrawText(fpsText, 10, 10, 20, DARKGRAY);
}

void Menu(int WIDTH, int HEIGHT, bool *isFullscreen, bool *showSettings, bool *showControls, int numResolutions, Resolution *resolutions, int *selectedResolution) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float scaleX = (float)screenWidth / WIDTH;
    float scaleY = (float)screenHeight / HEIGHT;
    
    const char *message = "Tela de Menu";
    int fontSize = 40 * scaleY;
    int textWidth = MeasureText(message, fontSize);
    DrawText(message, (screenWidth - textWidth) / 2, screenHeight / 4, fontSize, DARKGRAY);
    
    float btnWidth = 220 * scaleX;
    float btnHeight = 50 * scaleY;
    Vector2 mousePos = GetMousePosition();

    Rectangle btnPlay = {screenWidth / 2 - btnWidth / 2, screenHeight / 3, btnWidth, btnHeight};
    Rectangle btnControls = {btnPlay.x, btnPlay.y + btnHeight + 20 * scaleY, btnWidth, btnHeight};
    Rectangle btnSettings = {btnControls.x, btnControls.y + btnHeight + 20 * scaleY, btnWidth, btnHeight};
    
    DrawRectangleRec(btnPlay, GRAY);
    DrawText("Jogar", btnPlay.x + btnWidth / 3, btnPlay.y + btnHeight / 4, fontSize / 2, WHITE);
    
    DrawRectangleRec(btnControls, GRAY);
    DrawText("Controles", btnControls.x + btnWidth / 4, btnControls.y + btnHeight / 4, fontSize / 2, WHITE);
    
    DrawRectangleRec(btnSettings, GRAY);
    DrawText("Configurações", btnSettings.x + btnWidth / 4, btnSettings.y + btnHeight / 4, fontSize / 2, WHITE);
    
    if (CheckCollisionPointRec(mousePos, btnPlay) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        DrawText("Iniciando o jogo...", screenWidth / 2 - 100 * scaleX, screenHeight / 2, 30 * scaleY, BLACK);
    }
    
    if (CheckCollisionPointRec(mousePos, btnControls) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *showControls = !(*showControls);
    }
    
    if (CheckCollisionPointRec(mousePos, btnSettings) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *showSettings = !(*showSettings);
    }
    
    if (*showSettings) {
        DrawRectangle(screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2, LIGHTGRAY);
        DrawText("Escolha a Resolução", screenWidth / 2 - 150 * scaleX, screenHeight / 4 + 20 * scaleY, 30 * scaleY, DARKGRAY);

        for (int i = 0; i < numResolutions; i++) {
            Rectangle btnResolution = {screenWidth / 2 - btnWidth / 2, screenHeight / 4 + 50 * scaleY + (i * (btnHeight + 10 * scaleY)), btnWidth, btnHeight};
            DrawRectangleRec(btnResolution, GRAY);
            char resolutionText[32];
            snprintf(resolutionText, sizeof(resolutionText), "%dx%d", resolutions[i].width, resolutions[i].height);
            DrawText(resolutionText, btnResolution.x + btnWidth / 4, btnResolution.y + btnHeight / 4, fontSize / 2, WHITE);

            if (CheckCollisionPointRec(mousePos, btnResolution) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *selectedResolution = i;
                if (*isFullscreen) ToggleFullscreen();
                SetWindowSize(resolutions[*selectedResolution].width, resolutions[*selectedResolution].height);
                if (*isFullscreen) ToggleFullscreen();
                *showSettings = false;
            }
        }
    }
    
    if (*showControls) {
        DrawRectangle(screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2, LIGHTGRAY);
        DrawText("Controles do Jogo", screenWidth / 2 - 100 * scaleX, screenHeight / 4 + 20 * scaleY, 30 * scaleY, DARKGRAY);
        DrawText("F - Tela Cheia", screenWidth / 4 + 20 * scaleX, screenHeight / 4 + 50 * scaleY, 20 * scaleY, BLACK);
        
        Rectangle btnBack = {screenWidth / 2 - btnWidth / 2, screenHeight / 4 + screenHeight / 2 - btnHeight - 20 * scaleY, btnWidth, btnHeight};
        DrawRectangleRec(btnBack, GRAY);
        DrawText("Voltar", btnBack.x + btnWidth / 3, btnBack.y + btnHeight / 4, fontSize / 2, WHITE);
        
        if (CheckCollisionPointRec(mousePos, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *showControls = false;
        }
    }
    
    EndDrawing();
}