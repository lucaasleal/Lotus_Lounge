#include "raylib.h"
#include "game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#ifndef PI
#define PI 3.14
#endif
//Teste
// #define MAX_ZOMBIES 100

#define MAX_BULLETS 20
#define RELOAD_TIME 2.0f // em segundos
#define NUM_COLETAVEIS 23
#define NUM_OBSTACULOS_REC 10
#define MAX_VODKA 5
#define MAX_CIGARRETS 7
#define MAX_DISKS 10

Sound shotSound;
Sound reloadSound;

int currentBullets = MAX_BULLETS;
bool isReloading = false;
float reloadTimer = 0.0f;
extern bool coletaveisinicializados;
extern Object obstaculos[NUM_OBSTACULOS_REC];
extern Circle cadeiras[4]; //declarei aqui pra ele poder usar da main
extern Polygon poligonos[2];
extern Coletavel coletaveis[NUM_COLETAVEIS];

Bullet bullets[MAX_BULLETS];
Zombie zombies[100];
int drops = 0;

bool win = false;

// Função que inicializa sons
void InitPlayerSounds() {
    shotSound = LoadSound("assets/OCD/shot.ogg");
    reloadSound = LoadSound("assets/OCD/reload.ogg");
}
void InitZombies(Zombie zombies[], int numZombies) {
    for (int i = 0; i < numZombies; i++) {
        zombies[i].alive = false;
        zombies[i].hitbox = (Rectangle){0, 0, 50, 50};
        zombies[i].texture = LoadTexture("assets\\imagens\\zumbi.png");
        zombies[i].frame = 0;
        zombies[i].timer = 0.0f;
    }
}

bool try2DropColetavel(Player *player, Zombie zombie, Coletavel *coletaveis, int numColetaveis, Object obstaculos[], Circle cadeiras[], Polygon poligonos[], int level) {
    if(GetRandomValue(0, 100) < 30) { // 30% chance de dropar
        for (int i = 0; i < numColetaveis; i++) {
            if (!coletaveis[i].dropado && (level == 1 && drops + player->vodka <= 5) || (level == 2 && drops + player->cigarrets <= 7) || (level == 3 && drops + player->disks <= 10)) {
                bool valid_drop = true;
                for (int j = 0; j < NUM_OBSTACULOS_REC; j++){
                    if (CheckCollisionRecs(zombie.hitbox, obstaculos[j].rect)){
                        valid_drop = false;
                        break;
                    }
                }

                //coletaveis nao aparecer em cima das cadeiras
                for (int j = 0; j < 4; j++){
                    if (CheckCollisionCircleRec(cadeiras[j].center, cadeiras[j].radius, zombie.hitbox)) {
                        valid_drop = false;
                        break;
                    }
                }

                //coletaveis nao aparecer em cima dos poligonos 
                for (int j = 0; j < 2; j++) {
                    if (CheckCollisionRecPoly(zombie.hitbox, poligonos[j])) {
                        valid_drop = false;
                        break;
                    }
                }
                if(valid_drop) {
                    coletaveis[i].coletado = false;
                    coletaveis[i].dropado = true;
                    coletaveis[i].posicao = (Vector2){zombie.position.x, zombie.position.y};
                    drops++;
                    return true;
                } else {
                    return false;
                }
            }
        }
    }
    return false;
}

void SpawnZombies(Zombie zombies[], int numZombies, Rectangle spawnZones[], int numSpawnZones, int level) {
    for (int i = 0; i < numZombies; i++) {
        if (!zombies[i].alive && GetRandomValue(0, 1000) <= 20) { // Spawn a new zombie every second
            int spawnZoneIndex = rand() % numSpawnZones;
            zombies[i].position.x = spawnZones[spawnZoneIndex].x + GetRandomValue(0, (int)(spawnZones[spawnZoneIndex].width - 50));
            zombies[i].position.y = spawnZones[spawnZoneIndex].y + GetRandomValue(0, (int)(spawnZones[spawnZoneIndex].height - 50));
            zombies[i].alive = true;
            zombies[i].life = 100.0f + GetRandomValue(-20, 50);
            zombies[i].speed = 50.0f + GetRandomValue(0, 20);
            if(level == 1){
                zombies[i].dash = false;
                zombies[i].bite = false;
            } else if(level == 2){
                zombies[i].dash = (GetRandomValue(0, 100) < 40) ? true : false;
                zombies[i].bite = false;
                zombies[i].speed += 10.0f;
                zombies[i].life += 50.0f;
            } else if(level == 3){
                zombies[i].dash = true;
                zombies[i].bite = (GetRandomValue(0, 100) < 30) ? true : false;
                zombies[i].speed += 20.0f;
                zombies[i].life += 100.0f;
            }
        }
    }
}

