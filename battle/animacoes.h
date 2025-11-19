#ifndef BATTLE_ANIMACOES_H
#define BATTLE_ANIMACOES_H

#include <raylib.h>
#include <stdbool.h>

// Identificadores das animacoes compartilhadas na batalha.
typedef enum {
    BATTLE_ANIM_PLAYER_MAGO = 0,
    BATTLE_ANIM_HENCH_MONSTRO_PEDRA,
    BATTLE_ANIM_BOSS_MONSTRO_FOGO,
    BATTLE_ANIM_PROJECTILE_PLAYER,
    BATTLE_ANIM_PROJECTILE_BOSS,
    BATTLE_ANIM_COUNT
} BattleAnimationId;

// Estrutura basica para gerenciar animacao em batalha.
typedef struct BattleAnimation {
    Texture2D *frames;
    int frame_count;
    int frame_atual;
    double intervalo_ms;
    double acumulado_ms;
} BattleAnimation;

// Utilitarios basicos (tambem usados pelo gerenciador)
BattleAnimation *criar_battle_animation(const char *frame_paths[], int frame_count, double intervalo_ms);
void atualizar_battle_animation(BattleAnimation *anim, bool personagem_em_movimento);
Texture2D battle_animation_get_frame(const BattleAnimation *anim);
void descarregar_battle_animation(BattleAnimation *anim);

// API do gerenciador centralizado
BattleAnimation *obter_battle_animation(BattleAnimationId id);
void descarregar_animacoes_batalha(void);

#endif
