#ifndef batle_functions_h
#define batle_functions_h

#include <raylib.h>
#include <math.h>
#include "animacoes.h"
#include "entidades.h"

#define MAX_PROJECTILES 16
#define MAX_HENCH 64

// Personagem controlado pelo jogador (ou boss) na batalha.
typedef struct pem{
    float x;
    float y;
    float speed;
    int hp;
    int damage;
    int active;
    AnimacaoEstado anim; // estado da animacao (frame atual e tempo)
}Personagem_em_batalha;

// Capanga controlado pelo sistema.
typedef struct h{
    float x;
    float y;
    float speed;
    int hp;
    int damage;
    AnimacaoEstado anim; // estado da animacao
    int active;
}henchman;

typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    float speed;
    AnimacaoEstado anim; // animacao do projetil
    float angle_deg;
    int damage;
    int active;
} Projectile;

typedef struct {
    int wave;
    int enemiesToSpawn;
    float spawnRate;
    float spawnTimer;
    int activeEnemies;
} WaveManager;

void mov_battle(Personagem_em_batalha *p);
void spawn_henchman_offscreen(henchman *henchList, DadosEntidade *hench_dados, int screenWidth, int screenHeight);
void spawn_projectile(Projectile *projList, Personagem_em_batalha *p, Vector2 mouse, DadosProjetil *dados_proj);
void update_and_draw_projectiles(Projectile *projList, int screenWidth, int screenHeight);
void update_and_draw_henchmen(henchman *henchList, Personagem_em_batalha *p);
void handle_projectile_enemy_collisions(Projectile *projList, henchman *henchList);
void wizard_x_henchman_collisions(Personagem_em_batalha *p, henchman *henchList);
void init_wave_manager(WaveManager *waveManager);
void update_wave(WaveManager *waveManager, henchman *henchList, DadosEntidade *hench_dados, int screenWidth, int screenHeight,Personagem_em_batalha *p,Personagem_em_batalha *boss);
int count_active_henchmen(henchman *henchList);
void boss_movement(Personagem_em_batalha *p, int *direcao);
void Collision_boss_projectile(Personagem_em_batalha *p, Projectile *projList,int damage);
void spawn_projectile_boss(Projectile *projList, Personagem_em_batalha *p,Personagem_em_batalha *boss, DadosProjetil *dados_proj);

void desenhar_menu_vitoria(void);

#endif
