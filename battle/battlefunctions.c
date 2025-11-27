#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <stdbool.h>
#include "battlefunctions.h"

float angle;
// Escala unica aplicada aos sprites dos capangas na batalha
static const float HENCHMAN_SCALE = 0.10f;          // escala visual
static const float HENCHMAN_COLLISION_SCALE = 0.08f;
static const float PLAYER_COLLISION_SCALE = 0.08f;
static const float BOSS_COLLISION_SCALE = 0.08f;
static const float BOSS_DRAW_SCALE = 0.2f; // Escala para desenhar o boss. Ajuste conforme necessário.
static const float PROJECTILE_SCALE = 0.10f;

// Calcula o raio aproximado de um capanga usando a textura escalonada
static float get_henchman_radius(const henchman *h) {
    Texture2D frame = animacao_frame_atual(&h->anim);
    if (frame.id == 0) {
        return 0.0f;
    }
    float width = frame.width * HENCHMAN_COLLISION_SCALE;
    float height = frame.height * HENCHMAN_COLLISION_SCALE;
    float diameter = fmaxf(width, height);
    return diameter * 0.4f;
}




// Separa capangas que se sobrepoem para evitar empilhamento.
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

// Atualiza posição, animação e desenha o jogador na batalha.
// Passos: ler input, mover mantendo limites da tela, avançar animação e desenhar rotacionado para o mouse.
void mov_battle(Personagem_em_batalha *p)
{
    Vector2 mouse = GetMousePosition();
    float dt = GetFrameTime();
    bool personagem_em_movimento = false;

    float angle = atan2(mouse.y - p->y, mouse.x - p->x); // a funcao atan2 retorna o angulo em radianos

    if (IsKeyDown(KEY_W)) {
        p->y -= p->speed * dt;
        personagem_em_movimento = true;
    }
    if (IsKeyDown(KEY_S)) {
        p->y += p->speed * dt;
        personagem_em_movimento = true;
    }
    if (IsKeyDown(KEY_A)) {
        p->x -= p->speed * dt;
        personagem_em_movimento = true;
    }
    if (IsKeyDown(KEY_D)) {
        p->x += p->speed * dt;
        personagem_em_movimento = true;
    }

    if(p->x<0)p->x=0;
    if(p->y<0)p->y=0;
    if(p->x>1280)p->x=1280;
    if(p->y>720)p->y=720;

    atualizar_animacao_estado(&p->anim, personagem_em_movimento);
    Texture2D frame = animacao_frame_atual(&p->anim);
    if (frame.id == 0) {
        return;
    }

    // Desenha o personagem rotacionado para olhar na direcao do mouse
    const float scale = 0.10f; 
    Rectangle src = {0, 0, frame.width, frame.height}; 
    Rectangle dest = {p->x, p->y, frame.width * scale, frame.height * scale};
    Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};

    DrawTexturePro(frame, src, dest, origin, angle * RAD2DEG, WHITE); // converte radianos para graus
}



// Gera capanga fora da tela usando a animacao fornecida.
// Cria e posiciona um capanga fora da tela, ligando-o à animação compartilhada.
void spawn_henchman_offscreen(henchman *henchList, DadosEntidade *hench_dados, int screenWidth, int screenHeight) {
    // procura um slot livre
    if (!hench_dados) return;
    for (int i = 0; i < MAX_HENCH; i++) {
        if (!henchList[i].active) {
            iniciar_animacao_estado(&henchList[i].anim, &hench_dados->animacao);
            henchList[i].speed = hench_dados->speed;
            henchList[i].hp = hench_dados->hp;
            henchList[i].damage = hench_dados->damage;
            henchList[i].active = 1;

            int side = rand() % 4;

            // Usa a largura e altura escalonadas para posicionar o capanga um pouco fora da tela.
            Texture2D frame = animacao_frame_atual(&henchList[i].anim);
            float scaledWidth = frame.width * HENCHMAN_SCALE;
            float scaledHeight = frame.height * HENCHMAN_SCALE;
            switch (side) {
                case 0: // esquerda
                    henchList[i].x = -scaledWidth;
                    henchList[i].y = (float)(rand() % screenHeight);
                    break;
                case 1: // direita
                    henchList[i].x = screenWidth + scaledWidth;
                    henchList[i].y = (float)(rand() % screenHeight);
                    break;
                case 2: // cima
                    henchList[i].x = (float)(rand() % screenWidth);
                    henchList[i].y = -scaledHeight;
                    break;
                case 3: // baixo
                    henchList[i].x = (float)(rand() % screenWidth);
                    henchList[i].y = screenHeight + scaledHeight;
                    break;
            }
            break; 
        }
    }
}




