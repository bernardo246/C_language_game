#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include "battlefunctions.h"

float angle;
// Escala unica aplicada aos sprites dos capangas na batalha
static const float HENCHMAN_SCALE = 0.10f;

// Calcula o raio aproximado de um capanga usando a textura escalonada
static float get_henchman_radius(const henchman *h) {
    float width = h->t.width * HENCHMAN_SCALE;   // largura em tela
    float height = h->t.height * HENCHMAN_SCALE; // altura em tela
    float diameter = fmaxf(width, height);
    return diameter * 0.4f; //0,5 é o raio, 0.3f para ajuste visual
}


// Resolve colisoes entre capangas para evitar sobreposicao visivel
static void resolve_henchman_collisions(henchman *henchList) {
    for (int i = 0; i < MAX_HENCH; i++) {
        if (!henchList[i].active) continue;

        for (int j = i + 1; j < MAX_HENCH; j++) {
            if (!henchList[j].active) continue;

            // Vetor entre o centro dos dois capangas
            float dx = henchList[j].x - henchList[i].x;
            float dy = henchList[j].y - henchList[i].y;
            float radiusA = get_henchman_radius(&henchList[i]);
            float radiusB = get_henchman_radius(&henchList[j]);
            float minDistance = radiusA + radiusB;
            float distSq = dx * dx + dy * dy;

            if (distSq >= minDistance * minDistance) continue; // sem colisao

            if (distSq < 0.0001f) {
                dx = 1.0f; // evita divisao por zero quando estao exatamente juntos
                dy = 0.0f;
                distSq = 1.0f;
            }

            float dist = sqrtf(distSq);          // distancia real
            float overlap = minDistance - dist;  // quanto precisa separar
            float nx = dx / dist;                // normal unitaria
            float ny = dy / dist;
            float push = overlap * 0.5f;         // metade para cada capanga

            henchList[i].x -= nx * push;
            henchList[i].y -= ny * push;
            henchList[j].x += nx * push;
            henchList[j].y += ny * push;
        }
    }
}

void mov_battle(Personagem_em_batalha *p)
{
    Vector2 mouse = GetMousePosition();
    float dt = GetFrameTime(); // garante velocidade constante em FPS diferentes

    //ve o angulo
    float angle = atan2(mouse.y - p->y, mouse.x - p->x);

    if (IsKeyDown(KEY_W)) p->y -= p->speed * dt;
    if (IsKeyDown(KEY_S)) p->y += p->speed * dt;
    if (IsKeyDown(KEY_A)) p->x -= p->speed * dt;
    if (IsKeyDown(KEY_D)) p->x += p->speed * dt;

    // Desenho do personagem rotacionado em escala reduzida 
    const float scale = 0.10f; // fator único para encolher o sprite gigante
    Rectangle src = {0, 0, p->t.width, p->t.height};
    Rectangle dest = {p->x, p->y, p->t.width * scale, p->t.height * scale};
    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};

    DrawTexturePro(p->t, src, dest, origin, angle * RAD2DEG, WHITE);
}



void spawn_henchman_offscreen(henchman *henchList, Texture t, float speed, int hp, int damage, int screenWidth, int screenHeight) {
    // procura um slot livre
    int spaw=0;
    for (int i = 0; i < MAX_HENCH; i++) {
        if (!henchList[i].active) {
            henchList[i].t = t;
            henchList[i].speed = speed;
            henchList[i].hp = hp;
            henchList[i].damage = damage;
            henchList[i].active = 1;

            int side = rand() % 4;

            switch (side) {
                case 0: // esquerda
                    henchList[i].x = -t.width;
                    henchList[i].y = (float)(rand() % screenHeight);
                    break;
                case 1: // direita
                    henchList[i].x = screenWidth + t.width;
                    henchList[i].y = (float)(rand() % screenHeight);
                    break;
                case 2: // cima
                    henchList[i].x = (float)(rand() % screenWidth);
                    henchList[i].y = -t.height;
                    break;
                case 3: // baixo
                    henchList[i].x = (float)(rand() % screenWidth);
                    henchList[i].y = screenHeight + t.height;
                    break;
            }
            spaw++;
            if (spaw == 3){break;}
        }
    }
}




