#include "raylib.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

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