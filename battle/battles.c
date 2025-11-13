// primeira batalha
#include "battlefunctions.h"
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "./battles.h"
#define HENCHMAN_SCALE 0.10f
// Pool e textura compartilhados entre quadros para os projéteis do jogador.
static Projectile projectiles[MAX_PROJECTILES]; // lista de projéteis
static Texture2D projectileTexture = {0}; // so a textura do projétil (temporario)
static bool projectilesInitialized = false;


// serve apenas para inicialiazar as texturas dos projeteis
static void init_projectile_resources(void) {
    if (projectilesInitialized) return;

    // Zera o pool a cada inicialização para evitar lixo visual ao reentrar na batalha.
    memset(projectiles, 0, sizeof(projectiles));

    Image bulletImage = GenImageColor(12, 12, (Color){255, 220, 120, 255});
    projectileTexture = LoadTextureFromImage(bulletImage);
    UnloadImage(bulletImage);

    projectilesInitialized = true;
}



void batalha(Personagem_em_batalha *player,Texture2D back,henchman *list,Texture2D sprite_henchman) {
    static bool battle_initialized = false;

    
    // Isso garante que eles sejam criados apenas uma vez por batalha.
    if (!battle_initialized) {
        memset(list, 0, sizeof(henchman) * MAX_HENCH); // Limpa a lista para garantir slots vazios
        spawn_henchman_offscreen(list, sprite_henchman, 150.0f, 6, 20, 1280, 720); // Gera capangas com velocidade maior
        battle_initialized = true;
    }

    init_projectile_resources();

    
    BeginDrawing();

    ClearBackground((Color){ 20, 20, 30, 255 });// colocar o fundo é so desenhar o parametro BACK
    

    DrawText("Batalha em andamento... Pressione M para voltar ao mapa.", 10, 10, 20, RAYWHITE);
    DrawText("Clique com o botao esquerdo para atirar.", 10, 40, 20, RAYWHITE);

    
    mov_battle(player);

    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        spawn_projectile(projectiles, player, mouse, projectileTexture);
    }

    // Atualiza e desenha todos os projéteis ativos do pool.
    update_and_draw_projectiles(projectiles, GetScreenWidth(), GetScreenHeight());
    update_and_draw_henchmen(list,player);
    //check collisions
    handle_projectile_enemy_collisions(projectiles,list);
    wizard_x_henchman_collisions(player,list);


    EndDrawing();

    // Se o jogador sair da batalha (pressionando M), resetamos o estado para a próxima batalha.
    if (IsKeyPressed(KEY_M)) {
        battle_initialized = false;
    }
}
