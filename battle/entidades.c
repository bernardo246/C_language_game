// aqui todos os atributos de entidades de batalha vao ser definidos 

#include <string.h>
#include <stdbool.h>
#include "entidades.h"

// ---- Paths de sprites por batalha (edite aqui para trocar visuais) ----
// Batalha 1 (pedra)
static const char *PLAYER_B1_FRAMES[]      = {"img/battle/player/leste1.png", "img/battle/player/leste2.png", "img/battle/player/leste3.png", "img/battle/player/leste4.png"};
static const char *HENCH_B1_FRAMES[]       = {"img/battle/monstros/monstro_pedra/mp1.png"};
static const char *BOSS_B1_FRAMES[]        = {"img/battle/chefoes/mago_pedra.png"};
static const char *PROJ_PLAYER_B1_FRAMES[] = {"img/battle/projeteis/mago_principal/p1.png", "img/battle/projeteis/mago_principal/p2.png"};
static const char *PROJ_BOSS_B1_FRAMES[]   = {"img/battle/projeteis/mago_pedra/p1.png", "img/battle/projeteis/mago_pedra/p2.png"};

// Batalha 2 (planta) - ajuste os paths se tiver sprites diferentes.
static const char *PLAYER_B2_FRAMES[]      = {"img/battle/player/leste1.png", "img/battle/player/leste2.png", "img/battle/player/leste3.png", "img/battle/player/leste4.png"};
static const char *HENCH_B2_FRAMES[]       = {"img/battle/monstros/monstro_planta/mp1.png"};
static const char *BOSS_B2_FRAMES[]        = {"img/battle/chefoes/mago_planta.png"};
static const char *PROJ_PLAYER_B2_FRAMES[] = {"img/battle/projeteis/mago_principal/p1.png", "img/battle/projeteis/mago_principal/p2.png"};
static const char *PROJ_BOSS_B2_FRAMES[]   = {"img/battle/projeteis/mago_planta/p1.png", "img/battle/projeteis/mago_planta/p2.png"};

// Batalha 3 (fogo) - ajuste os paths se tiver sprites diferentes.
static const char *PLAYER_B3_FRAMES[]      = {"img/battle/player/leste1.png", "img/battle/player/leste2.png", "img/battle/player/leste3.png", "img/battle/player/leste4.png"};
static const char *HENCH_B3_FRAMES[]       = {"img/battle/monstros/monstro_fogo/mf1.png"};
static const char *BOSS_B3_FRAMES[]        = {"img/battle/chefoes/mago_fogo.png"};
static const char *PROJ_PLAYER_B3_FRAMES[] = {"img/battle/projeteis/mago_principal/p1.png", "img/battle/projeteis/mago_principal/p2.png"};
static const char *PROJ_BOSS_B3_FRAMES[]   = {"img/battle/projeteis/mago_fogo/p1.png", "img/battle/projeteis/mago_fogo/p2.png"};

// Helper para evitar passar contagens erradas.
#define COUNT(arr) (int)(sizeof(arr) / sizeof((arr)[0]))

// Cria uma configuração base com stats e paths.
static void preencher_config(EntidadesBatalha *d, const char **player_frames, int player_count,
                             const char **hench_frames, int hench_count,
                             const char **boss_frames, int boss_count,
                             const char **proj_p_frames, int proj_p_count,
                             const char **proj_b_frames, int proj_b_count) {
    memset(d, 0, sizeof(*d));
    d->player.speed = 300.0f;
    d->player.hp = 100;
    d->player.damage = 10;
    d->player.animacao.paths = player_frames;
    d->player.animacao.frame_count = player_count;
    d->player.animacao.intervalo_ms = 70.0;

    d->hench.speed = 150.0f;
    d->hench.hp = 6;
    d->hench.damage = 10;
    d->hench.animacao.paths = hench_frames;
    d->hench.animacao.frame_count = hench_count;
    d->hench.animacao.intervalo_ms = 150.0;

    d->boss.speed = 200.0f;
    d->boss.hp = 100;
    d->boss.damage = 10;
    d->boss.animacao.paths = boss_frames;
    d->boss.animacao.frame_count = boss_count;
    d->boss.animacao.intervalo_ms = 200.0;

    d->projetil_player.speed = 800.0f;
    d->projetil_player.damage = 2;
    d->projetil_player.animacao.paths = proj_p_frames;
    d->projetil_player.animacao.frame_count = proj_p_count;
    d->projetil_player.animacao.intervalo_ms = 80.0;

    d->projetil_boss.speed = 600.0f;
    d->projetil_boss.damage = 10;
    d->projetil_boss.animacao.paths = proj_b_frames;
    d->projetil_boss.animacao.frame_count = proj_b_count;
    d->projetil_boss.animacao.intervalo_ms = 80.0;
}

