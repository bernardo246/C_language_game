#ifndef BATTLE_ENTIDADES_H
#define BATTLE_ENTIDADES_H

#include <raylib.h>
#include <stdbool.h>
#include "animacoes.h"

// Configuração completa de uma entidade em batalha.
typedef struct {
    AnimacaoDados animacao; // frames + intervalo
    float speed;
    int hp;
    int damage;
} DadosEntidade;

// Configuração de projétil (mantém dano separado para flexibilidade).
typedef struct {
    AnimacaoDados animacao;
    float speed;
    int damage;
} DadosProjetil;

// Pacote com todas as entidades usadas em uma batalha específica.
typedef struct {
    DadosEntidade player;
    DadosEntidade hench;
    DadosEntidade boss;
    DadosProjetil projetil_player;
    DadosProjetil projetil_boss;
} EntidadesBatalha;

// Catálogo completo com as três batalhas disponíveis.
typedef struct {
    EntidadesBatalha batalha1;
    EntidadesBatalha batalha2;
    EntidadesBatalha batalha3;
} CatalogoBatalhas;

// Carrega as texturas e configurações para a batalha informada (1, 2 ou 3).
// Retorna false se algo falhar ao carregar sprites.
bool carregar_entidades_batalha(int id, EntidadesBatalha *dest);
// Libera todas as texturas carregadas para a batalha.
void descarregar_entidades_batalha(EntidadesBatalha *dest);

// Helpers para carregar/descartar as três batalhas de uma vez.
bool carregar_catalogo_batalhas(CatalogoBatalhas *catalogo);
void descarregar_catalogo_batalhas(CatalogoBatalhas *catalogo);

#endif
