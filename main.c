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


int direcao = 1;

int main(){
    const int screenX = 1280;
    const int screenY = 720;
    InitWindow(screenX,screenY,"magic battle");
    SetTargetFPS(60); 
    
    //mapa
    int mapa[Map_y][Map_x];
    Texture fundo=carregar_mapa(mapa);
    
    // personagem (textura será definida pela animação; evita carregar aqui)
    Personagem p = (Personagem){792, 174, 1.5f, (Texture){0}};
    float escala = 0.05f;

    //inside the battle
    // Animacoes usadas dentro da batalha para player, boss e capangas (obtidas do gerenciador).
    BattleAnimation *player_battle_anim = obter_battle_animation(BATTLE_ANIM_PLAYER_MAGO);
    Personagem_em_batalha battle_player = {640, 360, 300.0f, 100, 10, 1, player_battle_anim};
    BattleAnimation *boss_anim = obter_battle_animation(BATTLE_ANIM_BOSS_MONSTRO_FOGO);
    Personagem_em_batalha boss = {1024, 360, 200.0f, 100, 10, 0, boss_anim};
    
    henchman henchList[MAX_HENCH]; 
    memset(henchList, 0, sizeof(henchList)); // Zera a lista para garantir que 'active' seja 0

    BattleAnimation *henchman_anim = obter_battle_animation(BATTLE_ANIM_HENCH_MONSTRO_PEDRA);
    // TEM QUE SUBSTITUIR ESSE TRECHO PARA O LOADTEXTURE DA TEEXTURA DO FUNDO DA BATALHA
    Image img = GenImageColor(1280, 720, BLACK);
    Texture2D backgroud_sprite = LoadTextureFromImage(img);
    UnloadImage(img);
    
    
    
    // opcoes de tela
    int opcao = 0;
    int opcao_battle = 0; // batalha selecionada
   
    //lista de locais ondee vai ser iniciada a batalha
    coordenadas *hitbox_para_iniciar_batalha=NULL;
    criar_no(&hitbox_para_iniciar_batalha,370,216,412,239,1);
    criar_no(&hitbox_para_iniciar_batalha,499,391,520,412,2);
    criar_no(&hitbox_para_iniciar_batalha,777,626,819,654,3);
    
    char posText[32];
    
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
                    }
                    if (verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 2)) {
                        opcao_battle = 2; 
                    }
                    if (verificacao_de_area(&hitbox_para_iniciar_batalha, &p, 3)) {
                        opcao_battle = 3; 
                    }
                    DrawText("Batalha iniciada!", 10, 80, 20, BLACK);
                    opcao = 2; // Muda para a tela de batalha
                }
            }
            EndDrawing();
        }
        
        if (opcao==2){

            if (opcao_battle == 1) {
                batalha(&battle_player,backgroud_sprite,henchList,henchman_anim,&boss, &direcao);
            }
            if (opcao_battle == 2) {
                batalha(&battle_player,backgroud_sprite,henchList,henchman_anim,&boss, &direcao);
            }
            if (opcao_battle == 3) {
                batalha(&battle_player,backgroud_sprite,henchList,henchman_anim,&boss, &direcao);
            }
            if (IsKeyPressed(KEY_M) || battle_player.hp <= 0) {
                opcao = 1; // Volta para a tela do mapa
            }
        }
    }   


    // limpeza final
    descarregar_texturas();
    // descarrega todas as animacoes de batalha compartilhadas.
    descarregar_animacoes_batalha();
    UnloadTexture(fundo);
    descarregar_menu();
    CloseWindow();
    return 0;
    
}


