#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define BASE_WIDTH 1280
#define BASE_HEIGHT 720

typedef struct {
    int width;
    int height;
} Resolution;

Resolution resolutions[] = {
    {854, 480},
    {1280, 720},
    {1920, 1080}
};
int numResolutions = sizeof(resolutions) / sizeof(resolutions[0]);

int main(void)
{
    InitWindow(BASE_WIDTH, BASE_HEIGHT, "Jogo Massa");
    SetTargetFPS(60);

    // Variáveis para controle de estado
    bool isFullscreen = false;
    bool showSettings = false;
    bool showControls = false; // Mostrar tela de controles
    int selectedResolution = 1; // Índice da resolução atual (1920x1080)

    while (!WindowShouldClose())
    {
        // Obtém a resolução atual
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Fator de escala (proporção entre a tela atual e a base)
        float scaleX = (float)screenWidth / BASE_WIDTH;
        float scaleY = (float)screenHeight / BASE_HEIGHT;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Texto do Menu (Centralizado)
        const char *message = "Tela de Menu";
        int fontSize = 40 * scaleY;  // Ajusta a fonte proporcionalmente
        int textWidth = MeasureText(message, fontSize);
        int textX = (screenWidth - textWidth) / 2;
        int textY = screenHeight / 4;
        DrawText(message, textX, textY, fontSize, DARKGRAY);

        // Botão "Jogar" (Redimensionado)
        float btnWidth = 220 * scaleX;
        float btnHeight = 50 * scaleY;
        Rectangle btnPlay = {screenWidth / 2 - btnWidth / 2, textY + 100 * scaleY, btnWidth, btnHeight};
        DrawRectangleRec(btnPlay, GRAY);
        DrawText("Jogar", btnPlay.x + btnWidth / 3, btnPlay.y + btnHeight / 4, fontSize / 2, WHITE);

        // Botão "Controles" (Redimensionado)
        Rectangle btnControls = {screenWidth / 2 - btnWidth / 2, btnPlay.y + btnHeight + 20 * scaleY, btnWidth, btnHeight};
        DrawRectangleRec(btnControls, GRAY);
        DrawText("Controles", btnControls.x + btnWidth / 4, btnControls.y + btnHeight / 4, fontSize / 2, WHITE);

        // Botão "Configurações" (Redimensionado)
        Rectangle btnSettings = {screenWidth / 2 - btnWidth / 2, btnControls.y + btnHeight + 20 * scaleY, btnWidth, btnHeight};
        DrawRectangleRec(btnSettings, GRAY);
        DrawText("Configurações", btnSettings.x + btnWidth / 4, btnSettings.y + btnHeight / 4, fontSize / 2, WHITE);

        // Captura da posição do mouse
        Vector2 mousePos = GetMousePosition();

        // Detecta clique no botão "Jogar"
        if (CheckCollisionPointRec(mousePos, btnPlay) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Aqui você pode iniciar o jogo
            DrawText("Iniciando o jogo...", screenWidth / 2 - 100 * scaleX, screenHeight / 2, 30 * scaleY, BLACK);
        }

        // Detecta clique no botão "Controles"
        if (CheckCollisionPointRec(mousePos, btnControls) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            showControls = !showControls; // Alterna a exibição da tela de controles
        }

        // Detecta clique no botão "Configurações"
        if (CheckCollisionPointRec(mousePos, btnSettings) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            showSettings = !showSettings;
        }

        // Submenu de configurações
        if (showSettings) {
            // Fundo do submenu
            DrawRectangle(screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2, LIGHTGRAY);

            // Texto do submenu
            const char *settingsMessage = "Escolha a Resolução";
            int settingsFontSize = 30 * scaleY;
            int settingsTextWidth = MeasureText(settingsMessage, settingsFontSize);
            int settingsTextX = (screenWidth - settingsTextWidth) / 2;
            int settingsTextY = screenHeight / 4 + 20 * scaleY;
            DrawText(settingsMessage, settingsTextX, settingsTextY, settingsFontSize, DARKGRAY);

            // Botões para cada resolução
            for (int i = 0; i < numResolutions; i++) {
                Rectangle btnResolution = {screenWidth / 2 - btnWidth / 2, settingsTextY + 50 * scaleY + (i * (btnHeight + 10 * scaleY)), btnWidth, btnHeight};
                DrawRectangleRec(btnResolution, GRAY);
                char resolutionText[32];
                snprintf(resolutionText, sizeof(resolutionText), "%dx%d", resolutions[i].width, resolutions[i].height);
                DrawText(resolutionText, btnResolution.x + btnWidth / 4, btnResolution.y + btnHeight / 4, fontSize / 2, WHITE);

                // Detecta clique nos botões de resolução
                if (CheckCollisionPointRec(mousePos, btnResolution) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedResolution = i;


                    // Sai da tela cheia temporariamente para redimensionar
                    if (isFullscreen) {
                        ToggleFullscreen();
                    }

                    // Redimensiona a janela para a nova resolução
                    SetWindowSize(resolutions[selectedResolution].width, resolutions[selectedResolution].height);

                    // Reativa a tela cheia, se estiver no modo de tela cheia
                    if (isFullscreen) {
                        ToggleFullscreen();
                    }

                    // Centraliza a janela após redimensionar (se não estiver em tela cheia)
                    if (!isFullscreen) {
                        int monitorWidth = GetMonitorWidth(GetCurrentMonitor());
                        int monitorHeight = GetMonitorHeight(GetCurrentMonitor());
                        int windowX = (monitorWidth - resolutions[selectedResolution].width) / 2;
                        int windowY = (monitorHeight - resolutions[selectedResolution].height) / 2;
                        SetWindowPosition(windowX, windowY);
                    }

                    showSettings = false;  // Fecha o submenu após selecionar uma resolução
                }
            }

            // Botão "Tela Cheia" (Dentro do submenu)
            Rectangle btnFull = {screenWidth / 2 - btnWidth / 2, settingsTextY + 50 * scaleY + (numResolutions * (btnHeight + 10 * scaleY)), btnWidth, btnHeight};
            DrawRectangleRec(btnFull, GRAY);
            DrawText("Tela Cheia", btnFull.x + btnWidth / 4, btnFull.y + btnHeight / 4, fontSize / 2, WHITE);

            // Botão "Tela Normal" (Dentro do submenu)
            Rectangle btnNormal = {screenWidth / 2 - btnWidth / 2, btnFull.y + btnHeight + 10 * scaleY, btnWidth, btnHeight};
            DrawRectangleRec(btnNormal, GRAY);
            DrawText("Tela Normal", btnNormal.x + btnWidth / 4, btnNormal.y + btnHeight / 4, fontSize / 2, WHITE);

            // Detecta clique no botão "Tela Cheia"
            if (CheckCollisionPointRec(mousePos, btnFull) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (!isFullscreen) {
                    ToggleFullscreen();
                    isFullscreen = true;
                }
            }

            // Detecta clique no botão "Tela Normal"
            if (CheckCollisionPointRec(mousePos, btnNormal) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (isFullscreen) {
                    ToggleFullscreen();  // Sai da tela cheia
                    isFullscreen = false;
                }
                SetWindowSize(resolutions[selectedResolution].width, resolutions[selectedResolution].height);

                // Centraliza a janela após redimensionar
                int monitorWidth = GetMonitorWidth(GetCurrentMonitor());
                int monitorHeight = GetMonitorHeight(GetCurrentMonitor());
                int windowX = (monitorWidth - resolutions[selectedResolution].width) / 2;
                int windowY = (monitorHeight - resolutions[selectedResolution].height) / 2;
                SetWindowPosition(windowX, windowY);
            }
        }

        // Tela de controles
        if (showControls) {
            // Fundo da tela de controles
            DrawRectangle(screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2, LIGHTGRAY);

            // Texto da tela de controles
            const char *controlsMessage = "Controles do Jogo";
            int controlsFontSize = 30 * scaleY;
            int controlsTextWidth = MeasureText(controlsMessage, controlsFontSize);
            int controlsTextX = (screenWidth - controlsTextWidth) / 2;
            int controlsTextY = screenHeight / 4 + 20 * scaleY;
            DrawText(controlsMessage, controlsTextX, controlsTextY, controlsFontSize, DARKGRAY);

            // Instruções de controle
            const char *instructions = "WASD - Movimentação\nEspaço - Pular\nMouse - Mirar\nBotão Esquerdo - Atirar";
            int instructionsFontSize = 20 * scaleY;
            int instructionsX = screenWidth / 4 + 20 * scaleX;
            int instructionsY = controlsTextY + 50 * scaleY;
            DrawText(instructions, instructionsX, instructionsY, instructionsFontSize, BLACK);

            // Botão "Voltar" (Dentro da tela de controles)
            Rectangle btnBack = {screenWidth / 2 - btnWidth / 2, screenHeight / 4 + screenHeight / 2 - btnHeight - 20 * scaleY, btnWidth, btnHeight};
            DrawRectangleRec(btnBack, GRAY);
            DrawText("Voltar", btnBack.x + btnWidth / 3, btnBack.y + btnHeight / 4, fontSize / 2, WHITE);

            // Detecta clique no botão "Voltar"
            if (CheckCollisionPointRec(mousePos, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showControls = false; // Fecha a tela de controles
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}