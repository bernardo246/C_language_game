#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mapa/hitbox_mapa.h"


void animacao_do_mago_no_mapa(Personagem *p){
    if(IsKeyUp(KEY_RIGHT)&& IsKeyUp(KEY_DOWN)&& IsKeyUp(KEY_LEFT)&& IsKeyUp(KEY_UP)){
        p->t=LoadTexture(imagem);
    }
    if(IsKeyDown(KEY_RIGHT)){
        p->t=LoadTexture(imagem);
    }
    if(IsKeyDown(KEY_LEFT)){
        p->t=LoadTexture(imagem);
    }
    if(IsKeyDown(KEY_UP)){
        p->t=LoadTexture(imagem);
    }
    if(IsKeyDown(KEY_DOWN)){
        p->t=LoadTexture(imagem);
    }
}

