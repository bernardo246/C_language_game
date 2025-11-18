#ifndef battle_battle_h
#define battle_battle_h

#include <raylib.h>
#include <math.h>
#include "./battlefunctions.h"
void batalha(Personagem_em_batalha *player,Texture2D back,henchman *list,Texture2D sprite_henchman,Personagem_em_batalha *boss, int *direcao,Texture2D boss_texture_projectile);
#endif