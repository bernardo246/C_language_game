#include <raylib.h>
#include <time.h>
#include "../mapa/hitbox_mapa.h"

// Função de controle de tempo
int passou_tempo_animacao(double intervalo_ms) {
    static clock_t ultimo_tempo = 0; // mantém o tempo entre chamadas
    clock_t agora = clock();
    double tempo_decorrido = (double)(agora - ultimo_tempo) / CLOCKS_PER_SEC * 1000.0; 
    if (tempo_decorrido >= intervalo_ms) {
        ultimo_tempo = agora; 
        return 1;           
    }
    return 0;                 
}

// Função de animação do mago
void animacao_do_mago_no_mapa(Personagem *p){
    static int usou_animacao = 0;

    if(IsKeyUp(KEY_RIGHT)&& IsKeyUp(KEY_DOWN)&& IsKeyUp(KEY_LEFT)&& IsKeyUp(KEY_UP)){
        if (passou_tempo_animacao(2.0)){
            if(usou_animacao==0){
                p->t=LoadTexture(imagem,p->x,p->y,WHITE);
                usou_animacao=1;
            } else if(usou animacao==1){ 
                p->t=LoadTexture(imagem2,p->x,p->y,WHITE);
                usou_animacao=2;
            }
            else if(usou animacao==2){ 
                p->t=LoadTexture(imagem2,p->x,p->y,WHITE);
                usou_animacao=3;
            }
            else if(usou animacao==3){ 
                p->t=LoadTexture(imagem,p->x,p->y,WHITE);
                usou_animacao=0;
            }
        }
    }

    if(IsKeyDown(KEY_RIGHT)){
        if (passou_tempo_animacao(2.0)){
            if(usou_animacao==0){
                p->t=LoadTexture(imagem,p->x,p->y,WHITE);
                usou_animacao=1;
            } else if(usou animacao==1){ 
                p->t=LoadTexture(imagem2,p->x,p->y,WHITE);
                usou_animacao=2;
            }
            else if(usou animacao==2){ 
                p->t=LoadTexture(imagem2,p->x,p->y,WHITE);
                usou_animacao=3;
            }
            else if(usou animacao==3){ 
                p->t=LoadTexture(imagem,p->x,p->y,WHITE);
                usou_animacao=0;
            }
        }
    }

    if(IsKeyDown(KEY_LEFT)){
       if (passou_tempo_animacao(2.0)){
            if(usou_animacao==0){
                p->t=LoadTexture(imagem,p->x,p->y,WHITE);
                usou_animacao=1;
            } else if(usou animacao==1){ 
                p->t=LoadTexture(imagem2,p->x,p->y,WHITE);
                usou_animacao=2;
            }
            else if(usou animacao==2){ 
                p->t=LoadTexture(imagem2,p->x,p->y,WHITE);
                usou_animacao=3;
            }
            else if(usou animacao==3){ 
                p->t=LoadTexture(imagem,p->x,p->y,WHITE);
                usou_animacao=0;
            }
        }
    }



    if(IsKeyDown(KEY_DOWN)){
        if (passou_tempo_animacao(2.0)){
            if(usou_animacao==0){
                p->t=LoadTexture(imagem,p->x,p->y,WHITE);
                usou_animacao=1;
            } else if(usou animacao==1){ 
                p->t=LoadTexture(imagem2,p->x,p->y,WHITE);
                usou_animacao=2;
            }
            else if(usou animacao==2){ 
                p->t=LoadTexture(imagem2,p->x,p->y,WHITE);
                usou_animacao=3;
            }
            else if(usou animacao==3){ 
                p->t=LoadTexture(imagem,p->x,p->y,WHITE);
                usou_animacao=0;
            }
        }
    }
}