void ControlZombies(Zombie *zombie, Player *player, float delta, Object obstaculos[], int NumObstaculos, Zombie zombies[], int numZombies) {
    if (zombie->alive) {
        Vector2 direction = {player->position.x - zombie->position.x, player->position.y - zombie->position.y};
        float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
        float MULT = 1.0f;
        bool hit = false;
        for(int i = 0; i < NumObstaculos; i++) {
            if (CheckCollisionRecs((Rectangle){zombie->position.x, zombie->position.y, 50, 50}, obstaculos[i].rect)) {
                MULT = 0.5f;
            }
        }

        if(!hit) {
            if (distance > 30) {
                direction.x /= distance;
                direction.y /= distance;
                float dash = (zombie->dash && GetRandomValue(0, 100) < 10) ? 5.0f : 1.0f;
                zombie->position.x += direction.x * zombie->speed * delta * MULT * dash;
                zombie->position.y += direction.y * zombie->speed * delta * MULT * dash;

                // Atualiza a hitbox do zumbi
                zombie->hitbox.x = zombie->position.x;
                zombie->hitbox.y = zombie->position.y;
                
                zombie->timer += delta;
                if (zombie->timer >= 0.1f) {
                    zombie->frame = (zombie->frame + 1) % 9; // supondo 9 frames
                    zombie->timer = 0;
                }
            } else {
                player->life -= 0.1f; // Dano ao jogador se o zumbi colidir
                if(zombie->bite && GetRandomValue(0, 100) < 40) {
                    player->life -= 10.0f; // Dano extra se o zumbi morder
                }
            }            
        }
    }
}

void CheckBulletCollision(Player *player, Zombie *zombie, Bullet *bullet, int level) {
    if (bullet->active && CheckCollisionRecs(zombie->hitbox, (Rectangle){bullet->position.x, bullet->position.y, 10, 10})) {
        bullet->active = false;
        zombie->life -= 30.0f; // Dano ao zumbi
        if (zombie->life <= 0) {
            // Tenta dropar coletável
            if (try2DropColetavel(player, *zombie, coletaveis, NUM_COLETAVEIS, obstaculos, cadeiras, poligonos, level)) {
                // Coletável foi dropado
            } else {
                // Não conseguiu dropar coletável
            }
            zombie->alive = false; // Zumbi morre
        }
    }
}

void DrawZombies(Zombie zombies[], int numZombies, Player *player) {
    for (int i = 0; i < numZombies; i++) {
        if (zombies[i].alive) {
            int frameWidth = zombies[i].texture.width / 3;  // 3 frames por linha
            int frameHeight = zombies[i].texture.height; // 4 direções

            Rectangle source = {
                frameWidth * zombies[i].frame,
                frameHeight * zombies[i].direction,
                frameWidth,
                frameHeight
            };

            float scale = 2.5f;
            Rectangle dest = {
                zombies[i].position.x + 25,
                zombies[i].position.y + 25,
                frameWidth * scale,
                frameHeight * scale
            };


            // direção do zumbi até o player
            Vector2 playerPos = {zombies[i].hitbox.x, zombies[i].hitbox.y}; // Ou use player->position se quiser passar o player aqui
            Vector2 dir = {player->position.x - zombies[i].position.x, player->position.y - zombies[i].position.y};
            float rotation = atan2f(dir.y, dir.x) * 180.0f / PI;

            Vector2 origin = {(frameWidth * scale) / 2, (frameHeight * scale) / 2};
            DrawTexturePro(zombies[i].texture, source, dest, origin, rotation-90, WHITE);

            // Debug hitbox (opcional)
            // DrawRectangleLines(zombies[i].hitbox.x, zombies[i].hitbox.y, zombies[i].hitbox.width, zombies[i].hitbox.height, RED);
        }
    }
}

