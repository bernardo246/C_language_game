#ifndef ANIMACAO_H
#define ANIMACAO_H

#include <raylib.h>
#include <time.h>
#include "mapa/hitbox_mapa.h"

int passou_tempo_animacao(double intervalo_ms);
void animacao_do_mago_no_mapa(Personagem *p);

#endif