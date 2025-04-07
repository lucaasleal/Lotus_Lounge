#include "raylib.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#ifndef PI
#define PI 3.14
#endif

#define MAX_BULLETS 20
#define RELOAD_TIME 2.0f // em segundos
#define NUM_COLETAVEIS 5
#define NUM_OBSTACULOS_REC 10

int currentBullets = MAX_BULLETS;
bool isReloading = false;
float reloadTimer = 0.0f;
extern bool coletaveisinicializados;
extern Object obstaculos[NUM_OBSTACULOS_REC];
extern Circle cadeiras[4]; //declarei aqui pra ele poder usar da main
extern Polygon poligonos[2];
extern Coletavel coletaveis[NUM_COLETAVEIS];

Bullet bullets[MAX_BULLETS];

typedef struct zombie {
    Vector2 position;
    Rectangle hitbox;
    Texture2D texture;
    float speed;
    float life;
    int direction;
    bool alive;
} Zombie;

float GetDistance(Vector2 a, Vector2 b) {
    return sqrtf((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
}

double getPlayerRotation(Player *player);
// Estrutura do projétil

Player InitPlayer(Texture2D texture, Vector2 position, float PLAYER_SPEED) {
    Player player = {position, PLAYER_SPEED, texture, 0, 0.0f, 0, 100};
    return player;
}


// Função para atirar
void ShootBullet(Player *player) {
    if(currentBullets<=0 || isReloading) return;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = (Vector2){player->position.x+25, player->position.y+25}; // centro do boneco
            float angle = (getPlayerRotation(player)  - 90.0f)* (PI / 180.0f);
            bullets[i].velocity = (Vector2){cosf(angle) * 500, sinf(angle) * 500}; // velocidade
            bullets[i].active = true;
            currentBullets--;
            break;
        }
    }
}

// Atualiza os tiros
void UpdateBullets(float delta) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.x += bullets[i].velocity.x * delta;
            bullets[i].position.y += bullets[i].velocity.y * delta;

            // Se sair da tela, desativa
            if (bullets[i].position.x < 204 || bullets[i].position.x > 1074 ||
                bullets[i].position.y < 50 || bullets[i].position.y > 650) {
                bullets[i].active = false;
            }

        }
    }
}

// Desenha os tiros
void DrawBullets(Texture2D bulletTexture) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            float rotation = atan2f(bullets[i].velocity.y, bullets[i].velocity.x) * 180.0f / PI;
            // Tamanho da bala na tela
            float scale = 0.02f;
            Rectangle source = {0, 0, (float)bulletTexture.width, (float)bulletTexture.height};
            Rectangle dest = {
                bullets[i].position.x,
                bullets[i].position.y,
                bulletTexture.width * scale,
                bulletTexture.height * scale
            };
            Vector2 origin = {dest.width / 2, dest.height / 2};

            DrawTexturePro(bulletTexture, source, dest, origin, rotation, WHITE);
        }
    }
}


