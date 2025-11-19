#include "animacoes.h"

#include <math.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

static const char *player_mago_frames[] = {
    "img/battle/player/leste1.png",
    "img/battle/player/leste2.png",
    "img/battle/player/leste3.png",
    "img/battle/player/leste4.png"
};

static const char *hench_monstro_pedra_frames[] = {
    "img/battle/monstros/monstro_pedra/mp1.png",
    "img/battle/monstros/monstro_pedra/mp2.png",
    "img/battle/monstros/monstro_pedra/mp3.png",
    "img/battle/monstros/monstro_pedra/mp4.png"
};

static const char *boss_monstro_fogo_frames[] = {
    "img/battle/monstros/monstro_fogo/mf1.png"
};

static const char *projectile_player_frames[] = {
    "img/battle/projeteis/mago_principal/p1.png",
    "img/battle/projeteis/mago_principal/p2.png"
};

static const char *projectile_boss_frames[] = {
    "img/battle/projeteis/mago_pedra/p1.png",
    "img/battle/projeteis/mago_pedra/p2.png"
};

// Cache simples para cada id de animacao.
static BattleAnimation *animation_cache[BATTLE_ANIM_COUNT] = {0};

static BattleAnimation *carregar_animacao_por_id(BattleAnimationId id) {
    switch (id) {
        case BATTLE_ANIM_PLAYER_MAGO:
            return criar_battle_animation(player_mago_frames, 4, 70.0);
        case BATTLE_ANIM_HENCH_MONSTRO_PEDRA:
            return criar_battle_animation(hench_monstro_pedra_frames, 1, 150.0);
        case BATTLE_ANIM_BOSS_MONSTRO_FOGO:
            return criar_battle_animation(boss_monstro_fogo_frames, 1, 200.0);
        case BATTLE_ANIM_PROJECTILE_PLAYER:
            return criar_battle_animation(projectile_player_frames, 2, 80.0);
        case BATTLE_ANIM_PROJECTILE_BOSS:
            return criar_battle_animation(projectile_boss_frames, 2, 80.0);
        default:
            return NULL;
    }
}

// Carrega os frames definidos em frame_paths e prepara a animacao.
BattleAnimation *criar_battle_animation(const char *frame_paths[], int frame_count, double intervalo_ms) {
    if (frame_paths == NULL || frame_count <= 0 || intervalo_ms <= 0.0) {
        return NULL;
    }

    BattleAnimation *anim = (BattleAnimation *)malloc(sizeof(BattleAnimation));
    if (!anim) {
        return NULL;
    }

    anim->frames = (Texture2D *)malloc(sizeof(Texture2D) * frame_count);
    if (!anim->frames) {
        free(anim);
        return NULL;
    }

    memset(anim->frames, 0, sizeof(Texture2D) * frame_count);
    for (int i = 0; i < frame_count; i++) {
        anim->frames[i] = LoadTexture(frame_paths[i]);
    }

    anim->frame_count = frame_count;
    anim->frame_atual = 0;
    anim->intervalo_ms = intervalo_ms;
    anim->acumulado_ms = 0.0;

    return anim;
}

// Helper para converter o delta do Raylib (s) para ms.
static inline double get_delta_ms(void) {
    return GetFrameTime() * 1000.0;
}

// Avanca frames quando o personagem esta em movimento.
void atualizar_battle_animation(BattleAnimation *anim, bool personagem_em_movimento) {
    if (!anim || anim->frame_count <= 0) {
        return;
    }

    if (!personagem_em_movimento) {
        anim->frame_atual = 0;
        anim->acumulado_ms = 0.0;
        return;
    }

    anim->acumulado_ms += get_delta_ms();
    if (anim->acumulado_ms < anim->intervalo_ms) {
        return;
    }

    int frames_to_advance = (int)(anim->acumulado_ms / anim->intervalo_ms);
    anim->frame_atual = (anim->frame_atual + frames_to_advance) % anim->frame_count;
    anim->acumulado_ms = fmod(anim->acumulado_ms, anim->intervalo_ms);
}

// Retorna o frame atual ou um Texture2D zerado.
Texture2D battle_animation_get_frame(const BattleAnimation *anim) {
    if (!anim || anim->frame_count <= 0) {
        return (Texture2D){0};
    }

    return anim->frames[anim->frame_atual];
}

// Descarta texturas carregadas e libera memoria da animacao.
void descarregar_battle_animation(BattleAnimation *anim) {
    if (!anim) {
        return;
    }

    if (anim->frames) {
        for (int i = 0; i < anim->frame_count; i++) {
            if (anim->frames[i].id != 0) {
                UnloadTexture(anim->frames[i]);
            }
        }
        free(anim->frames);
    }

    free(anim);
}

// Retorna (e carrega, se preciso) a animacao correspondente ao id solicitado.
BattleAnimation *obter_battle_animation(BattleAnimationId id) {
    if (id < 0 || id >= BATTLE_ANIM_COUNT) {
        return NULL;
    }
    if (!animation_cache[id]) {
        animation_cache[id] = carregar_animacao_por_id(id);
    }
    return animation_cache[id];
}

// Libera todas as animacoes armazenadas no cache.
void descarregar_animacoes_batalha(void) {
    for (int i = 0; i < BATTLE_ANIM_COUNT; i++) {
        if (animation_cache[i]) {
            descarregar_battle_animation(animation_cache[i]);
            animation_cache[i] = NULL;
        }
    }
}