// Move capangas em direcao ao jogador, resolve empurrões entre eles e desenha apontando para o player.
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
        atualizar_animacao_estado(&h->anim, true);
    }

    // Segunda etapa: separa capangas que encostaram entre si
    resolve_henchman_collisions(henchList);

    for (int i = 0; i < MAX_HENCH; i++) {
        if (!henchList[i].active)
            continue;

        henchman *h = &henchList[i];

        // Calcula o angulo para desenhar o sprite apontado para o jogador
        float angle = atan2f(p->y - h->y, p->x - h->x);
        Texture2D frame = animacao_frame_atual(&h->anim);
        if (frame.id == 0) {
            continue;
        }
        Rectangle src = {0, 0, frame.width, frame.height};
        Rectangle dest = {h->x, h->y, frame.width * HENCHMAN_SCALE, frame.height * HENCHMAN_SCALE};
        Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};

        DrawTexturePro(
            frame,
            src,
            dest,
            origin,
            angle * RAD2DEG,
            WHITE
        );
    }
}


// Reaproveita um slot livre de projétil, aponta para o mouse e liga à animação compartilhada.
void spawn_projectile(Projectile *projList, Personagem_em_batalha *p, Vector2 mouse, DadosProjetil *dados_proj) {
    if (!dados_proj) return;
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

            projList[i].speed = dados_proj->speed; 
            iniciar_animacao_estado(&projList[i].anim, &dados_proj->animacao);
            projList[i].angle_deg = atan2f(projList[i].dy, projList[i].dx) * RAD2DEG;
            projList[i].damage = dados_proj->damage;
            projList[i].active = 1;

            break; 
        }
    }
}
// wave system
void init_wave_manager(WaveManager *waveManager) {
    waveManager->wave = 1;
    waveManager->enemiesToSpawn = 3; 
    waveManager->spawnRate = 1.5f;   
    waveManager->spawnTimer = 0.0f;
    waveManager->activeEnemies = 0;
}

int count_active_henchmen(henchman *henchList) {
    int count = 0;
    for (int i = 0; i < MAX_HENCH; i++) {
        if (henchList[i].active) {
            count++;
        }
    }
    return count;
}

// Gerencia hordas: controla timers de spawn, avança onda e libera o boss na onda 3.
void update_wave(WaveManager *waveManager, henchman *henchList, DadosEntidade *hench_dados, int screenWidth, int screenHeight, Personagem_em_batalha *p,Personagem_em_batalha *boss) {
    waveManager->activeEnemies = count_active_henchmen(henchList);

    // Se a horda do boss (3) já passou e o boss foi derrotado, a batalha terminou.
    if (waveManager->wave > 3) {
        // A lógica de desenho da vitoria agora é tratada em battles.c
        return;
    }

    // Se estamos na horda do boss e ele foi derrotado, avançamos para a próxima horda.
    if (waveManager->wave == 3 && boss->active) {
        if(boss->hp<=0){
            waveManager->wave++;
            waveManager->spawnTimer = 0; 
            waveManager->enemiesToSpawn = 0;
            boss->active = 0;
            p->active = 1;
            p->hp = 100;
        }
        return;
    }

    // Se todos os inimigos da horda foram derrotados e não há mais para surgir, avança para a próxima horda
    if (waveManager->activeEnemies == 0 && waveManager->enemiesToSpawn == 0) {
        waveManager->wave++;
        waveManager->spawnTimer = 0; 

        
        if (waveManager->wave == 2) {
            waveManager->enemiesToSpawn = 7;
            waveManager->spawnRate = 1.0f;
        }
    }
    
    //logica de spawn
    if (waveManager->enemiesToSpawn > 0) {
        waveManager->spawnTimer += GetFrameTime();
        if (waveManager->spawnTimer >= waveManager->spawnRate) {
            waveManager->spawnTimer = 0;
            spawn_henchman_offscreen(henchList, hench_dados, screenWidth, screenHeight);
            waveManager->enemiesToSpawn--;
        }
    }
}

