#ifndef batle_functions_h
#define batle_functions_h

#include <raylib.h>
#include <math.h>

#define MAX_PROJECTILES 16 
#define MAX_HENCH 64 
typedef struct pem{
    float x;
    float y;
    float speed;
    int hp;
    int damage;
    Texture2D t;
}Personagem_em_batalha;

typedef struct h{
    float x;
    float y;
    float speed;
    int hp;
    int damage;
    Texture2D t;
    int active;
}henchman;

typedef struct {
    float x;
    float y;
    float dx;       
    float dy;       
    float speed;
    Texture2D t;    
    int active;     
} Projectile;



void mov_battle(Personagem_em_batalha *p); // movimentaçao do personagem em batalha
void spawn_henchman_offscreen(henchman *henchList, Texture2D t, float speed, int hp, int damage, int screenWidth, int screenHeight); // cria henchman fora da tela
void spawn_projectile(Projectile *projList, Personagem_em_batalha *p, Vector2 mouse, Texture2D t); // cria projétil
void update_and_draw_projectiles(Projectile *projList, int screenWidth, int screenHeight); // atualiza e desenha projéteis
void update_and_draw_henchmen(henchman *henchList, Personagem_em_batalha *p); // atualiza e desenha henchmen
void handle_projectile_enemy_collisions(Projectile *projList, henchman *henchList); // colisao projétil+henchman
void wizard_x_henchman_collisions(Personagem_em_batalha *p, henchman *henchList); // colisao personagem+henchman


#endif
