#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mapa/hitbox_mapa.h"
#include "menus/menuprincipal.h"


int main(){
    const int screenX = 1280;
    const int screenY = 720;
    InitWindow(screenX,screenY,"magic battle"); // inicializando janela
    SetTargetFPS(60); // fps
    
    int mapa[Map_y][Map_x];
    
    
    //personagem
    Personagem p = {792, 174, 2.5f};
    
    // opcoes de tela
    int opcao = 0;
    
    
    char posText[32];
    char rgb[64];
    while(!WindowShouldClose()){// loop principal + windowshouldclose= analise se a janela esta fechada, nesse caso o loop roda enquanto a janela esta aberta por causa do not
        
        if(opcao==0){
            //menu principal
            mostrar_menu(&opcao);
        }

        if(opcao==1){
            //tela de jogo
            Texture fundo=carregar_mapa(mapa);

            //atualiza a posição do personagem com a mecanica de colisao
            logica_de_colisao_movimentacao(&p, mapa);
            int x = (int)p.x;
            int y = (int)p.y;
            sprintf(posText, "x: %d  y: %d", x, y);

            // mostra o valor RGB do pixel sob o mouse
            Vector2 mouse = GetMousePosition();
            Color pixel = GetImageColor(LoadImageFromScreen(), mouse.x, mouse.y);
            sprintf(rgb,"R:%d G:%d B:%d A:%d\n", pixel.r, pixel.g, pixel.b, pixel.a);

            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexture(fundo,0,0,WHITE);
            // SE VC TIRAR ELE COMO COMENTARIO, VC CONSEGUE VISUALIZAR A HIT BOX
            Color vermelhoSemi = (Color){ 255, 0, 0, 100 };
            Color verdeSemi = (Color){0,255,0,100};
            for (int y = 0; y < Map_y; y++) {
                for (int x = 0; x < Map_x; x++) {
                    Color c = (mapa[y][x] == Tile_Nao_anda) ? vermelhoSemi : verdeSemi;
                    DrawRectangle(x * Tile_size, y * Tile_size, Tile_size, Tile_size, c);
                }
            }
            DrawText(rgb, 10, 40, 20, BLACK);
            DrawText(posText, 10, 10, 20, BLACK);
            DrawCircle(p.x,p.y,6,RED);

            EndDrawing();
        }
        
    }
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