// Helper para carregar todos os AnimacaoDados declarados na config.
static bool carregar_pacote_animacoes(EntidadesBatalha *d) {
    bool ok = true;
    ok &= carregar_animacao_dados(&d->player.animacao, d->player.animacao.paths, d->player.animacao.frame_count, d->player.animacao.intervalo_ms);
    ok &= carregar_animacao_dados(&d->hench.animacao, d->hench.animacao.paths, d->hench.animacao.frame_count, d->hench.animacao.intervalo_ms);
    ok &= carregar_animacao_dados(&d->boss.animacao, d->boss.animacao.paths, d->boss.animacao.frame_count, d->boss.animacao.intervalo_ms);
    ok &= carregar_animacao_dados(&d->projetil_player.animacao, d->projetil_player.animacao.paths, d->projetil_player.animacao.frame_count, d->projetil_player.animacao.intervalo_ms);
    ok &= carregar_animacao_dados(&d->projetil_boss.animacao, d->projetil_boss.animacao.paths, d->projetil_boss.animacao.frame_count, d->projetil_boss.animacao.intervalo_ms);
    return ok;
}

bool carregar_entidades_batalha(int id, EntidadesBatalha *dest) {
    if (!dest) return false;

    switch (id) {
        case 1:
            preencher_config(dest, PLAYER_B1_FRAMES, COUNT(PLAYER_B1_FRAMES),
                             HENCH_B1_FRAMES, COUNT(HENCH_B1_FRAMES),
                             BOSS_B1_FRAMES, COUNT(BOSS_B1_FRAMES),
                             PROJ_PLAYER_B1_FRAMES, COUNT(PROJ_PLAYER_B1_FRAMES),
                             PROJ_BOSS_B1_FRAMES, COUNT(PROJ_BOSS_B1_FRAMES));
            break;
        case 2:
            preencher_config(dest, PLAYER_B2_FRAMES, COUNT(PLAYER_B2_FRAMES),
                             HENCH_B2_FRAMES, COUNT(HENCH_B2_FRAMES),
                             BOSS_B2_FRAMES, COUNT(BOSS_B2_FRAMES),
                             PROJ_PLAYER_B2_FRAMES, COUNT(PROJ_PLAYER_B2_FRAMES),
                             PROJ_BOSS_B2_FRAMES, COUNT(PROJ_BOSS_B2_FRAMES));
            break;
        case 3:
            preencher_config(dest, PLAYER_B3_FRAMES, COUNT(PLAYER_B3_FRAMES),
                             HENCH_B3_FRAMES, COUNT(HENCH_B3_FRAMES),
                             BOSS_B3_FRAMES, COUNT(BOSS_B3_FRAMES),
                             PROJ_PLAYER_B3_FRAMES, COUNT(PROJ_PLAYER_B3_FRAMES),
                             PROJ_BOSS_B3_FRAMES, COUNT(PROJ_BOSS_B3_FRAMES));
            break;
        default:
            return false;
    }
    return carregar_pacote_animacoes(dest);
}

void descarregar_entidades_batalha(EntidadesBatalha *dest) {
    if (!dest) return;
    descarregar_animacao_dados(&dest->player.animacao);
    descarregar_animacao_dados(&dest->hench.animacao);
    descarregar_animacao_dados(&dest->boss.animacao);
    descarregar_animacao_dados(&dest->projetil_player.animacao);
    descarregar_animacao_dados(&dest->projetil_boss.animacao);
}

// Carrega todas as três batalhas em um único catálogo.
bool carregar_catalogo_batalhas(CatalogoBatalhas *catalogo) {
    if (!catalogo) return false;
    bool ok = true;
    ok &= carregar_entidades_batalha(1, &catalogo->batalha1);
    ok &= carregar_entidades_batalha(2, &catalogo->batalha2);
    ok &= carregar_entidades_batalha(3, &catalogo->batalha3);
    return ok;
}

// Descarta texturas de todas as batalhas carregadas.
void descarregar_catalogo_batalhas(CatalogoBatalhas *catalogo) {
    if (!catalogo) return;
    descarregar_entidades_batalha(&catalogo->batalha1);
    descarregar_entidades_batalha(&catalogo->batalha2);
    descarregar_entidades_batalha(&catalogo->batalha3);
}