void update_and_draw_henchmen(henchman *henchList, Personagem_em_batalha *p) {
    float dt = GetFrameTime(); // dt unico garante movimento suave em FPS diferentes

    for (int i = 0; i < MAX_HENCH; i++) {
        if (!henchList[i].active)
            continue;

        henchman *h = &henchList[i];

        // Direcao normalizada apontando para o player
        Vector2 dir = { p->x - h->x, p->y - h->y };
        float dist = sqrtf(dir.x * dir.x + dir.y * dir.y);

        if (dist > 0.01f) {
            dir.x /= dist;
            dir.y /= dist;
        }

        // Move o capanga ate o player
        h->x += dir.x * h->speed * dt;
        h->y += dir.y * h->speed * dt;
    }

    // Segunda etapa: separa capangas que encostaram entre si
    resolve_henchman_collisions(henchList);

    for (int i = 0; i < MAX_HENCH; i++) {
        if (!henchList[i].active)
            continue;

        henchman *h = &henchList[i];

        // Calcula o angulo para desenhar o sprite apontado para o jogador
        float angle = atan2f(p->y - h->y, p->x - h->x);
        Rectangle src = {0, 0, h->t.width, h->t.height};
        Rectangle dest = {h->x, h->y, h->t.width * HENCHMAN_SCALE, h->t.height * HENCHMAN_SCALE};
        Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};

        DrawTexturePro(
            h->t,
            src,
            dest,
            origin,
            angle * RAD2DEG,
            WHITE
        );
    }
}


void spawn_projectile(Projectile *projList, Personagem_em_batalha *p, Vector2 mouse, Texture2D t) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projList[i].active) {
            projList[i].x = p->x;
            projList[i].y = p->y;

            float dx = mouse.x - p->x;
            float dy = mouse.y - p->y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance > 0.01f) {
                projList[i].dx = dx / distance;
                projList[i].dy = dy / distance;
            } else {
                projList[i].dx = 0;
                projList[i].dy = 0;
            }

            projList[i].speed = 800.0f; 
            projList[i].t = t;
            projList[i].active = 1;

            break; // cria apenas 1 por clique
        }
    }
}


void update_and_draw_projectiles(Projectile *projList, int screenWidth, int screenHeight) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projList[i].active) continue;

        // Move o projétil
        projList[i].x += projList[i].dx * projList[i].speed * GetFrameTime();
        projList[i].y += projList[i].dy * projList[i].speed * GetFrameTime();

        // Desenha o projétil
        DrawTextureV(projList[i].t, (Vector2){projList[i].x, projList[i].y}, WHITE);

        // Se sair da tela, desativa
        if (projList[i].x < -10 || projList[i].x > screenWidth + 10 ||
            projList[i].y < -10 || projList[i].y > screenHeight + 10) 
        {
            projList[i].active = 0;
        }
    }
}
// colisao de projetil+henchman
void handle_projectile_enemy_collisions(Projectile *projList, henchman *henchList) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projList[i].active) continue;

        Rectangle projRect = { projList[i].x, projList[i].y, (float)projList[i].t.width, (float)projList[i].t.height };

        for (int j = 0; j < MAX_HENCH; j++) {
            if (!henchList[j].active) continue;

            Rectangle henchRect = { henchList[j].x, henchList[j].y, (float)henchList[j].t.width, (float)henchList[j].t.height };

            if (CheckCollisionRecs(projRect, henchRect)) {
                // Colisão detectada
                projList[i].active = 0;        
                henchList[j].hp -= 1;          

                if (henchList[j].hp <= 0) {    
                    henchList[j].active = 0;             
                }

                break; 
            }
        }
    }
}
