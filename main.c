#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(){
    const int screenX = 1280;
    const int screenY = 720;
    InitWindow(screenX,screenY,"magic battle"); // inicializando janela
    SetTargetFPS(60); // fps
    
    Image img = LoadImage("img/mapa.png");
    ImageResize(&img, screenX, screenY);

    Texture2D mapa = LoadTextureFromImage(img);

    UnloadImage(img);
    int x=0;
    int y=0;
    int speed = 3;

    while(!WindowShouldClose()){// loop principal + windowshouldclose= analise se a janela esta fechada, nesse caso o loop roda enquanto a janela esta aberta por causa do not
        // --- MOVIMENTO ---
        if (IsKeyDown(KEY_RIGHT)) x += speed;
        if (IsKeyDown(KEY_LEFT))  x -= speed;
        if (IsKeyDown(KEY_UP))    y -= speed;
        if (IsKeyDown(KEY_DOWN))  y += speed;

        // --- LIMITE DE TELA ---
        if (x < 0) x = 0;
        if (x > screenX) x = screenX;
        if (y < 0) y = 0;
        if (y > screenY) y = screenY;
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(mapa,0,0,WHITE);
        DrawCircle(x,y,10,RED);
        EndDrawing();
        printf("posicao %d %d\n",x,y);
    }
    UnloadTexture(mapa);
    CloseWindow();// fechar a janela

    return 0;
}

//funcao para escrever algo na tela :
//BeginDrawing()
//
//nesse meio eu coloco a funcao com o formato da escrita e seus parametros ex:
// DrawText(texto,posicao x, posicao y, tamanho da fonte, cor)
//
//EndDrawing()


//carregar img:
//Texture nome da variave =x;
//x = LoadTexture(caminho para img)
//funcao para dar o diplay da img na tela = DrawTexture(x, posicao x, posicao y, cor)

// add fundo do jogo: tem que ser carregado antes de tudo
//vai dar um loadtexture()
//drawtexture(variavel, 0, 0,white)
