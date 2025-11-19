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



// Pools de projeteis do jogador e do chefe para reutilizacao.
static Projectile projectiles[MAX_PROJECTILES];
static Projectile boss_projectiles[MAX_PROJECTILES];

// Loop principal de uma batalha completa (UI, entradas e atualizacoes).

void batalha(Personagem_em_batalha *player,Texture2D back,henchman *list,BattleAnimation *henchman_anim,Personagem_em_batalha *boss, int *direcao) {
    static bool battle_initialized = false;
    static WaveManager waveManager;
    static float bossAttackTimer = 2.0f; // Chefe ataca a cada 2 segundos
    BattleAnimation *player_projectile_anim = obter_battle_animation(BATTLE_ANIM_PROJECTILE_PLAYER);
    BattleAnimation *boss_projectile_anim = obter_battle_animation(BATTLE_ANIM_PROJECTILE_BOSS);
    
    // Isso garante que eles sejam criados apenas uma vez por batalha.
    if (!battle_initialized) {
        memset(list, 0, sizeof(henchman) * MAX_HENCH); 
        memset(projectiles, 0, sizeof(projectiles)); 
        memset(boss_projectiles, 0, sizeof(boss_projectiles)); 
        init_wave_manager(&waveManager);
        battle_initialized = true;
    }

    
    BeginDrawing();

    ClearBackground((Color){ 20, 20, 30, 255 });// colocar o fundo é so desenhar o parametro BACK
    

    DrawText("Batalha em andamento... Pressione M para voltar ao mapa.", 10, 10, 20, RAYWHITE);
    DrawText("Clique com o botao esquerdo para atirar.", 10, 40, 20, RAYWHITE);
    char waveText[32];
    sprintf(waveText, "Horda: %d", waveManager.wave);
    DrawText(waveText, 1180, 10, 20, RAYWHITE);

    if (boss->active) boss_movement(boss, direcao);
    mov_battle(player);

    // Lógica de ataque do chefe
    if (boss->active) {
        bossAttackTimer -= GetFrameTime();
        if (bossAttackTimer <= 0) {
            spawn_projectile_boss(boss_projectiles, player, boss, boss_projectile_anim);
            bossAttackTimer = 2.0f; 
        }
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        spawn_projectile(projectiles, player, mouse, player_projectile_anim);
    }
 
    if (waveManager.wave >= 3 && !boss->active) {
        boss->active = true;
    }

    // 1. Atualiza posições e desenha tudo
    update_wave(&waveManager, list, henchman_anim, GetScreenWidth(), GetScreenHeight(),player,boss);
    update_and_draw_projectiles(projectiles, GetScreenWidth(), GetScreenHeight());
    update_and_draw_henchmen(list,player);
    if (boss->active) update_and_draw_projectiles(boss_projectiles, GetScreenWidth(), GetScreenHeight());
    


    // 2. Resolve todas as colisões após as atualizações de movimento
    wizard_x_henchman_collisions(player, list);
    handle_projectile_enemy_collisions(projectiles, list);
    if (boss->active) {
        Collision_boss_projectile(boss, projectiles,5);      // Projéteis do JOGADOR acertando o CHEFE
        Collision_boss_projectile(player, boss_projectiles,10); // Projéteis do CHEFE acertando o JOGADOR
    }
    DrawRectangle(10,70,player->hp,10,GREEN);

    EndDrawing();

    // Se o jogador sair da batalha (pressionando M), resetamos o estado para a próxima batalha.
    if (IsKeyPressed(KEY_M)) {
        battle_initialized = false;
    }
}



