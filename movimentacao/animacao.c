#include <raylib.h>
#include <time.h>
#include "../mapa/hitbox_mapa.h"


// static Texture norte[4], sul[4], leste[4], oeste[4];
// static int texturas_carregadas = 0;
float tempoanimacao = 20.0; // intervalo de tempo em ms entre cada frame da animação

// // carregar texturas
// void carregar_textura(){
//     norte[0] = LoadTexture("img/wizard/norte/imagem1.png");
//     norte[1] = LoadTexture("img/wizard/norte/imagem2.png");
//     norte[2] = LoadTexture("img/wizard/norte/imagem3.png");
//     norte[3] = LoadTexture("img/wizard/norte/imagem4.png");

//     sul[0] = LoadTexture("img/wizard/sul/imagem1.png");
//     sul[1] = LoadTexture("img/wizard/sul/imagem2.png");
//     sul[2] = LoadTexture("img/wizard/sul/imagem3.png");
//     sul[3] = LoadTexture("img/wizard/sul/imagem4.png");

//     leste[0] = LoadTexture("img/wizard/leste/imagem1.png");
//     leste[1] = LoadTexture("img/wizard/leste/imagem2.png");
//     leste[2] = LoadTexture("img/wizard/leste/imagem3.png");
//     leste[3] = LoadTexture("img/wizard/leste/imagem4.png");

//     oeste[0] = LoadTexture("img/wizard/oeste/imagem1.png");
//     oeste[1] = LoadTexture("img/wizard/oeste/imagem2.png");
//     oeste[2] = LoadTexture("img/wizard/oeste/imagem3.png");
//     oeste[3] = LoadTexture("img/wizard/oeste/imagem4.png");

//     texturas_carregadas = 1;
// }


// // descarregar texturas
// void descarregar_texturas(){
//     UnloadTexture(norte[0]);
//     UnloadTexture(norte[1]);
//     UnloadTexture(norte[2]);
//     UnloadTexture(norte[3]);

//     UnloadTexture(sul[0]);
//     UnloadTexture(sul[1]);
//     UnloadTexture(sul[2]);
//     UnloadTexture(sul[3]);

//     UnloadTexture(leste[0]);
//     UnloadTexture(leste[1]);
//     UnloadTexture(leste[2]);
//     UnloadTexture(leste[3]);

//     UnloadTexture(oeste[0]);
//     UnloadTexture(oeste[1]);
//     UnloadTexture(oeste[2]);
//     UnloadTexture(oeste[3]);

//     texturas_carregadas = 0;
// }



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


// // Função de animação do mago
// void animacao_do_mago_no_mapa(Personagem *p){
//     static int usou_animacao = 0;
//     static int direcao_atual = 4; // 0: norte, 1: leste, 2: oeste, 3: sul
//     if (!texturas_carregadas) {
//         carregar_textura();
//     }


//     if (IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_DOWN) && IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_UP)) {
//         if (direcao_atual == 0) {
//             p->t = norte[0];
//         } else if (direcao_atual == 1) {
//             p->t = leste[0];
//         } else if (direcao_atual == 2) {
//             p->t = oeste[0];
//         } else if (direcao_atual == 3) {
//             p->t = sul[0];
//         }
//     }

//     if (IsKeyDown(KEY_UP)){
//         direcao_atual = 0; // norte
//         if (passou_tempo_animacao(tempoanimacao)){
//             if (usou_animacao == 0){
//                 p->t = norte[0];
//                 usou_animacao = 1;
//             } else if (usou_animacao == 1){ 
//                 p->t = norte[1];
//                 usou_animacao = 2;
//             }
//             else if (usou_animacao == 2){ 
//                 p->t = norte[2];
//                 usou_animacao = 3;
//             }
//             else if (usou_animacao == 3){ 
//                 p->t = norte[3];
//                 usou_animacao = 0;
//             }
//         }
//     }

//     else if (IsKeyDown(KEY_RIGHT)){
//         direcao_atual = 1; // leste
//         if (passou_tempo_animacao(tempoanimacao)){
//             if (usou_animacao == 0){
//                 p->t = leste[0];
//                 usou_animacao = 1;
//             } else if (usou_animacao == 1){ 
//                 p->t = leste[1];
//                 usou_animacao = 2;
//             }
//             else if (usou_animacao == 2){ 
//                 p->t = leste[2];
//                 usou_animacao = 3;
//             }
//             else if (usou_animacao == 3){ 
//                 p->t = leste[3];
//                 usou_animacao = 0;
//             }
//         }
//     }

//     else if (IsKeyDown(KEY_LEFT)){
//         direcao_atual = 2; // oeste
//         if (passou_tempo_animacao(tempoanimacao)){
//             if (usou_animacao == 0){
//                 p->t = oeste[0];
//                 usou_animacao = 1;
//             } else if (usou_animacao == 1){ 
//                 p->t = oeste[1];
//                 usou_animacao = 2;
//             }
//             else if (usou_animacao == 2){ 
//                 p->t = oeste[2];
//                 usou_animacao = 3;
//             }
//             else if (usou_animacao == 3){ 
//                 p->t = oeste[3];
//                 usou_animacao = 0;
//             }
//         }
//     }

//     else if (IsKeyDown(KEY_DOWN)){
//         direcao_atual = 3; // sul
//         if (passou_tempo_animacao(tempoanimacao)){
//             if (usou_animacao == 0){
//                 p->t = sul[0];
//                 usou_animacao = 1;
//             } else if (usou_animacao == 1){ 
//                 p->t = sul[1];
//                 usou_animacao = 2;
//             }
//             else if (usou_animacao == 2){ 
//                 p->t = sul[2];
//                 usou_animacao = 3;
//             }
//             else if (usou_animacao == 3){ 
//                 p->t = sul[3];
//                 usou_animacao = 0;
//             }
//         }
//     }
// }



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
        if (passou_tempo_animacao(tempoanimacao)){
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
        if (passou_tempo_animacao(tempoanimacao)){
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
        if (passou_tempo_animacao(tempoanimacao)){
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
        if (passou_tempo_animacao(tempoanimacao)){
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
