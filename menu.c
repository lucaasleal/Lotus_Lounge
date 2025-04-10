#include "game.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

void FPS_visor() {
    int fps = GetFPS();
    char fpsText[32];
    snprintf(fpsText, sizeof(fpsText), "FPS: %d", fps);
    DrawText(fpsText, 15, 10, 20, WHITE);
}

void Menu(int WIDTH, int HEIGHT, bool *isFullscreen, bool *showSettings, bool *showControls, bool *showCredit, bool *init) {
    // Comentei isso porque tava bugando o som
    //BeginDrawing();
    //ClearBackground(RAYWHITE);
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float scaleX = (float)screenWidth / WIDTH;
    float scaleY = (float)screenHeight / HEIGHT;
    
    Texture2D texture = LoadTexture("assets//imagens//menu.png");
    int fontSize = 40 * scaleY;
    int textY = screenHeight / 4;
    // Define o retângulo de destino (centralizado na tela da atual diemensão)
    Rectangle destRec = {
    (screenWidth - (texture.width *scaleX)) / 2, // Posição X 
    (screenHeight - (texture.height*scaleY)) / 2, // Posição Y
    (texture.width*scaleX), // Largura, matem proporção
    (texture.height*scaleY) // Altura, mantem proporção
    };
    Vector2 origin = { 0, 0 };

    Rectangle sourceRec = { 0, 0, texture.width, texture.height };  //toda a imagem aparece

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
    
    float btnWidth = 220 * scaleX;
    float btnHeight = 50 * scaleY;
    Vector2 mousePos = GetMousePosition();

    Rectangle btnPlay = {screenWidth / 2 - btnWidth /2.7, textY + 100 * scaleY, btnWidth/2, btnHeight*2};
    Rectangle btnControls = {12, screenHeight/2 + screenHeight/3 + 15, btnWidth/2+20, btnHeight*2-10};
    Rectangle btnSettings = {70,  screenHeight/2.3, btnWidth/2, btnHeight*3};
    Rectangle btnExit = {screenWidth - 245*scaleX, textY +70*scaleY, btnWidth/2, btnHeight};
    Rectangle btnCredit = {screenWidth/3 - 60*scaleX, textY + 295 * scaleY, btnWidth/4.3, btnHeight};

    
    if (CheckCollisionPointRec(mousePos, btnPlay)){
        texture = LoadTexture("assets//imagens//play.png");
        DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
        DrawText("Iniciar", screenWidth / 2 - 57 * scaleX, screenHeight/2.45, 15 * scaleY, BLACK);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *init = !(*init);
        }
    }
    
    if (CheckCollisionPointRec(mousePos, btnControls)){
        texture = LoadTexture("assets//imagens//controls.png");
        DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *showControls = !(*showControls);
        }
    }
    
    if (CheckCollisionPointRec(mousePos, btnSettings)){ 
        texture = LoadTexture("assets//imagens//config.png");
        DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *showSettings = !(*showSettings);
        }
    }

    if (CheckCollisionPointRec(mousePos, btnExit)){ 
        texture = LoadTexture("assets//imagens//destexit.png");
        DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            exit(0);
        }
    }

    if (CheckCollisionPointRec(mousePos, btnCredit)){ 
        texture = LoadTexture("assets\\imagens\\creditos.png");
        DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *showCredit = !(*showCredit);
        }
    }

    
    if (*showSettings) {
        DrawRectangle(screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2, BLACK);
        DrawText("HISTORIA\n" ,screenWidth / 2 - 150 * scaleX, screenHeight / 4 + 30 * scaleY, 30 * scaleY, RED);
        DrawText("Você é um ex-policial.\nEsperando uma aposentadoria que nunca veio.\nSem rumo, você decide afogar as mágoas\nno bar mais sujo da cidade: o Lotus Lounge.\nApós a quinta cerveja...\n...o mundo muda.\nZumbis invadem a cidade. O apocalipse começou.\nAgora, sua missão é simples:\nColete 5 garrafas de vodka, mantenha-se vivo\n e encontre uma saída desse pesadelo.\nSua sanidade e sua ressaca dependem disso.\n",
                 screenWidth / 2 - 300 * scaleX, screenHeight / 4 + 70 * scaleY, 20 * scaleY, WHITE);
    }
    
    if (*showControls) {
        DrawRectangle(screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2, BLACK);
        DrawText("Controles do Jogo", screenWidth / 2 - 100 * scaleX, screenHeight / 4 + 20 * scaleY, 30 * scaleY, GRAY);
        DrawText("\nF - Tela Cheia", screenWidth / 4 + 20 * scaleX, screenHeight / 4 + 50 * scaleY, 20 * scaleY, WHITE);
        DrawText("\nW, A, S, D - Mover", screenWidth / 4 + 20 * scaleX, screenHeight / 4 + 75 * scaleY, 20 * scaleY, WHITE);
        DrawText("\nMouse - Mirar", screenWidth / 4 + 20 * scaleX, screenHeight / 4 + 100 * scaleY, 20 * scaleY, WHITE);
        DrawText("\nClique esquerdo - Atirar", screenWidth / 4 + 20 * scaleX, screenHeight / 4 + 125 * scaleY, 20 * scaleY, WHITE);
        
        Rectangle btnBack = {screenWidth / 2 - btnWidth / 2, screenHeight / 4 + screenHeight / 2 - btnHeight - 20 * scaleY, btnWidth, btnHeight};
        DrawRectangleRec(btnBack, DARKGRAY);
        DrawText("Voltar", btnBack.x + btnWidth / 3, btnBack.y + btnHeight / 4, fontSize / 2, WHITE);
        
        if (CheckCollisionPointRec(mousePos, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *showControls = false;
        }
    }

    if (*showCredit) {
        DrawRectangle(screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2, BLACK);
        DrawText("Desenvolvedores", screenWidth / 2 - 100 * scaleX, screenHeight / 4 + 20 * scaleY, 30 * scaleY, GRAY);
        DrawText("André Lima - <alj>\nIsmael Álvaro - <ials>\nLucas Leal - <lfla>\nMaria Gabriella - <mgma>\nMatheus Braglia - <mbcv>\nMilla Rwana - <mras>\nRayssa Vitória - <rvls>", screenWidth / 4 + 20 * scaleX, screenHeight / 4 + 50 * scaleY, 20 * scaleY, WHITE);
        
        Rectangle btnBack = {screenWidth / 2 - btnWidth / 2, screenHeight / 4 + screenHeight / 2 - btnHeight - 20 * scaleY, btnWidth, btnHeight};
        DrawRectangleRec(btnBack, DARKGRAY);
        DrawText("Voltar", btnBack.x + btnWidth / 3, btnBack.y + btnHeight / 4, fontSize / 2, WHITE);
        
        if (CheckCollisionPointRec(mousePos, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *showCredit = false;
        }
    }
    
    
    EndDrawing();
}