void desenhar_menu_vitoria(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // 1. Escurece o fundo para dar foco ao menu
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));

    // 2. Desenha o texto de vitoria
    const char *titulo = "VITORIA!";
    const int tamanho_titulo = 100;
    int largura_titulo = MeasureText(titulo, tamanho_titulo);// serve para sentralizar e otimizar a quantidade de pixels utilizados
    DrawText(titulo, (screenWidth - largura_titulo) / 2, screenHeight / 2 - 100, tamanho_titulo, GOLD);

    // 3. Desenha a instrução para voltar ao mapa
    const char *instrucao = "Pressione 'M' para voltar ao mapa";
    const int tamanho_instrucao = 30;
    int largura_instrucao = MeasureText(instrucao, tamanho_instrucao);
    DrawText(instrucao, (screenWidth - largura_instrucao) / 2, screenHeight / 2 + 50, tamanho_instrucao, WHITE);
}


//update projectile
void update_and_draw_projectiles(Projectile *projList, int screenWidth, int screenHeight) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projList[i].active || !projList[i].anim.dados) continue;

        // Move o projetil pela hip
        projList[i].x += projList[i].dx * projList[i].speed * GetFrameTime();
        projList[i].y += projList[i].dy * projList[i].speed * GetFrameTime();

        atualizar_animacao_estado(&projList[i].anim, true);
        Texture2D frame = animacao_frame_atual(&projList[i].anim);
        if (frame.id == 0) continue;

        Rectangle src = {0, 0, frame.width, frame.height};
        Rectangle dest = {
            projList[i].x,
            projList[i].y,
            (frame.width - 12 ) * PROJECTILE_SCALE,
            (frame.height - 12 ) * PROJECTILE_SCALE
        };
        Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};

        DrawTexturePro(frame, src, dest, origin, projList[i].angle_deg, WHITE);

        float boundX = dest.width;
        float boundY = dest.height;
        if (projList[i].x < -boundX || projList[i].x > screenWidth + boundX ||
            projList[i].y < -boundY || projList[i].y > screenHeight + boundY)
        {
            projList[i].active = 0;
        }
    }
}
// colisao de projetil+henchman
// Trata dano dos projeteis do jogador nos capangas.
void handle_projectile_enemy_collisions(Projectile *projList, henchman *henchList) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projList[i].active || !projList[i].anim.dados) continue;

        Texture2D projFrame = animacao_frame_atual(&projList[i].anim);
        if (projFrame.id == 0) continue;
        float projWidth = projFrame.width * PROJECTILE_SCALE;
        float projHeight = projFrame.height * PROJECTILE_SCALE;
        Rectangle projRect = {
            projList[i].x - (projWidth / 2.0f),
            projList[i].y - (projHeight / 2.0f),
            projWidth,
            projHeight
        };

        for (int j = 0; j < MAX_HENCH; j++) {
            if (!henchList[j].active) continue;
            Texture2D frame = animacao_frame_atual(&henchList[j].anim);
            if (frame.id == 0) continue;
            Rectangle henchRect = {
                henchList[j].x - (frame.width * HENCHMAN_COLLISION_SCALE / 2.0f),
                henchList[j].y - (frame.height * HENCHMAN_COLLISION_SCALE / 2.0f),
                (float)frame.width * HENCHMAN_COLLISION_SCALE,
                (float)frame.height * HENCHMAN_COLLISION_SCALE
            };

            if (CheckCollisionRecs(projRect, henchRect)) {
                // Colisão detectada
                projList[i].active = 0;        
                henchList[j].hp -= projList[i].damage;          

                if (henchList[j].hp <= 0) {    
                    henchList[j].active = 0;             
                }

                break; 
            }
        }
    }
}

