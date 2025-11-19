#ifndef battle_battle_h
#define battle_battle_h

#include <raylib.h>
#include <math.h>
#include "./battlefunctions.h"
// Executa a batalha completa ate o jogador sair ou vencer.
void batalha(Personagem_em_batalha *player,Texture2D back,henchman *list,BattleAnimation *henchman_anim,Personagem_em_batalha *boss, int *direcao);
#endif
