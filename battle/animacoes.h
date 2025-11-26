#ifndef BATTLE_ANIMACOES_H
#define BATTLE_ANIMACOES_H

#include <raylib.h>
#include <stdbool.h>

// Forward declaro para evitar incluir battlefunctions.h aqui (ciclo).
typedef struct h henchman;

// Dados compartilhados de uma animação (paths + textures carregadas).
// Vários personagens podem apontar para o mesmo conjunto de dados.
typedef struct {
    const char **paths;     // lista de caminhos dos frames
    int frame_count;        // quantos frames existem
    double intervalo_ms;    // tempo entre frames
    Texture2D *frames;      // textures carregadas (tamanho = frame_count)
} AnimacaoDados;

// Estado individual da animação de um personagem/projétil.
// Cada entidade guarda qual frame está ativo e o tempo acumulado.
typedef struct {
    AnimacaoDados *dados;   // aponta para os dados compartilhados
    int frame_atual;
    double acumulado_ms;
    Texture2D textura_atual; // texture do frame atual para uso rápido
} AnimacaoEstado;

// Utilitários simples para carregar/usar animações (não existe mais gerenciador central).
bool carregar_animacao_dados(AnimacaoDados *anim, const char **paths, int frame_count, double intervalo_ms);
void descarregar_animacao_dados(AnimacaoDados *anim);
void iniciar_animacao_estado(AnimacaoEstado *estado, AnimacaoDados *dados);
void atualizar_animacao_estado(AnimacaoEstado *estado, bool em_movimento);
Texture2D animacao_frame_atual(const AnimacaoEstado *estado);

// Desenha a mira customizada sobre henchmen.
void mostrar_mira(henchman *henchman);

#endif