//interacao mago x capanga aqui quando o capanga atinge ele, o capanga desaparece 
// Detecta dano corpo a corpo entre jogador e capangas.
void wizard_x_henchman_collisions(Personagem_em_batalha *p, henchman *henchList) {
    Texture2D playerFrame = animacao_frame_atual(&p->anim);
    if (playerFrame.id == 0) {
        return;
    }
    for (int i = 0; i < MAX_HENCH; i++) {
        if (!henchList[i].active) continue;
        Rectangle playerRect = {
            p->x - (playerFrame.width * PLAYER_COLLISION_SCALE / 2.0f),
            p->y - (playerFrame.height * PLAYER_COLLISION_SCALE / 2.0f),
            (float)playerFrame.width * PLAYER_COLLISION_SCALE,
            (float)playerFrame.height * PLAYER_COLLISION_SCALE
        };
        Texture2D henchFrame = animacao_frame_atual(&henchList[i].anim);
        if (henchFrame.id == 0) continue;
        Rectangle henchmanRect = {
            henchList[i].x - (henchFrame.width * HENCHMAN_COLLISION_SCALE / 2.0f),
            henchList[i].y - (henchFrame.height * HENCHMAN_COLLISION_SCALE / 2.0f),
            (float)henchFrame.width * HENCHMAN_COLLISION_SCALE,
            (float)henchFrame.height * HENCHMAN_COLLISION_SCALE
        };
        if (CheckCollisionRecs(playerRect, henchmanRect)) {
            p->hp -= henchList[i].damage;
            henchList[i].active = 0;
        }
    }
}

// boss manager

// Movimento e animacao do boss que sobe/desce na tela.
void boss_movement(Personagem_em_batalha *p,int *direcao){
    bool em_movimento = true;
    if(*direcao == 1){
        p->y += p->speed * GetFrameTime();
    }else{
        p->y -= p->speed * GetFrameTime();
    }
    atualizar_animacao_estado(&p->anim, em_movimento);
    Texture2D frame = animacao_frame_atual(&p->anim);
    if (frame.id != 0) {
        // Usa DrawTexturePro para redimensionar o boss
        Rectangle src = {0, 0, (float)frame.width, (float)frame.height};
        Rectangle dest = {
            p->x,
            p->y,
            (float)frame.width * BOSS_DRAW_SCALE,
            (float)frame.height * BOSS_DRAW_SCALE};
        Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
        DrawTexturePro(frame, src, dest, origin, 0.0f, WHITE);
    }
    if(p->y<0){
        p->y=0;
        *direcao=1;
    }
    if(p->y>720){
        p->y=720;
        *direcao=0;
    };
}

// Colisoes genericas entre projeteis e o personagem informado (boss ou jogador).
void Collision_boss_projectile(Personagem_em_batalha *p, Projectile *projList,int damage){
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projList[i].active || !projList[i].anim.dados) continue;
        Texture2D projFrame = animacao_frame_atual(&projList[i].anim);
        if (projFrame.id == 0) continue;
        float projWidth = projFrame.width * PROJECTILE_SCALE;
        float projHeight = projFrame.height * PROJECTILE_SCALE;
        Rectangle projRect = {
            projList[i].x - (projWidth / 2.0f),
            projList[i].y - (projHeight / 2.0f),
            projWidth,
            projHeight
        };
        Texture2D frame = animacao_frame_atual(&p->anim);
        if (frame.id == 0) continue;
        Rectangle bossRect = {
            p->x - (frame.width * BOSS_COLLISION_SCALE / 2.0f),
            p->y - (frame.height * BOSS_COLLISION_SCALE / 2.0f),
            (float)frame.width * BOSS_COLLISION_SCALE,
            (float)frame.height * BOSS_COLLISION_SCALE
        };

        if (CheckCollisionRecs(projRect, bossRect)) {
            projList[i].active = 0;        
            int dano = (damage > 0) ? damage : projList[i].damage;
            p->hp -= dano;          
            if (p->hp <= 0) {    
                p->active = 0;             
            }
            break; 
        }
    }
}
    
void spawn_projectile_boss(Projectile *projList, Personagem_em_batalha *p,Personagem_em_batalha *boss, DadosProjetil *dados_proj) {
    if (!dados_proj) return;
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projList[i].active) {
            projList[i].x = boss->x;
            projList[i].y = boss->y;

            float dx = p->x - boss->x;
            float dy = p->y - boss->y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance > 0.01f) {
                projList[i].dx = dx / distance;
                projList[i].dy = dy / distance;
            } else {
                projList[i].dx = 0;
                projList[i].dy = 0;
            }

            projList[i].speed = dados_proj->speed;
            iniciar_animacao_estado(&projList[i].anim, &dados_proj->animacao);
            projList[i].angle_deg = atan2f(projList[i].dy, projList[i].dx) * RAD2DEG;
            projList[i].damage = dados_proj->damage;
            projList[i].active = 1;
            break;
        }
    }
}

