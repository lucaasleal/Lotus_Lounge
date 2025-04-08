#include "raylib.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

#define BASE_WIDTH 1280
#define BASE_HEIGHT 720
#define PLAYER_SPEED 100.0f

int main(void)
{   
    InitWindow(BASE_WIDTH, BASE_HEIGHT, "Lotus Lounge");  
    SetAudioStreamBufferSizeDefault(1024);
    InitAudioDevice();
    InitPlayerSounds();  // Carrega os efeitos sonoros (tiro e recarregamento)
    SetTargetFPS(60);
    
    // === CARREGAMENTO DAS MÚSICAS ===
    Music musicMenu = LoadMusicStream("assets/OCD/musica_menu.ogg");
    Music musicFase1 = LoadMusicStream("assets/OCD/Twilight_Zone.ogg");
    Music musicFase2 = LoadMusicStream("assets/OCD/musica_fase_2.ogg");

    SetMusicVolume(musicMenu, 1.0f);
    SetMusicVolume(musicFase1, 1.0f);
    SetMusicVolume(musicFase2, 1.0f);
    
    PlayMusicStream(musicMenu);  // Começa no menu
    
    Sound shotSound = LoadSound("assets/OCD/shot.ogg");
    Sound reloadSound = LoadSound("assets/OCD/reload.ogg");

    // Variáveis para controle de estado
    bool isFullscreen = false;
    bool showSettings = false;
    bool showControls = false; // Mostrar tela de controles
    bool showCredit = false;
    bool init = false;
    int currentMusic = 0;       // 0 = menu, 1 = fase 1, 2 = fase 2

    Texture2D spriteSheet = LoadTexture("assets//imagens//player.png");
    Texture2D bulletTexture = LoadTexture("assets//imagens//bullet.png");
    Texture2D phaseOneBG = LoadTexture("assets//imagens//lotus_lounge_bar.png");
    Texture2D bottleTexture = LoadTexture("assets//imagens//bottle.png");
    Player player = InitPlayer(spriteSheet, (Vector2){BASE_WIDTH / 2, BASE_HEIGHT / 2}, PLAYER_SPEED);

    while (!WindowShouldClose())
    {   
        // Atualiza as músicas
        UpdateMusicStream(musicMenu);
        UpdateMusicStream(musicFase1);
        UpdateMusicStream(musicFase2);
        
        // Lógica de troca de música
        if (!init && currentMusic != 0) {
            StopMusicStream(musicFase1);
            StopMusicStream(musicFase2);
            if (!IsMusicStreamPlaying(musicMenu)) PlayMusicStream(musicMenu);
            currentMusic = 0;
        } 
        else if (init && level == 1 && currentMusic != 1) {
            StopMusicStream(musicMenu);
            StopMusicStream(musicFase2);
            if (!IsMusicStreamPlaying(musicFase1)) PlayMusicStream(musicFase1);
            currentMusic = 1;
        } 
        else if (init && level == 2 && currentMusic != 2) {
            StopMusicStream(musicMenu);
            StopMusicStream(musicFase1);
            if (!IsMusicStreamPlaying(musicFase2)) PlayMusicStream(musicFase2);
            currentMusic = 2;
        }
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
            SetMusicVolume(Music_Lobby, 0.5f);
            Player_main(BASE_WIDTH, BASE_HEIGHT, PLAYER_SPEED, &player, bulletTexture, phaseOneBG, bottleTexture, &init);
        } else {
            SetMusicVolume(Music_Lobby, 1.0f);
            Menu(BASE_WIDTH, BASE_HEIGHT, &isFullscreen, &showSettings, &showControls, &showCredit, &init);
        }
    }
    // Liberação dos recursos
    UnloadPlayerSounds();  // Libera os efeitos de som
    UnloadMusicStream(musicMenu);
    UnloadMusicStream(musicFase1);
    UnloadMusicStream(musicFase2);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