float GetDistance(Vector2 a, Vector2 b) {
    return sqrtf((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
}

double getPlayerRotation(Player *player);
// Estrutura do projétil

Player InitPlayer(Texture2D texture, Vector2 position, float PLAYER_SPEED) {
    Player player = {position, PLAYER_SPEED, texture, 0, 0.0f, 0, 100, 0, 0};
    return player;
}


// Função para atirar
void ShootBullet(Player *player) {
    if(currentBullets<=0 || isReloading) return;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            PlaySound(shotSound);
            bullets[i].position = (Vector2){player->position.x+25, player->position.y+25}; // centro do boneco
            float angle = (getPlayerRotation(player)  - 90.0f)* (PI / 180.0f) + GetRandomValue(-player->vodka * 3, player->vodka * 3)*(PI/180.0);
            bullets[i].velocity = (Vector2){cosf(angle) * 1000, sinf(angle) * 1000}; // velocidade
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
            float scale = 0.01f;
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


void getPlayerPos(Player *player, float delta, float PLAYER_SPEED, Object obstaculos[], Circle cadeiras[], Polygon poligonos[], Coletavel coletaveis[], bool isReloading, int level) {

    for (int i = 0; i < NUM_COLETAVEIS; i++) {
        if (!coletaveis[i].coletado &&
            CheckCollisionRecs((Rectangle){player->position.x, player->position.y, 32, 32}, //32 é o player Size TODO
                                (Rectangle){coletaveis[i].posicao.x, coletaveis[i].posicao.y, 20, 20})) {
            coletaveis[i].coletado = true;
            drops--;
            switch(level){
                case 1:
                    player->vodka += 1;
                    break;
                case 2:
                    player->cigarrets += 1;
                    break;
                case 3:
                    player->disks += 1;
                    break;
            }
        }
    }
    Vector2 movement = {0, 0};
    bool isMoving = false;
    float MUL = (isReloading) ? 0.5 : 1;
    if (IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) { movement.y -= PLAYER_SPEED * MUL * delta; player->direction = 1; isMoving = true; }
    if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) { movement.x -= PLAYER_SPEED * MUL * delta; player->direction = 2; isMoving = true; }
    if (IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)) { movement.y += PLAYER_SPEED * MUL * delta; player->direction = 0; isMoving = true; }
    if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) { movement.x += PLAYER_SPEED * MUL * delta; player->direction = 3; isMoving = true; }

    Rectangle PlayerRect = {player->position.x + movement.x, player->position.y + movement.y, 40, 40};
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
            player->frame = (player->frame + 1) % 9; // Alterna entre 4 frames
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

void Player_main(int WIDTH, int HEIGHT, float PLAYER_SPEED, Player *player, Texture2D bulletTexture, Texture2D phaseOneBG, Texture2D bottleTexture, Texture2D cigarretsTexture, Texture2D diskTexture, Texture2D lifeplayer, bool *init, int level) {
    float delta = GetFrameTime();
    int numZombies = 5 + player->vodka + player->cigarrets + player->disks; // Número de zumbis a serem gerados
    Rectangle spawnZones[2] = {
        {282, 580, 400, 80},
        {950, 78, 100, 200},
    };
    if(player->vodka >= MAX_VODKA) level = 2; 
    if(player->cigarrets >= MAX_CIGARRETS) level = 3; 
    if(player->disks >= MAX_DISKS) win = true;

    if (!coletaveisinicializados){ //booleano para garantir que ele gere os coletaveis uma vez 
    InitZombies(zombies, 100);
    InicializarObstaculos(obstaculos);
    InicializarCadeiras(cadeiras);
    InicializarPoligonos(poligonos);
    coletaveisinicializados = true;}
    // Atualiza posição do jogador
    getPlayerPos(player, delta, PLAYER_SPEED, obstaculos, cadeiras, poligonos, coletaveis, isReloading, level);
    for(int i = 0; i < numZombies; ++i) {
        if(zombies[i].alive) {
            ControlZombies(&zombies[i], player, delta, obstaculos, NUM_OBSTACULOS_REC, zombies, numZombies);
        }
    }
    for(int i = 0; i < currentBullets; i++) {
        for (int j = 0; j < numZombies; j++) {
            CheckBulletCollision(player, &zombies[j], &bullets[i], level);
        }
    }

    // Atira ao apertar espaço
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ShootBullet(player);
    }
    // Atualiza os tiros
    UpdateBullets(delta);
    // Seta a posição dos zombies
    SpawnZombies(zombies, numZombies, spawnZones, 2, level);

    //Lógica de Recarga
    if (((IsKeyPressed(KEY_R) && currentBullets < MAX_BULLETS) || 
       (currentBullets==0 && (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)))) && !isReloading) {
        isReloading = true;
        reloadTimer = 0.0f;
        PlaySound(reloadSound);
    }
    
    if (isReloading) {
        reloadTimer += delta;
        if (reloadTimer >= RELOAD_TIME) {
            currentBullets = MAX_BULLETS;
            isReloading = false;
            reloadTimer = 0.0f;
        }
    }

    static bool gameOver = false;

    if (player->life <= 0) {
        gameOver = true;
    }

    if (gameOver) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("GAME OVER", WIDTH/2 - MeasureText("GAME OVER", 50)/2, HEIGHT/2 - 25, 50, RED);
        DrawText("Pressione [ENTER] para voltar ao menu", WIDTH/2 - MeasureText("Pressione [ENTER] para voltar ao menu", 20)/2, HEIGHT/2 + 40, 20, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_ENTER)) {
            coletaveisinicializados = false; // reinicia obstáculos/coletáveis
            *init = false;
            gameOver = false;
            player->life = 100;
            player->vodka = 0;
            player->cigarrets = 0;
            player->disks = 0;
            currentBullets = 20;
            win = false;
            player->position = (Vector2){WIDTH/2, HEIGHT/2}; // ou posição inicial original
            return; // retorna para que o main decida exibir o menu
        }
        return; // interrompe o resto do desenho se o jogo acabou
    }

    if(win) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("YOU WON!", WIDTH/2 - MeasureText("YOU WON", 50)/2, HEIGHT/2 - 25, 50, GREEN);
        DrawText("Mas a vida continua... bebida, cigarros e musica.", WIDTH/2 - MeasureText("Mas a vida continua...", 20)/2, HEIGHT/2 + 40, 20, WHITE);
        DrawText("Pressione [ENTER] para voltar ao menu", WIDTH/2 - MeasureText("Pressione [ENTER] para voltar ao menu", 20)/2, HEIGHT/2 + 80, 20, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_ENTER)) {
            coletaveisinicializados = false; // reinicia obstáculos/coletáveis
            *init = false;
            gameOver = false;
            player->life = 100;
            player->vodka = 0;
            player->cigarrets = 0;
            player->disks = 0;
            currentBullets = 20;
            win = false;
            player->position = (Vector2){WIDTH/2, HEIGHT/2}; // ou posição inicial original
            return; // retorna para que o main decida exibir o menu
        }
        return; // interrompe o resto do desenho se o jogo acabou
    }

    // Desenho da cena
    BeginDrawing();
    ClearBackground(BLACK);
    Vector2 mouse = GetMousePosition();
    DrawTexture(phaseOneBG, 20, 0, WHITE);
    DrawText(TextFormat("Nivel: %d", level), 550, 350, 40, WHITE);
    DrawText("Pressione [ESC] para voltar ao menu", 10, 10, 10, WHITE);
    DrawText("Pressione [R] para recarregar", 10, 30, 10, WHITE);
    DrawText("Pressione [F] para fullscreen", 10, 50, 10, WHITE);

    // Desenha os projéteis
    DrawBullets(bulletTexture);
    DrawPlayer(*player, PLAYER_SPEED);
    DrawZombies(zombies, numZombies, player);
    //FPS_visor();
    //DrawText(TextFormat("Mouse X: %.0f Y: %.0f", mouse.x, mouse.y), 300, 10, 20, WHITE);
    
    //desenha o controle de balas
    float rotation = 320.0f; // Rotação de 300°
    Vector2 position = {WIDTH-195, HEIGHT-80};
    Rectangle source = {0, 0, bulletTexture.width, bulletTexture.height};
    Rectangle dest = {position.x, position.y, bulletTexture.width * 0.050f, bulletTexture.height * 0.050f};
    Vector2 origin = {0, 0}; // Pivot no centro
    DrawTexturePro(bulletTexture, source, dest, origin, rotation, WHITE);
    DrawText(TextFormat("%d/%d", currentBullets, MAX_BULLETS), WIDTH-130, HEIGHT-92, 20, WHITE);

    //desenha a quantidade de canas coletadas
    if (level == 1){
    float girocana = 320.0f; // Rotação de 300°
    Vector2 positioncana = {WIDTH-175, HEIGHT-125};
    Rectangle sourcecana = {0, 0, bottleTexture.width, bottleTexture.height};
    Rectangle destcana = {positioncana.x, positioncana.y, bottleTexture.width * 0.080f, bottleTexture.height * 0.080f};
    Vector2 origincana = {0, 0}; // Pivot no centro
    DrawTexturePro(bottleTexture, sourcecana, destcana, origincana, girocana, WHITE);
    DrawText(TextFormat("%d/5", player->vodka), WIDTH-120, HEIGHT-130, 20, WHITE);}
    //desenha a quantidade de cigarros coletados
    if (level==2){
    float girocigarro = 0.0f; // Rotação de 300°
    Vector2 positioncigarro = {WIDTH-165, HEIGHT-140};
    Rectangle sourcecigarro = {0, 0, cigarretsTexture.width, cigarretsTexture.height};
    Rectangle destcigarro = {positioncigarro.x, positioncigarro.y, cigarretsTexture.width * 1.3f, cigarretsTexture.height * 1.3f};
    Vector2 origincigarro = {0, 0}; // Pivot no centro
    DrawTexturePro(cigarretsTexture, sourcecigarro, destcigarro, origincigarro, girocigarro, WHITE);
    DrawText(TextFormat("%d/10", player->cigarrets), WIDTH-120, HEIGHT-130, 20, WHITE);}
    //desenha a quantidade de discos coletados
    if (level==3){
    float girocana = 320.0f; // Rotação de 300°
    Vector2 positioncana = {WIDTH-175, HEIGHT-125};
    Rectangle sourcecana = {0, 0, diskTexture.width, diskTexture.height};
    Rectangle destcana = {positioncana.x, positioncana.y, bottleTexture.width * 0.080f, bottleTexture.height * 0.080f};
    Vector2 origincana = {0, 0}; // Pivot no centro
    DrawTexturePro(diskTexture, sourcecana, destcana, origincana, girocana, WHITE);
    DrawText(TextFormat("%d/10", player->disks), WIDTH-120, HEIGHT-130, 20, WHITE);}
    
    //desenha a vida do player
    //DrawText(TextFormat("life: %.0f", player->life), 600, 10, 20, WHITE);

    //desenha vidas
    //desenha o contreole  da vida do player
    Vector2 positlife = {WIDTH-170, HEIGHT-175};
    Rectangle sourcelife = {0, 0, lifeplayer.width, lifeplayer.height};
    Rectangle destlife = {positlife.x, positlife.y, lifeplayer.width * 1.6f, lifeplayer.height * 1.6f};
    Vector2 originlife = {0, 0};
    DrawTexturePro(lifeplayer, sourcelife, destlife, originlife, 0.0f, WHITE);
    DrawText(TextFormat("%.0f", player->life), WIDTH-122, HEIGHT-167, 20, WHITE);
    //DrawText(TextFormat("Mouse X: %.0f Y: %.0f", mouse.x, mouse.y), 300, 10, 20, WHITE);

    if (isReloading) {
        DrawText("Recarregando...", WIDTH-(WIDTH/8), HEIGHT-(HEIGHT/3), 20, RED);
    }

    for (int i = 0; i < NUM_COLETAVEIS; i++) {
        if (!coletaveis[i].coletado && coletaveis[i].dropado) {
            float scale = 0.08f;
            Rectangle source = {0, 0, (float)bottleTexture.width, (float)bottleTexture.height};
            Rectangle dest = {
                coletaveis[i].posicao.x,
                coletaveis[i].posicao.y,
                bottleTexture.width * scale,
                bottleTexture.height * scale
            };
            Vector2 origin = {dest.width / 2, dest.height / 2};
            if(level == 1){
                DrawTexturePro(bottleTexture, source, dest, origin, 0, WHITE);
            } else if(level == 2){
                DrawTexturePro(cigarretsTexture, (Rectangle){0, 0, (float)cigarretsTexture.width, (float)cigarretsTexture.height}, (Rectangle){coletaveis[i].posicao.x, coletaveis[i].posicao.y, cigarretsTexture.width * 1.2f, cigarretsTexture.height * 1.2f}, origin, 0, WHITE);
            } else if(level == 3){
                DrawTexturePro(diskTexture, (Rectangle){0, 0, (float)diskTexture.width, (float)diskTexture.height}, (Rectangle){coletaveis[i].posicao.x, coletaveis[i].posicao.y, diskTexture.width * 0.02f, diskTexture.height * 0.02f}, origin, 0, WHITE);
            }
        }
    }
    EndDrawing();
}

void UnloadPlayerSounds() {
    UnloadSound(shotSound);
    UnloadSound(reloadSound);
}
