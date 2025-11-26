#include "animacoes.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "battlefunctions.h"

// Carrega todos os frames informados e prepara a struct de dados compartilhados.
// Retorna false em caso de erro para permitir checagem no código de montagem da batalha.
bool carregar_animacao_dados(AnimacaoDados *anim, const char **paths, int frame_count, double intervalo_ms) {
    if (!anim || !paths || frame_count <= 0 || intervalo_ms <= 0.0) {
        return false;
    }
    memset(anim, 0, sizeof(*anim));
    anim->paths = paths;
    anim->frame_count = frame_count;
    anim->intervalo_ms = intervalo_ms;

    anim->frames = (Texture2D *)malloc(sizeof(Texture2D) * frame_count);
    if (!anim->frames) {
        return false;
    }
    memset(anim->frames, 0, sizeof(Texture2D) * frame_count);

    // Carrega cada arquivo de sprite em uma Texture2D.
    for (int i = 0; i < frame_count; i++) {
        anim->frames[i] = LoadTexture(paths[i]);
    }
    return true;
}

// Libera as textures carregadas e zera ponteiros.
void descarregar_animacao_dados(AnimacaoDados *anim) {
    if (!anim || !anim->frames) return;

    for (int i = 0; i < anim->frame_count; i++) {
        if (anim->frames[i].id != 0) {
            UnloadTexture(anim->frames[i]);
        }
    }
    free(anim->frames);
    anim->frames = NULL;
}

// Liga um estado individual a um conjunto de dados já carregado.
void iniciar_animacao_estado(AnimacaoEstado *estado, AnimacaoDados *dados) {
    if (!estado) return;
    estado->dados = dados;
    estado->frame_atual = 0;
    estado->acumulado_ms = 0.0;
    estado->textura_atual = (Texture2D){0};
    if (dados && dados->frames && dados->frame_count > 0) {
        estado->textura_atual = dados->frames[0];
    }
}

// Converte delta de raylib (s) para ms.
static inline double delta_ms(void) {
    return GetFrameTime();
}

// Avança a animação se o personagem/projétil está em movimento.
void atualizar_animacao_estado(AnimacaoEstado *estado, bool em_movimento) {
    if (!estado || !estado->dados || !estado->dados->frames || estado->dados->frame_count <= 0) return;

    if (!em_movimento) {
        estado->frame_atual = 0;
        estado->acumulado_ms = 0.0;
        estado->textura_atual = estado->dados->frames[0];
        return;
    }

    estado->acumulado_ms += delta_ms();
    if (estado->acumulado_ms < estado->dados->intervalo_ms) {
        return;
    }

    int passos = (int)(estado->acumulado_ms / estado->dados->intervalo_ms);
    estado->frame_atual = (estado->frame_atual + passos) % estado->dados->frame_count;
    estado->acumulado_ms = fmod(estado->acumulado_ms, estado->dados->intervalo_ms);
    estado->textura_atual = estado->dados->frames[estado->frame_atual];
}

// Recupera o frame atual (ou zerado se algo faltou).
Texture2D animacao_frame_atual(const AnimacaoEstado *estado) {
    if (!estado || !estado->dados || !estado->dados->frames || estado->dados->frame_count <= 0) {
        return (Texture2D){0};
    }
    return estado->textura_atual;
}

// MIRA DINAMICA 
void mostrar_mira(henchman *henchman){
    // Desenha uma mira quadrada no mouse: vermelha se sobre um henchman, verde caso contrario.
    // Mantemos a checagem simples usando o mesmo scale de colisao dos capangas.
    static const float HENCHMAN_COLLISION_SCALE = 0.08f;

    Vector2 mouse = GetMousePosition();
    bool em_cima_de_henchman = false;
    for (int i = 0; i < MAX_HENCH; i++) {
        if (!henchman[i].active) continue;

        Texture2D henchFrame = animacao_frame_atual(&henchman[i].anim);
        if (henchFrame.id == 0) continue;

        Rectangle henchmanRect = {
            henchman[i].x - (henchFrame.width * HENCHMAN_COLLISION_SCALE / 2.0f),
            henchman[i].y - (henchFrame.height * HENCHMAN_COLLISION_SCALE / 2.0f),
            (float)henchFrame.width * HENCHMAN_COLLISION_SCALE,
            (float)henchFrame.height * HENCHMAN_COLLISION_SCALE
        };

        if (CheckCollisionPointRec(mouse, henchmanRect)) {
            em_cima_de_henchman = true;
            break;
        }
    }

    Color cor_mira = em_cima_de_henchman ? RED : GREEN;
    const float tamanho = 36.0f;
    const float arredondamento = 0.3f;
    Rectangle mira = {
        mouse.x - tamanho / 2.0f,
        mouse.y - tamanho / 2.0f,
        tamanho,
        tamanho
    };
    const int espessura = 4;
    for (int i = 0; i < espessura; i++) {
        Rectangle r = {
            mira.x - (float)i,
            mira.y - (float)i,
            mira.width + 2.0f * (float)i,
            mira.height + 2.0f * (float)i
        };
        // Desenho empilhado: cada loop expande o retangulo para simular espessura de borda.
        DrawRectangleRoundedLines(r, arredondamento, 6, cor_mira);
    }
}
