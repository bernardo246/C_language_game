// primeira batalha
#include "battlefunctions.h"
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "./battles.h"
#include<stdio.h>



// Pools de projeteis do jogador e do chefe para reutilizacao (evita malloc em runtime).
static Projectile projectiles[MAX_PROJECTILES];
static Projectile boss_projectiles[MAX_PROJECTILES];
static WaveManager waveManager;
static float bossAttackTimer = 2.0f; // Chefe ataca a cada 2 segundos
static bool cursor_oculto = false;   // controla se o cursor padrao ja foi escondido

// Reinicia o estado completo da batalha para um novo começo.
void reiniciar_batalha(henchman *list, Personagem_em_batalha *player, Personagem_em_batalha *boss, EntidadesBatalha *ent) {
    memset(list, 0, sizeof(henchman) * MAX_HENCH);
    memset(projectiles, 0, sizeof(projectiles));
    memset(boss_projectiles, 0, sizeof(boss_projectiles));
    init_wave_manager(&waveManager);

    // Recarrega stats e animações a partir da configuração da batalha selecionada.
    player->hp = ent->player.hp;
    player->speed = ent->player.speed;
    player->damage = ent->player.damage;
    iniciar_animacao_estado(&player->anim, &ent->player.animacao);

    boss->hp = ent->boss.hp;
    boss->speed = ent->boss.speed;
    boss->damage = ent->boss.damage;
    iniciar_animacao_estado(&boss->anim, &ent->boss.animacao);
    boss->active = 0; // Garante que o boss comece inativo.
}

// Loop principal de uma batalha completa (UI, entradas e atualizacoes).
// Loop de uma batalha completa: desenha HUD, lida com entrada, spawns, movimentos e colisões.
void batalha(Personagem_em_batalha *player,Texture2D back,henchman *list,EntidadesBatalha *ent, Personagem_em_batalha *boss, int *direcao,bool vitoria) {
    if (!ent) return;
    DadosProjetil *player_projectile = &ent->projetil_player;
    DadosProjetil *boss_projectile = &ent->projetil_boss;

    // Esconde o cursor nativo para ficar apenas a mira customizada desenhada em battle/animacoes.c
    if (!cursor_oculto) {
        HideCursor();
        cursor_oculto = true;
    }

    // O fundo agora é desenhado no loop principal em main.c
    DrawTexture(back, 0, 0, WHITE);

    DrawText("Batalha em andamento... Pressione M para voltar ao mapa.", 10, 10, 20, RAYWHITE);
    DrawText("Clique com o botao esquerdo para atirar.", 10, 40, 20, RAYWHITE);
    char waveText[32];
    sprintf(waveText, "Horda: %d", waveManager.wave);
    DrawText(waveText, 1180, 10, 20, RAYWHITE);

    // Se a batalha foi vencida, mostra o menu de vitória em vez do jogo
    if (waveManager.wave > 3) {
        mostrar_mira(list); // Continua mostrando a mira para clicar no botão
        desenhar_menu_vitoria(); // A ação do botão será tratada no main.c
        vitoria = true;
        return; // Interrompe a execução do resto da lógica da batalha
    }
    if (player->hp <=0){
        mostrar_mira(list); // Continua mostrando a mira para clicar no botão
        desenhar_menu_derrota();
        return;
    }

    if (boss->active) boss_movement(boss, direcao);
    mov_battle(player);

    // Lógica de ataque do chefe
    if (boss->active) {
        bossAttackTimer -= GetFrameTime();
        if (bossAttackTimer <= 0) {
            spawn_projectile_boss(boss_projectiles, player, boss, boss_projectile);
            bossAttackTimer = 2.0f; 
        }
    }

    // mira desenhada depois dos sprites, ver final da funcao
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        static Sound shootsound = {0};
        static bool shootsound_loaded = false;

        // carregando o som apenas uma vez
        if (!shootsound_loaded) {
            shootsound = LoadSound("som/efeitos/tiro.wav");
            shootsound_loaded = true;
        }
        
        // tocando o som
        if (shootsound_loaded) {
            PlaySound(shootsound);
        }
        
        // disparando o projetil
        Vector2 mouse = GetMousePosition();
        spawn_projectile(projectiles, player, mouse, player_projectile);
    }
 
    if (waveManager.wave >= 3 && !boss->active) {
        boss->active = true;
    }

    // 1. Atualiza posições e desenha tudo
    update_wave(&waveManager, list, &ent->hench, GetScreenWidth(), GetScreenHeight(),player,boss);
    update_and_draw_projectiles(projectiles, GetScreenWidth(), GetScreenHeight());
    update_and_draw_henchmen(list,player);
    if (boss->active) update_and_draw_projectiles(boss_projectiles, GetScreenWidth(), GetScreenHeight());
    


    // 2. Resolve todas as colisões após as atualizações de movimento
    wizard_x_henchman_collisions(player, list);
    handle_projectile_enemy_collisions(projectiles, list);
    if (boss->active) {
        Collision_boss_projectile(boss, projectiles,0);      // Usa o dano do projetil do jogador
        Collision_boss_projectile(player, boss_projectiles,0); // Usa o dano do projetil do chefe
    }
    mostrar_mira(list);
    DrawRectangle(10,70,player->hp,10,GREEN);
    if(boss->active)DrawRectangle(10,90,boss->hp,10,RED);
    

}
