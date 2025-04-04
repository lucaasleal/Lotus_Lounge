#include "raylib.h"

#define PLAYER_SPEED 200.0f
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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

Player InitPlayer(Texture2D texture, Vector2 position) {
    Player player = {position, PLAYER_SPEED, texture, 0, 0.0f, 0};
    return player;
}

void getPlayerPos(Player *player, Object *objects, int n_objects, float delta) {
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

void DrawPlayer(Player player) {
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

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jogo");
    SetTargetFPS(60);

    Texture2D spriteSheet = LoadTexture("C://Users//lucas//Downloads//boneco.png");
    Player player = InitPlayer(spriteSheet, (Vector2){SCREEN_WIDTH / 16, SCREEN_HEIGHT / 12});

    Object objects[5] = {
        {{500, 500, 100, 100}, 1, RED},
        {{0, 0, SCREEN_WIDTH, 10}, 1, BLACK},  // Parede superior
        {{0, SCREEN_HEIGHT - 10, SCREEN_WIDTH, 10}, 1, BLACK},  // Parede inferior
        {{0, 0, 10, SCREEN_HEIGHT}, 1, BLACK},  // Parede esquerda
        {{SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT}, 1, BLACK}  // Parede direita
    };
    int num_objects = sizeof(objects) / sizeof(objects[0]);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        getPlayerPos(&player, objects, num_objects, delta);

        BeginDrawing();
        ClearBackground(WHITE);
        DrawPlayer(player);
        for (int i = 0; i < num_objects; ++i) {
            DrawRectangleV((Vector2){objects[i].rect.x, objects[i].rect.y}, 
                           (Vector2){objects[i].rect.width, objects[i].rect.height}, 
                           objects[i].color);
        }
        EndDrawing();
    }

    UnloadTexture(spriteSheet);
    CloseWindow();
    return 0;
}