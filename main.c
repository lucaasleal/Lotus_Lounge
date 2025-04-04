#include "raylib.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

#define BASE_WIDTH 1280
#define BASE_HEIGHT 720
#define PLAYER_SPEED 200.0f

Resolution resolutions[] = {
    {1280, 720},
    {1920, 1080}
};

int numResolutions = sizeof(resolutions) / sizeof(resolutions[0]);

int main(void)
{   
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(BASE_WIDTH, BASE_HEIGHT, "Jogo Massa");
    InitAudioDevice();
    SetTargetFPS(60);
    
    //Sound Music_Lobby = LoadSound(".\\assets\\OCD\\Twilight_Zone.mp3");
    //PlaySound(Music_Lobby);

    // Variáveis para controle de estado
    bool isFullscreen = false;
    bool showSettings = false;
    bool showControls = false; // Mostrar tela de controles
    bool showCredit = false;
    int selectedResolution = 1; // Índice da resolução atual (1920x1080)
    bool init = 0;
    int letterCount = 0;
    int framesCounter = 0;
    int velocidade = 4;

    Texture2D spriteSheet = LoadTexture("C://Users//lucas//Downloads//boneco.png");
    Player player = InitPlayer(spriteSheet, (Vector2){BASE_WIDTH / 16, BASE_HEIGHT / 12}, PLAYER_SPEED);

    while (!WindowShouldClose())
    {   
        if (!player.texture.id) {
            printf("Erro: Player não inicializado corretamente!\n");
            CloseAudioDevice();
            CloseWindow();
            exit(1);
        }
        FPS_visor();
        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
        }
        if (init){
            Player_main(BASE_WIDTH, BASE_HEIGHT, PLAYER_SPEED, &player);
        } else {
            Menu(BASE_WIDTH, BASE_HEIGHT, &isFullscreen, &showSettings, &showControls, &showCredit, numResolutions, resolutions, &selectedResolution, &init);
        }
    }
    //UnloadSound(Music_Lobby);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}