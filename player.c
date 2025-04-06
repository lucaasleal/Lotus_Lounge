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
#define NUM_COLETAVEIS 10
#define NUM_OBSTACULOS_REC 10

int currentBullets = MAX_BULLETS;
bool isReloading = false;
float reloadTimer = 0.0f;

double getPlayerRotation(Player *player);

// Estrutura do projétil
typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool active;
} Bullet;
Bullet bullets[MAX_BULLETS];

Player InitPlayer(Texture2D texture, Vector2 position, float PLAYER_SPEED) {
    Player player = {position, PLAYER_SPEED, texture, 0, 0.0f, 0};
    return player;
}

void InicializarObstaculos(Object obstaculos[]) {
    // Definição de obstáculos (paredes, bar, mesa de sinuca, etc.)
    obstaculos[0] = (Object){{0, 0, 204, 720},1,RED};       // Parede esquerda
    obstaculos[1] = (Object){{1076, 0, 204, 720},1,RED};    // Parede direita
    obstaculos[2] = (Object){{0, 0, 1280, 47},1,RED};       // Parede superior
    obstaculos[3] = (Object){{0, 652, 1280, 47},1,RED};     // Parede inferior
    obstaculos[4] = (Object){{204, 47, 734, 221},1,RED};    // Mesa do bar
    obstaculos[5] = (Object){{952, 47, 114, 11}, 1, RED};   // Porta
    obstaculos[6] = (Object){{288, 641, 114, 11}, 1, RED};  // Janela 1
    obstaculos[7] = (Object){{490, 641, 114, 11}, 1, RED};  // Janela 2
    obstaculos[8] = (Object){{686, 641, 114, 11}, 1, RED};  // Janela 3
    obstaculos[9] = (Object){{872, 641, 114, 11}, 1, RED};  // Porta 2
}

void InicializarCadeiras(Circle cadeiras[]) {
    cadeiras[0] = (Circle){{365, 312},20};
    cadeiras[1] = (Circle){{428, 300},20};
    cadeiras[2] = (Circle){{630, 305},20};
    cadeiras[3] = (Circle){{700, 305},20};
}

void InicializarPoligonos(Polygon poligonos[]) {
    for (int i = 0; i < 2; i++){
        poligonos[i].points = malloc(4 * sizeof(Vector2));
        poligonos[i].pointCount = 4;
    }

    // Mesa do bar
    poligonos[0].points[0] = (Vector2){798, 384};
    poligonos[0].points[1] = (Vector2){992, 415};
    poligonos[0].points[2] = (Vector2){971, 536};
    poligonos[0].points[3] = (Vector2){778, 504};

    // Jukebox
    poligonos[1].points[0] = (Vector2){214, 299};
    poligonos[1].points[1] = (Vector2){291, 276};
    poligonos[1].points[2] = (Vector2){305, 327};
    poligonos[1].points[3] = (Vector2){227, 350};
}

int CheckCollisionRecPoly(Rectangle Rec, Polygon poly){
    Vector2 pontosRec[4] = {
        {Rec.x, Rec.y},
        {Rec.x + Rec.width, Rec.y},
        {Rec.x, Rec.y + Rec.height},
        {Rec.x + Rec.width, Rec.y + Rec.height}
    };

    for (int i = 0; i < 4; i++){
        if (CheckCollisionPointPoly(pontosRec[i], poly.points, poly.pointCount)){
            return 1;
        }
    }
    return 0;
}

void InicializarColetaveis(Coletavel coletaveis[], Object obstaculos[], Circle cadeiras[], Polygon poligonos[]) {
    srand(time(NULL)); // Gerar números aleatórios
    for (int i = 0; i < NUM_COLETAVEIS; i++){ 
        bool posicaoValida = false;
        while(!posicaoValida){ //verificação para que os coletáveis não aparecam em lugares impróprios 
            coletaveis[i].posicao.x = GetRandomValue(200, 1070);
            coletaveis[i].posicao.y = GetRandomValue(45, 650);
            coletaveis[i].coletado = false;

            Rectangle novoColetavel = {coletaveis[i].posicao.x, coletaveis[i].posicao.y, 20, 20};
            posicaoValida = true;

            //coletaveis nao aparecer em cima dos retangulos:
            for (int j = 0; j < NUM_OBSTACULOS_REC; j++){
                if (CheckCollisionRecs(novoColetavel, obstaculos[j].rect)){
                    posicaoValida = false;
                    break;
                }
            }

            //coletaveis nao aparecer em cima das cadeiras
            for (int j = 0; j < 4; j++){
                if (CheckCollisionCircleRec(cadeiras[j].center, cadeiras[j].radius, novoColetavel)) {
                    posicaoValida = false;
                    break;
                }
            }

            //coletaveis nao aparecer em cima dos poligonos 
            for (int j = 0; j < 2; j++) {
                if (CheckCollisionRecPoly(novoColetavel, poligonos[j])) {
                    posicaoValida = false;
                    break;
                }
            }
        }
    }
}

// Função para atirar
void ShootBullet(Player *player) {
    if(currentBullets<=0 || isReloading) return;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = (Vector2){player->position.x + 25, player->position.y + 25}; // centro do boneco
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
            if (bullets[i].position.x < 0 || bullets[i].position.x > GetScreenWidth() ||
                bullets[i].position.y < 0 || bullets[i].position.y > GetScreenHeight()) {
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
            float scale = 0.025f;
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

    double rotation = getPlayerRotation(&player);

    Vector2 origin = {(frameWidth * scale) / 2, (frameHeight * scale*1.2) / 2.2}; // Ajusta a origem
    DrawTexturePro(player.texture, source, dest, origin, rotation, WHITE);
}

void Player_main(int WIDTH, int HEIGHT, float PLAYER_SPEED, Player *player, Texture2D bulletTexture, Texture2D phaseOneBG){
    float delta = GetFrameTime();

    Object obstaculos[NUM_OBSTACULOS_REC];
    Circle cadeiras[4];
    Polygon poligonos[2];
    Coletavel coletaveis[NUM_COLETAVEIS];

    InicializarObstaculos(obstaculos);
    InicializarCadeiras(cadeiras);
    InicializarPoligonos(poligonos);
    InicializarColetaveis(coletaveis, obstaculos, cadeiras, poligonos);

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
    DrawTexture(phaseOneBG, 0, 0, WHITE);
    DrawPlayer(*player, PLAYER_SPEED);
    FPS_visor();

    // Desenha os projéteis
    DrawBullets(bulletTexture);

    //Mostra Nº de Balas
    DrawText(TextFormat("Balas: %d/%d", currentBullets, MAX_BULLETS), WIDTH-160, HEIGHT-90, 20, WHITE);
    if (isReloading) {
        DrawText("Recarregando...", WIDTH-(WIDTH/8), HEIGHT-(HEIGHT/4.5), 20, RED);
    }

    for (int i = 0; i < NUM_COLETAVEIS; i++) {
        if (!coletaveis[i].coletado) {
            DrawRectangleV(coletaveis[i].posicao, (Vector2){20, 20}, RED);
        }
    }

    EndDrawing();
}