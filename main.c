#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mapa/hitbox_mapa.h"
#include "menus/menuprincipal.h"
#include "movimentacao/animacao.h"
#include "battle/battles.h"
#include "battle/battlefunctions.h"


int main(){
    const int screenX = 1280;
    const int screenY = 720;
    InitWindow(screenX,screenY,"magic battle"); // inicializando janela
    SetTargetFPS(60); // fps
    
    //mapa
    int mapa[Map_y][Map_x];
    Texture fundo=carregar_mapa(mapa);
    
    // personagem (textura será definida pela animação; evita carregar aqui)
    Personagem p = (Personagem){792, 174, 1.5f, (Texture){0}};
    float escala = 0.05f;

    Personagem_em_batalha battle_player = {640, 360, 300.0f, 100, 10, LoadTexture("img/battle/player/leste1.png")};

    
    // opcoes de tela
    int opcao = 0;
   
    //lista de locais ondee vai ser iniciada a batalha
    coordenadas *hitbox_para_iniciar_batalha=NULL;
    criar_no(&hitbox_para_iniciar_batalha,370,216,412,239,1);
    criar_no(&hitbox_para_iniciar_batalha,499,391,520,412,2);
    criar_no(&hitbox_para_iniciar_batalha,777,626,819,654,3);
    
    char posText[32];
    // char rgb[64];
    while(!WindowShouldClose()){
        
        if(opcao==0){
            // menu principal (desenha e atualiza a opção)
            mostrar_menu(&opcao);
            if (opcao == -1) break; // sair pelo menu
            continue; // não desenhar o jogo quando ainda no menu
        }

        if(opcao==1){
            
            //atualiza a posição do personagem com a mecanica de colisao
            logica_de_colisao_movimentacao(&p, mapa);
            int x = (int)p.x;
            int y = (int)p.y;
            sprintf(posText, "x: %d  y: %d", x, y);

            // // Se voce tirar ele como moentario, voce consegue mostrar o valor RGB do pixel sob o mouse
            // Vector2 mouse = GetMousePosition();
            // Color pixel = GetImageColor(LoadImageFromScreen(), mouse.x, mouse.y);
            // sprintf(rgb,"R:%d G:%d B:%d A:%d\n", pixel.r, pixel.g, pixel.b, pixel.a);

            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexture(fundo,0,0,WHITE);
            // // SE VC TIRAR ELE COMO COMENTARIO, VC CONSEGUE VISUALIZAR A HIT BOX
            // Color vermelhoSemi = (Color){ 255, 0, 0, 100 };
            // Color verdeSemi = (Color){0,255,0,100};
            // for (int y = 0; y < Map_y; y++) {
            //     for (int x = 0; x < Map_x; x++) {
            //         Color c = (mapa[y][x] == Tile_Nao_anda) ? vermelhoSemi : verdeSemi;
            //         DrawRectangle(x * Tile_size, y * Tile_size, Tile_size, Tile_size, c);
            //     }
            // }
            // DrawText(rgb, 10, 40, 20, BLACK);
            DrawText(posText, 10, 10, 20, BLACK);
            DrawCircle(p.x,p.y,6,RED);
            
            animacao_do_mago_no_mapa(&p);
            DrawTextureEx(p.t, (Vector2){ p.x - (p.t.width*escala)/2, p.y - ((p.t.height*escala)-430 * escala) }, 0.0f, escala, WHITE); // estou desenhando redimensionando. esse -430 eh pq a imagem do mago tem mt espaço vazio embaixo

            if (verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 1) || verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 2) || verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 3)) {
                DrawText("press space to start battle", 10, 60, 20, BLACK);
                if (IsKeyPressed(KEY_SPACE)) {
                    
                    DrawText("Batalha iniciada!", 10, 80, 20, BLACK);
                    opcao = 2; // Muda para a tela de batalha
                }
            }
            EndDrawing();
        
        }
        
        if (opcao==2){
            //tela de batalha
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Batalha em andamento... Pressione M para voltar ao mapa.", 10, 10, 20, BLACK);
            EndDrawing();

            if (IsKeyPressed(KEY_M)) {
                opcao = 1; // Volta para a tela do mapa
            }

            batalha1(&battle_player);

        }
    }   


    // limpeza final
    descarregar_texturas();
    UnloadTexture(fundo);
    descarregar_menu();
    CloseWindow();
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
