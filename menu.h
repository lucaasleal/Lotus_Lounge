#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef struct {
    int width;
    int height;
} Resolution;

typedef struct Player {
    Vector2 position;
    float speed;
    Texture2D texture;
    int frame;
    float timer;
    int direction; // 0 = Baixo, 1 = Esquerda, 2 = Direita, 3 = Cima
} Player;

typedef struct Object {
    Rectangle rect;
    int blocking;
    Color color;
} Object;


void FPS_visor();
void Menu(int WIDTH, int HEIGHT, bool *isFullscreen, bool *showSettings, bool *showControls, bool *showCredit, int numResolutions, Resolution *resolutions, int *selectedResolution, bool *init);
void text_board(int *framesCounter, int *letterCount, int *velocidade);
Player InitPlayer(Texture2D texture, Vector2 position, float PLAYER_SPEED);
void Player_main(int WIDTH, int HEIGHT, float PLAYER_SPEED, Player *player);
void ShootBullet(Player *player);
void UpdateBullets(float delta);
void DrawBullets();


#include "menu.c"
#include "game.c"
#include "player.c"
#endif
