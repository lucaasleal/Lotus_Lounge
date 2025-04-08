#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define MAX_OBSTACULOS 9

typedef struct {
    Rectangle rec;
    int ativo;
    Color cor;
} Object;

void InicializarObstaculosPhaseTwo(Object obstaculos[]) {
    obstaculos[0] = (Object){{0, 0, 200, 720}, 1, RED};       // Parede esquerda
    obstaculos[1] = (Object){{1080, 0, 100, 700}, 1, RED};    // Parede direita
    obstaculos[2] = (Object){{1500, 1450, 1500, 1047}, 1, RED};     // Parede superior
    obstaculos[3] = (Object){{0, 644, 1230, 80}, 1, RED};     // Parede inferior
    obstaculos[4] = (Object){{200, 15, 1000, 220}, 1, RED};    // Cabine
    obstaculos[5] = (Object){{266, 632, 114, 17}, 1, RED};    // Janela 1
    obstaculos[6] = (Object){{484, 535, 740, 110}, 1, RED};   // Pia completa
    obstaculos[7] = (Object){{1180, 0, 100, 720}, 1, RED};  
    obstaculos[8] = (Object){{0, 0, 1500, 100}, 1, RED};

}

bool VerificarColisaoComObstaculos(Rectangle jogador, Object obstaculos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (obstaculos[i].ativo && CheckCollisionRecs(jogador, obstaculos[i].rec)) {
            return true;
        }
    }
    return false;
}

int main() {
    //Só uma inicalização básica
    InitWindow(1280, 720, "Fase 2 - Banheiro");
    SetTargetFPS(60);

    Texture2D mapa = LoadTexture("assets/imagens/lotus_lounge_banheiro.PNG");

    Object obstaculos[MAX_OBSTACULOS];
    InicializarObstaculosPhaseTwo(obstaculos);

    Rectangle jogador = {(1280 - 40) / 2, (720 - 40) / 2, 40, 40};
    float velocidade = 3.0f;

    while (!WindowShouldClose()) {
        Vector2 movimento = {0};

        if (IsKeyDown(KEY_RIGHT)) movimento.x += velocidade;
        if (IsKeyDown(KEY_LEFT)) movimento.x -= velocidade;
        if (IsKeyDown(KEY_UP)) movimento.y -= velocidade;
        if (IsKeyDown(KEY_DOWN)) movimento.y += velocidade;

        Rectangle proximo = jogador;
        proximo.x += movimento.x;
        proximo.y += movimento.y;

        if (!VerificarColisaoComObstaculos(proximo, obstaculos, MAX_OBSTACULOS)) {
            jogador = proximo;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(mapa, 0, 0, WHITE);

        for (int i = 0; i < MAX_OBSTACULOS; i++) {
            if (obstaculos[i].ativo)
                DrawRectangleRec(obstaculos[i].rec, Fade(obstaculos[i].cor, 0.3f));
        } 

        DrawRectangleRec(jogador, BLUE);

        EndDrawing();
    }

    UnloadTexture(mapa);
    CloseWindow();
    return 0;
}
