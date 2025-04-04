#include "menu.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>


Player InitPlayer(Texture2D texture, Vector2 position, float PLAYER_SPEED) {
    Player player = {position, PLAYER_SPEED, texture, 0, 0.0f, 0};
    return player;
}

void getPlayerPos(Player *player, Object *objects, int n_objects, float delta, float PLAYER_SPEED) {
    Vector2 movement = {0, 0};
    bool isMoving = false;

    if (IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) { movement.y -= PLAYER_SPEED * delta; player->direction = 1; isMoving = true; }
    if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) { movement.x -= PLAYER_SPEED * delta; player->direction = 2; isMoving = true; }
    if (IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)) { movement.y += PLAYER_SPEED * delta; player->direction = 0; isMoving = true; }
    if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) { movement.x += PLAYER_SPEED * delta; player->direction = 3; isMoving = true; }

    Rectangle PlayerRect = {player->position.x + movement.x, player->position.y + movement.y, 50, 50};
    bool hit = false;
    for (int i = 0; i < n_objects; ++i) {
        if (CheckCollisionRecs(objects[i].rect, PlayerRect)) {
            hit = true;
        }
    }

    if (!hit && isMoving) {
        double norm = 1;
        if(movement.x > 0 && movement.y > 0) norm = 1.41;
        player->position.x += movement.x/norm;
        player->position.y += movement.y/norm;
    }

    if (isMoving) {
        player->timer += delta;
        if (player->timer >= 0.1f) {
            player->frame = (player->frame + 1) % 4; // Alterna entre 4 frames
            player->timer = 0;
        }
    } else {
        player->frame = 0; // Mantém um único frame quando parado
    }
}

void DrawPlayer(Player player, float PLAYER_SPEED) {
    int frameWidth = player.texture.width / 4;   // Assumindo 4 colunas na spritesheet
    int frameHeight = player.texture.height / 4; // Assumindo 4 linhas na spritesheet
    Rectangle source = {frameWidth * player.frame, frameHeight * player.direction, frameWidth, frameHeight};

    float scale = 0.5; // Reduz tamanho do sprite para caber melhor no retângulo
    Rectangle dest = {
        player.position.x + 24,  // Centraliza o sprite dentro do retângulo (50x50)
        player.position.y + 24,
        frameWidth * scale,
        frameHeight * scale
    };

    Vector2 origin = {(frameWidth * scale) / 2, (frameHeight * scale*1.2) / 2.2}; // Ajusta a origem
    DrawTexturePro(player.texture, source, dest, origin, 0, WHITE);
}

void Player_main(int WIDTH, int HEIGHT, float PLAYER_SPEED, Player *player){
    Object objects[5] = {
        {{500, 500, 100, 100}, 1, RED},
        {{0, 0, WIDTH, 10}, 1, BLACK},  // Parede superior
        {{0, HEIGHT - 10, WIDTH, 10}, 1, BLACK},  // Parede inferior
        {{0, 0, 10, HEIGHT}, 1, BLACK},  // Parede esquerda
        {{WIDTH - 10, 0, 10, HEIGHT}, 1, BLACK}  // Parede direita
    };
    int num_objects = sizeof(objects) / sizeof(objects[0]);
    BeginDrawing();
    float delta = GetFrameTime();
    ClearBackground(WHITE);
    getPlayerPos(player, objects, num_objects, delta, PLAYER_SPEED);
    DrawPlayer(*player, PLAYER_SPEED);
    for (int i = 0; i < num_objects; ++i) {
        DrawRectangleV((Vector2){objects[i].rect.x, objects[i].rect.y}, 
                    (Vector2){objects[i].rect.width, objects[i].rect.height}, 
                    objects[i].color);
    }
    EndDrawing();
}