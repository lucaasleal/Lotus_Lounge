#ifndef MENU_H
#define MENU_H

#include "raylib.h"

#define NUM_COLETAVEIS 5
#define NUM_OBSTACULOS_REC 10

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
    double life;
} Player;

typedef struct Object {
    Rectangle rect;
    int blocking;
    Color color;
} Object;


typedef struct{
    Vector2 posicao;
    bool coletado;
} Coletavel;

typedef struct {
    Vector2 center;
    float radius;
} Circle;

typedef struct {
    Vector2 *points;
    int pointCount;
} Polygon;

typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool active;
} Bullet;

typedef struct Zombie {
    Vector2 position;
    Rectangle hitbox;
    Texture2D texture;
    float speed;
    float life;
    bool alive;
    int frame;
    float timer;
    int direction; // 0: baixo, 1: cima, 2: esquerda, 3: direita
} Zombie;

bool coletaveisinicializados;
Object obstaculos[NUM_OBSTACULOS_REC];
Circle cadeiras[4]; //tive que declarar fora de player_main 
Polygon poligonos[2];
Coletavel coletaveis[NUM_COLETAVEIS];

void FPS_visor();
void Menu(int WIDTH, int HEIGHT, bool *isFullscreen, bool *showSettings, bool *showControls, bool *showCredit, bool *init);
bool text_board(int *framesCounter, int *letterCount, int *velocidade, const char *text[], int numLines);
Player InitPlayer(Texture2D texture, Vector2 position, float PLAYER_SPEED);
void Player_main(int WIDTH, int HEIGHT, float PLAYER_SPEED, Player *player, Texture2D bulletTexture, Texture2D phaseOneBG, Texture2D bottleTexture, bool *init);


void InicializarObstaculos(Object obstaculos[]);
void InicializarCadeiras(Circle cadeiras[]);
void InicializarPoligonos(Polygon poligonos[]);
void InicializarColetaveis(Coletavel coletaveis[], Object obstaculos[], Circle cadeiras[], Polygon poligonos[]);
int CheckCollisionRecPoly(Rectangle Rec, Polygon poly);




#include "menu.c"
#include "game.c"
#include "player.c"
#include "phaseOne.c"
#endif
