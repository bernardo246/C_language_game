#include <raylib>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    InitWindow(1280,720) // inicializando janela

    while(!WindowShouldClose()){// loop principal + windowshouldclose= analise se a janela esta fechada, nesse caso o loop roda enquanto a janela esta aberta por causa do not


    }
    CloseWindow();// fechar a janela

    return 0;
}

//funcao para escrever algo na tela :
//BeginDrawing()
//
//nesse meio eu coloco a funcao com o formato da escrita e seus parametros ex:
// DrawText(texto,posicao x, posicao y, tamanho da fonte, cor)
//
//EndDrawing()


//carregar img:
//Texture nome da variave =x;
//x = LoadTexture(caminho para img)
//funcao para dar o diplay da img na tela = DrawTexture(x, posicao x, posicao y, cor)

// add fundo do jogo: tem que ser carregado antes de tudo
//vai dar um loadtexture()
//drawtexture(variavel, 0, 0,white)
