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
    static int direcao_atual = 4; // 0: norte, 1: leste, 2: oeste, 3: sul

    if (IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_DOWN) && IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_UP)) {
        if (direcao_atual == 0) {
            p->t = LoadTexture("img/wizard/norte/imagem1.png");
        } else if (direcao_atual == 1) {
            p->t = LoadTexture("img/wizard/leste/imagem1.png");
        } else if (direcao_atual == 2) {
            p->t = LoadTexture("img/wizard/oeste/imagem1.png");
        } else if (direcao_atual == 3) {
            p->t = LoadTexture("img/wizard/sul/imagem1.png");
        }
    }

    if (IsKeyDown(KEY_UP)){
        direcao_atual = 0; // norte
        if (passou_tempo_animacao(2.0)){
            if (usou_animacao == 0){
                p->t = LoadTexture("img/wizard/norte/imagem1.png");
                usou_animacao = 1;
            } else if (usou_animacao == 1){ 
                p->t = LoadTexture("img/wizard/norte/imagem2.png");
                usou_animacao = 2;
            }
            else if (usou_animacao == 2){ 
                p->t = LoadTexture("img/wizard/norte/imagem3.png");
                usou_animacao = 3;
            }
            else if (usou_animacao == 3){ 
                p->t = LoadTexture("img/wizard/norte/imagem4.png");
                usou_animacao = 0;
            }
        }
    }

    else if (IsKeyDown(KEY_RIGHT)){
        direcao_atual = 1; // leste
        if (passou_tempo_animacao(2.0)){
            if (usou_animacao == 0){
                p->t = LoadTexture("img/wizard/leste/imagem1.png");
                usou_animacao = 1;
            } else if (usou_animacao == 1){ 
                p->t = LoadTexture("img/wizard/leste/imagem2.png");
                usou_animacao = 2;
            }
            else if (usou_animacao == 2){ 
                p->t = LoadTexture("img/wizard/leste/imagem3.png");
                usou_animacao = 3;
            }
            else if (usou_animacao == 3){ 
                p->t = LoadTexture("img/wizard/leste/imagem4.png");
                usou_animacao = 0;
            }
        }
    }

    else if (IsKeyDown(KEY_LEFT)){
        direcao_atual = 2; // oeste
        if (passou_tempo_animacao(2.0)){
            if (usou_animacao == 0){
                p->t = LoadTexture("img/wizard/oeste/imagem1.png");
                usou_animacao = 1;
            } else if (usou_animacao == 1){ 
                p->t = LoadTexture("img/wizard/oeste/imagem2.png");
                usou_animacao = 2;
            }
            else if (usou_animacao == 2){ 
                p->t = LoadTexture("img/wizard/oeste/imagem3.png");
                usou_animacao = 3;
            }
            else if (usou_animacao == 3){ 
                p->t = LoadTexture("img/wizard/oeste/imagem4.png");
                usou_animacao = 0;
            }
        }
    }

    else if (IsKeyDown(KEY_DOWN)){
        direcao_atual = 3; // sul
        if (passou_tempo_animacao(2.0)){
            if (usou_animacao == 0){
                p->t = LoadTexture("img/wizard/sul/imagem1.png");
                usou_animacao = 1;
            } else if (usou_animacao == 1){ 
                p->t = LoadTexture("img/wizard/sul/imagem2.png");
                usou_animacao = 2;
            }
            else if (usou_animacao == 2){ 
                p->t = LoadTexture("img/wizard/sul/imagem3.png");
                usou_animacao = 3;
            }
            else if (usou_animacao == 3){ 
                p->t = LoadTexture("img/wizard/sul/imagem4.png");
                usou_animacao = 0;
            }
        }
    }
}
