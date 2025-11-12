#ifndef batle_functions_h
#define batle_functions_h

#include <raylib.h>
#include <math.h>

typedef struct pem{
    float x;
    float y;
    float speed;
    int hp;
    int damage;
    Texture t;
}Personagem_em_batalha;

typedef struct h{
    float x;
    float y;
    float speed;
    int hp;
    int damage;
    Texture t;
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



void mov_battle(Personagem_em_batalha *p);
void spawn_henchman_offscreen(henchman *henchList, Texture t, float speed, int hp, int damage, int screenWidth, int screenHeight);
void spawn_projectile(Projectile *projList, Personagem_em_batalha *p, Vector2 mouse, Texture2D t);
void update_and_draw_projectiles(Projectile *projList, int screenWidth, int screenHeight);
void update_and_draw_henchmen(henchman *henchList, Personagem_em_batalha *p);

#endif
