#ifndef battle_battle_h
#define battle_battle_h

#include <raylib.h>
#include <math.h>
#include "./battlefunctions.h"
// Executa a batalha completa ate o jogador sair ou vencer.



void reiniciar_batalha(henchman *list, Personagem_em_batalha *player, Personagem_em_batalha *boss, EntidadesBatalha *ent);
void batalha(Personagem_em_batalha *player,Texture2D back,henchman *list,EntidadesBatalha *ent, Personagem_em_batalha *boss, int *direcao);

#endif
