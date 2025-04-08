#include "raylib.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>


#define BASE_WIDTH 1280
#define BASE_HEIGHT 720
#define PLAYER_SPEED 200.0f

bool coletaveisinicializados;
Object obstaculos[NUM_OBSTACULOS_REC];
Circle cadeiras[4]; //tive que declarar fora de player_main 
Polygon poligonos[2];
Coletavel coletaveis[NUM_COLETAVEIS];


int main(void)
{   
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(BASE_WIDTH, BASE_HEIGHT, "Lotus Lounge");  
    SetAudioStreamBufferSizeDefault(1024);
    InitAudioDevice();
    SetTargetFPS(60);
    
    Music Music_Lobby = LoadMusicStream(".\\assets\\OCD\\Twilight_Zone.ogg");
    PlayMusicStream(Music_Lobby);

    // Variáveis para controle de estado
    bool isFullscreen = false;
    bool showSettings = false;
    bool showControls = false; // Mostrar tela de controles
    bool showCredit = false;
    int selectedResolution = 1; // Índice da resolução atual (1920x1080)
    bool init = 0;

    Texture2D spriteSheet = LoadTexture("assets//imagens//player.png");
    Texture2D bulletTexture = LoadTexture("assets//imagens//bullet.png");
    Texture2D phaseOneBG = LoadTexture("assets//imagens//lotus_lounge_bar.png");
    Texture2D bottleTexture = LoadTexture("assets//imagens//bottle.png");
    Player player = InitPlayer(spriteSheet, (Vector2){BASE_WIDTH / 2, BASE_HEIGHT / 2}, PLAYER_SPEED);

    Rectangle spawnZones[5];
    spawnZones[0] = (Rectangle){50, 50, 100, 100};   // canto superior esquerdo
    spawnZones[1] = (Rectangle){700, 50, 100, 100};  // canto superior direito
    spawnZones[2] = (Rectangle){50, 500, 100, 100};  // canto inferior esquerdo
    spawnZones[3] = (Rectangle){700, 500, 100, 100}; // canto inferior direito
    spawnZones[4] = (Rectangle){ BASE_HEIGHT/2, BASE_WIDTH/2, 450, 350}; // centro


    while (!WindowShouldClose())
    {   
    

        UpdateMusicStream(Music_Lobby);
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
            SetMusicVolume(Music_Lobby, 1.0f);
            Player_main(BASE_WIDTH, BASE_HEIGHT, PLAYER_SPEED, &player, bulletTexture, phaseOneBG, bottleTexture);
            
        } else {
            SetMusicVolume(Music_Lobby, 0.5f);
            Menu(BASE_WIDTH, BASE_HEIGHT, &isFullscreen, &showSettings, &showControls, &showCredit, &init);
        }
    }
    UnloadMusicStream(Music_Lobby);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
