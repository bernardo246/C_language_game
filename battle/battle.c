#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>


typedef struct pem{
    float x=640;
    float y=360
    float speed;
    int hp = 100;
    int damage =10;
    Texture t;
}Personagem_em batalha;

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

float angle;


void mov_battle(Personagem_em_batalha *p)
{
    Vector2 mouse = GetMousePosition();

    //ve o angulo
    float angle = atan2(mouse.y - p->y, mouse.x - p->x);

    if (IsKeyDown(KEY_W)) p->y -= p->speed;
    if (IsKeyDown(KEY_S)) p->y += p->speed;
    if (IsKeyDown(KEY_A)) p->x -= p->speed;
    if (IsKeyDown(KEY_D)) p->x += p->speed;

    // --- Desenho do personagem rotacionado ---
    DrawTexturePro(
        p->t,
        (Rectangle){0, 0, p->t.width, p->t.height},              
        (Rectangle){p->x, p->y, p->t.width, p->t.height},        
        (Vector2){p->t.width / 2.0f, p->t.height / 2.0f},        
        angle * RAD2DEG,                                        
        WHITE
    );
}


void spawn_henchman_offscreen(henchman *h, Texture t, float speed, int hp, int damage, int screenWidth, int screenHeight) {
    h->t = t;
    h->speed = speed;
    h->hp = hp;
    h->damage = damage;
    h->active = 1;

    int side = rand() % 4;

    switch (side) {
        case 0: // esquerda
            h->x = -t.width;
            h->y = (float)(rand() % screenHeight);
            break;
        case 1: // direita
            h->x = screenWidth + t.width;
            h->y = (float)(rand() % screenHeight);
            break;
        case 2: // cima
            h->x = (float)(rand() % screenWidth);
            h->y = -t.height;
            break;
        case 3: // baixo
            h->x = (float)(rand() % screenWidth);
            h->y = screenHeight + t.height;
            break;
    }
}


// Cria um novo henchman se houver espaço livre
/*
void spawn_new_henchman(Texture t, float speed, int hp, int damage, int screenWidth, int screenHeight) {
    for (int i = 0; i < MAX_HENCHMEN; i++) {
        if (!henchmen[i].active) { // encontra um slot livre
            spawn_henchman_offscreen(&henchmen[i], t, speed, hp, damage, screenWidth, screenHeight);
            break; // sai do loop após criar um
        }
    }
}
*/

// Move e desenha um único henchman
void mov_henchman(henchman *h, Personagem_em_batalha *p) {
    // Calcula direção
    Vector2 dir = { p->x - h->x, p->y - h->y };

    float dis_btw_h_x_p = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (dis_btw_h_x_p > 0.01f) {
        dir.x /= dis_btw_h_x_p;
        dir.y /= dis_btw_h_x_p;
    }

    // Move
    h->x += dir.x * h->speed;
    h->y += dir.y * h->speed;

    // Calcula ângulo
    float angle = atan2(p->y - h->y, p->x - h->x);

    // Desenha
    DrawTexturePro(
        h->t,
        (Rectangle){0, 0, h->t.width, h->t.height},
        (Rectangle){h->x, h->y, h->t.width, h->t.height},
        (Vector2){h->t.width / 2.0f, h->t.height / 2.0f},
        angle * RAD2DEG,
        WHITE
    );
}


// Atualiza todos os henchmen ativos
void update_henchmen(Personagem_em_batalha *p) {
    for (int i = 0; i < MAX_HENCHMEN; i++) {
        if (henchmen[i].active) {
            mov_henchman(&henchmen[i], p);

            // Exemplo: desativa se morrer
            if (henchmen[i].hp <= 0) {
                henchmen[i].active = 0;
            }
        }
    }
}

Projectile* spawn_projectile(Personagem_em_batalha *p, Vector2 mouse, Texture2D t) {// alterar para gerenciar para multiplos disparos
    Projectile *proj = (Projectile*)malloc(sizeof(Projectile));
    if(!proj) return NULL; 

    proj->x = p->x;
    proj->y = p->y;

    float dx = mouse.x - p->x;
    float dy = mouse.y - p->y;
    float distance = sqrtf(dx*dx + dy*dy);

    if(distance > 0.01f){
        proj->dx = dx / distance;
        proj->dy = dy / distance;
    } else {
        proj->dx = 0;
        proj->dy = 0;
    }

    proj->speed = 10.0f;
    proj->t = t;
    proj->active = 1;

    return proj;
}


void mov_projectile(Projectile **proj) { //colocar a condicao de dano aq dps
    if(*proj && (*proj)->active){
        (*proj)->x += (*proj)->dx * (*proj)->speed;
        (*proj)->y += (*proj)->dy * (*proj)->speed;

        DrawTextureV((*proj)->t, (Vector2){(*proj)->x, (*proj)->y}, WHITE);

        // Se sair da tela, libera memória
        if((*proj)->x < 0 || (*proj)->x > 1280 || (*proj)->y < 0 || (*proj)->y > 720){
            free(*proj);
            *proj = NULL; // evita ponteiro pendurado
        }
    }
}