void getPlayerPos(Player *player, float delta, float PLAYER_SPEED, Object obstaculos[], Circle cadeiras[], Polygon poligonos[], Coletavel coletaveis[]) {

    for (int i = 0; i < NUM_COLETAVEIS; i++) {
        if (!coletaveis[i].coletado &&
            CheckCollisionRecs((Rectangle){player->position.x, player->position.y, 32, 32}, //32 é o player Size TODO
                                (Rectangle){coletaveis[i].posicao.x, coletaveis[i].posicao.y, 20, 20})) {
            coletaveis[i].coletado = true;
        }
    }
    Vector2 movement = {0, 0};
    bool isMoving = false;

    if (IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) { movement.y -= PLAYER_SPEED * delta; player->direction = 1; isMoving = true; }
    if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) { movement.x -= PLAYER_SPEED * delta; player->direction = 2; isMoving = true; }
    if (IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)) { movement.y += PLAYER_SPEED * delta; player->direction = 0; isMoving = true; }
    if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) { movement.x += PLAYER_SPEED * delta; player->direction = 3; isMoving = true; }

    Rectangle PlayerRect = {player->position.x + movement.x, player->position.y + movement.y, 50, 50};
    bool hit = false;

    for (int i = 0; i < NUM_OBSTACULOS_REC; i++) {
        if (CheckCollisionRecs(PlayerRect, obstaculos[i].rect)) {
            hit = true;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (CheckCollisionCircleRec(cadeiras[i].center,cadeiras[i].radius,PlayerRect)) {
            hit = true;
        }
    }

    for (int i = 0; i < 2; i++){
        if (CheckCollisionRecPoly(PlayerRect, poligonos[i])) {
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

double getPlayerRotation(Player *player) {
    Vector2 mousePos = GetMousePosition();
    return (atan2f((player->position.y - mousePos.y), (player->position.x - mousePos.x)) * 180/PI + 270);
}

void DrawPlayer(Player player, float PLAYER_SPEED) {
    int frameWidth = player.texture.width / 9;   // Assumindo 4 colunas na spritesheet
    int frameHeight = player.texture.height; // Assumindo 4 linhas na spritesheet
    Rectangle source = {frameWidth * player.frame, frameHeight * player.direction, frameWidth, frameHeight};

    float scale = 1.5; // Reduz tamanho do sprite para caber melhor no retângulo
    Rectangle dest = {
        player.position.x + 24,  // Centraliza o sprite dentro do retângulo (50x50)
        player.position.y + 24,
        frameWidth * scale,
        frameHeight * scale
    };

    double rotation = getPlayerRotation(&player);

    Vector2 origin = {(frameWidth * scale) / 2, (frameHeight * scale*1.2) / 2.2}; // Ajusta a origem
    DrawTexturePro(player.texture, source, dest, origin, rotation, WHITE);
}

void Player_main(int WIDTH, int HEIGHT, float PLAYER_SPEED, Player *player, Texture2D bulletTexture, Texture2D phaseOneBG, Texture2D bottleTexture, Rectangle spawnZones[]) {
    float delta = GetFrameTime();
    

    if (!coletaveisinicializados){ //booleano para garantir que ele gere os coletaveis uma vez 
    InicializarObstaculos(obstaculos);
    InicializarCadeiras(cadeiras);
    InicializarPoligonos(poligonos);
    InicializarColetaveis(coletaveis, obstaculos, cadeiras, poligonos);
    coletaveisinicializados = true;}
    // Atualiza posição do jogador
    getPlayerPos(player, delta, PLAYER_SPEED, obstaculos, cadeiras, poligonos, coletaveis);

    // Atira ao apertar espaço
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ShootBullet(player);
    }
    // Atualiza os tiros
    UpdateBullets(delta);

    //Lógica de Recarga
    if (((IsKeyPressed(KEY_R) && currentBullets < MAX_BULLETS) || 
       (currentBullets==0 && (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)))) && !isReloading) {
        isReloading = true;
        reloadTimer = 0.0f;
    }
    
    if (isReloading) {
        reloadTimer += delta;
        if (reloadTimer >= RELOAD_TIME) {
            currentBullets = MAX_BULLETS;
            isReloading = false;
            reloadTimer = 0.0f;
        }
    }

    // Desenho da cena
    BeginDrawing();
    ClearBackground(BLACK);
    Vector2 mouse = GetMousePosition();
    DrawTexture(phaseOneBG, 20, 0, WHITE);
    // Desenha os projéteis
    DrawBullets(bulletTexture);
    DrawPlayer(*player, PLAYER_SPEED);
    FPS_visor();
    DrawText(TextFormat("Mouse X: %.0f Y: %.0f", mouse.x, mouse.y), 300, 10, 20, WHITE);

    //Mostra Nº de Balas
    DrawText(TextFormat("Balas: %d/%d", currentBullets, MAX_BULLETS), WIDTH-160, HEIGHT-90, 20, WHITE);
    if (isReloading) {
        DrawText("Recarregando...", WIDTH-(WIDTH/8), HEIGHT-(HEIGHT/4.5), 20, RED);
    }

    for (int i = 0; i < NUM_COLETAVEIS; i++) {
        if (!coletaveis[i].coletado) {
            float scale = 0.07f;
            Rectangle source = {0, 0, (float)bottleTexture.width, (float)bottleTexture.height};
            Rectangle dest = {
                coletaveis[i].posicao.x,
                coletaveis[i].posicao.y,
                bottleTexture.width * scale,
                bottleTexture.height * scale
            };
            Vector2 origin = {dest.width / 2, dest.height / 2};
            DrawTexturePro(bottleTexture, source, dest, origin, 0, WHITE);
        }
    }
    EndDrawing();
}