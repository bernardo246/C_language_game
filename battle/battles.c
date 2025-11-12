// primeira batalha
#include "battlefunctions.h"
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "./battles.h"

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



void batalha1(Personagem_em_batalha *player) {
    init_projectile_resources();

    // Cena completa da batalha: fundo simples + instruções.
    BeginDrawing();

    ClearBackground((Color){ 20, 20, 30, 255 }); // temporario, apenas enquanto nao tem fundo de batalha

    DrawText("Batalha em andamento... Pressione M para voltar ao mapa.", 10, 10, 20, RAYWHITE);
    DrawText("Clique com o botao esquerdo para atirar.", 10, 40, 20, RAYWHITE);

    // Personagem gira e se move antes de desenharmos os projéteis.
    mov_battle(player);

    // Dispara sempre que o jogador clicar; reaproveitamos o primeiro slot livre.
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        spawn_projectile(projectiles, player, mouse, projectileTexture);
    }

    // Atualiza e desenha todos os projéteis ativos do pool.
    update_and_draw_projectiles(projectiles, GetScreenWidth(), GetScreenHeight());

    EndDrawing();
}
