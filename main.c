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
#include "battle/entidades.h"


int direcao = 1;

int main(){
    const int screenX = 1280;
    const int screenY = 720;
    InitWindow(screenX,screenY,"magic battle");
    SetTargetFPS(60); 

    
    //mapa
    int mapa[Map_y][Map_x];
    Texture fundo=carregar_mapa(mapa);
    

    //musica de fundo
    InitAudioDevice();

    Music defaultMusic = LoadMusicStream("som/defaultMusic.mp3");
    PlayMusicStream(defaultMusic);
    SetMusicVolume(defaultMusic, 0.5f);


    // personagem (textura será definida pela animação; evita carregar aqui)
    Personagem p = (Personagem){792, 174, 1.5f, (Texture){0}};
    float escala = 0.05f;

    //inside the battle
    // Carrega configuracoes (paths + stats) para cada batalha em um catálogo simples.
    CatalogoBatalhas catalogo;
    if (!carregar_catalogo_batalhas(&catalogo)) {
        TraceLog(LOG_ERROR, "Falha ao carregar sprites de batalha");
        CloseWindow();
        return 1;
    }

    // Estados iniciais do player/boss em batalha usando batalha 1 como padrao.
    Personagem_em_batalha battle_player = {640, 360, catalogo.batalha1.player.speed, catalogo.batalha1.player.hp, catalogo.batalha1.player.damage, 1, {0}};
    iniciar_animacao_estado(&battle_player.anim, &catalogo.batalha1.player.animacao);
    Personagem_em_batalha boss = {1024, 360, catalogo.batalha1.boss.speed, catalogo.batalha1.boss.hp, catalogo.batalha1.boss.damage, 0, {0}};
    iniciar_animacao_estado(&boss.anim, &catalogo.batalha1.boss.animacao);
    
    henchman henchList[MAX_HENCH]; 
    memset(henchList, 0, sizeof(henchList)); // Zera a lista para garantir que 'active' seja 0

    //TEXTURA DO FUNDO DA BATALHA
    Image img_cenario_de_pedra = LoadImage("img/battle/cenarios/cenario_pedra.png");
    ImageResize(&img_cenario_de_pedra, 1280, 720);
    Texture2D cenario_de_pedra = LoadTextureFromImage(img_cenario_de_pedra);
    UnloadImage(img_cenario_de_pedra);
    
    Image img_cenario_de_fogo = LoadImage("img/battle/cenarios/cenario_fogo.png");
    ImageResize(&img_cenario_de_fogo, 1280, 720);
    Texture2D cenario_de_fogo = LoadTextureFromImage(img_cenario_de_fogo);
    UnloadImage(img_cenario_de_fogo);

    Image img_cenario_de_planta = LoadImage("img/battle/cenarios/cenario_planta.png");
    ImageResize(&img_cenario_de_planta, 1280, 720);
    Texture2D cenario_de_planta = LoadTextureFromImage(img_cenario_de_planta);
    UnloadImage(img_cenario_de_planta);
    
    
    
    // opcoes de tela
    int opcao = 0;
    int opcao_battle = 0; // batalha selecionada
    EntidadesBatalha *ent_atual = &catalogo.batalha1; // ponteiro para a config (sprites + stats) da batalha atual
   
    //lista de locais ondee vai ser iniciada a batalha
    coordenadas *hitbox_para_iniciar_batalha=NULL;
    criar_no(&hitbox_para_iniciar_batalha,370,216,412,239,1);
    criar_no(&hitbox_para_iniciar_batalha,499,391,520,412,2);
    criar_no(&hitbox_para_iniciar_batalha,777,626,819,654,3);
    
    char posText[32];
    
    while(!WindowShouldClose()){
        
        UpdateMusicStream(defaultMusic);

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

            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexture(fundo,0,0,WHITE);
            DrawText(posText, 10, 10, 20, BLACK);
            DrawCircle(p.x,p.y,6,RED);
            
            animacao_do_mago_no_mapa(&p);
            DrawTextureEx(p.t, (Vector2){ p.x - (p.t.width*escala)/2, p.y - ((p.t.height*escala)-430 * escala) }, 0.0f, escala, WHITE); // estou desenhando redimensionando. esse -430 eh pq a imagem do mago tem mt espaço vazio embaixo

            if (verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 1) || verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 2) || verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 3)) {
                DrawText("press space to start battle", 10, 60, 20, BLACK);
                if (IsKeyPressed(KEY_SPACE)) {
                    if (verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 1)) {
                        opcao_battle = 1; 
                        ent_atual = &catalogo.batalha1;
                    }
                    if (verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 2)) {
                        opcao_battle = 2; 
                        ent_atual = &catalogo.batalha2;
                    }
                    if (verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 3)) {
                        opcao_battle = 3; 
                        ent_atual = &catalogo.batalha3;
                    }
                    // Atualiza stats/animações conforme batalha selecionada e reseta estado.
                    reiniciar_batalha(henchList, &battle_player, &boss, ent_atual);
                    DrawText("Batalha iniciada!", 10, 80, 20, BLACK);
                    opcao = 2; // Muda para a tela de batalha
                }
            }
            EndDrawing();
        }
        
        if (opcao==2){

            BeginDrawing();
            ClearBackground(BLACK);

            if (opcao_battle == 1) {
                //batalha de pedra
                batalha(&battle_player, cenario_de_pedra, henchList, ent_atual, &boss, &direcao);
            }
            if (opcao_battle == 2) {
                //batalha de planta
                batalha(&battle_player, cenario_de_planta, henchList, ent_atual, &boss, &direcao);
            }
            if (opcao_battle == 3) {
                //batalha de fogo
                batalha(&battle_player, cenario_de_fogo, henchList, ent_atual, &boss, &direcao);
            }
            if (IsKeyPressed(KEY_M) || battle_player.hp <= 0) {
                opcao = 1; // Volta para a tela do mapa
            }
            EndDrawing();
        }
    }   


    // limpeza final
    descarregar_texturas();
    // descarrega todas as animacoes de batalha compartilhadas.

    descarregar_catalogo_batalhas(&catalogo);
    UnloadTexture(cenario_de_pedra);
    UnloadTexture(cenario_de_fogo);
    UnloadTexture(cenario_de_planta);
    UnloadTexture(fundo);
    descarregar_menu();
    UnloadMusicStream(defaultMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
    
}
