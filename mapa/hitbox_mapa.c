#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include "hitbox_mapa.h" // Inclui o cabeçalho para usar as definições

#define Tile_estrada 0
#define Tile_Nao_anda 1


// definindo as areas de batalha
area_de_batalha area1 = {370, 216, 412, 239, NULL}; // mago 1




// A definição da função agora corresponde à declaração em mapa.h
Texture carregar_mapa(int mapa[Map_y][Map_x]) {
    const char *imagem_path = "img/mapa.png";
    
    // Carrega a imagem original
    Image img = LoadImage(imagem_path);
    if (!img.data) {
        printf("Erro: nao foi possivel carregar %s\n", imagem_path);
        return (Texture){0};
    }

    // Redimensiona a imagem para a resolução da janela
    ImageResize(&img, Map_x * Tile_size, Map_y * Tile_size);

    // Preenche a matriz do mapa (hitbox)
    for (int y = 0; y < Map_y; y++) {
        for (int x = 0; x < Map_x; x++) {
            int pixelX = x * Tile_size + Tile_size / 2;
            int pixelY = y * Tile_size + Tile_size / 2;

            if (pixelX >= img.width)  pixelX = img.width - 1;
            if (pixelY >= img.height) pixelY = img.height - 1;

            Color cor = GetImageColor(img, pixelX, pixelY);
            int brilho = (cor.r + cor.g + cor.b) / 3;

            if (cor.g > cor.r && cor.g > cor.b)
                mapa[y][x] = Tile_Nao_anda;
            else if (cor.b > 150 && cor.g < 100)
                mapa[y][x] = Tile_Nao_anda;
            else if (brilho < 50)
                mapa[y][x] = Tile_Nao_anda;
            else if (brilho > 100 && abs(cor.r - cor.g) < 30 && abs(cor.r - cor.b) < 30 && abs(cor.g - cor.b) ) // tons de cinza
                mapa[y][x] = Tile_Nao_anda;
            // azul do mar 
            else if (cor.b > cor.r + 50 && cor.b > cor.g + 50 && cor.b > 100)
                mapa[y][x] = Tile_Nao_anda;
            // mar claro
            else if (cor.b > 150 && cor.g > 150 && cor.r < 100)
                mapa[y][x] = Tile_Nao_anda;
            // verde do mar
            else if (cor.g > cor.b + 50 && cor.g > cor.r + 50 && cor.g > 100)
                mapa[y][x] = Tile_Nao_anda;
            else if(cor.r<40 && cor.g>100 && cor.b>50) 
                mapa[y][x] = Tile_Nao_anda;
            else
                mapa[y][x] = Tile_estrada;
        }
    }

    // Exportar mapa para debug (gera map_hitbox.txt)
    FILE *f = fopen("map_hitbox.txt", "w");
    if (f) {
        for (int y = 0; y < Map_y; y++) {
            for (int x = 0; x < Map_x; x++)
                fprintf(f, "%d", mapa[y][x]);
            fprintf(f, "\n");
        }
        fclose(f);
        printf("Arquivo map_hitbox.txt gerado com sucesso!\n");
    } else {
        printf("Erro ao salvar map_hitbox.txt\n");
    }

    // Cria a textura para desenhar na GPU
    Texture tex = LoadTextureFromImage(img);

    // Libera a imagem da RAM
    UnloadImage(img);

    return tex;
}



void logica_de_colisao_movimentacao(Personagem *p, int mapa[Map_y][Map_x]){
    //coordenadas do personagem
    float dx= p->x;
    float dy= p->y;
    //movimentaçao
    if(IsKeyDown(KEY_RIGHT)) dx += p->speed;
    if(IsKeyDown(KEY_LEFT))  dx -= p->speed;
    if(IsKeyDown(KEY_UP))    dy -= p->speed;
    if(IsKeyDown(KEY_DOWN))  dy += p->speed;
    //calculo da tile que ele estaria dps do movimento = chunk que ele esta conjunto de pixels 4x4
    int tileX= (int)(dx/Tile_size);
    int tileY= (int)(dy/Tile_size);
    
    // Verificar se está dentro do mapa
    if (tileX >= 0 && tileX < Map_x && tileY >= 0 && tileY < Map_y){
        if(mapa[tileY][tileX] == Tile_estrada){
            p->x=dx;
            p->y=dy;
        }
    }
}


// Função para criar um novo nó na lista ligada
void criar_no(coordenadas **head, int cx, int cy) {
    coordenadas *novo = (coordenadas *)malloc(sizeof(coordenadas));
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }

    novo->x = cx;
    novo->y = cy;
    novo->prox = NULL; 

    if (*head == NULL) {
        *head = novo;
    } else {
        coordenadas *aux = *head;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}



// Função para definir uma área de batalha
void area_de_batalha(coordenadas **head, int x, int y, int xf, int yf) {
    for (int i = y; i < yf; i++) {
        for (int j = x; j < xf; j++) {
            criar_no(head, j, i); 
        }
    }
}

int verificacao_de_area(coordenadas **head, Personagem *p) {
    coordenadas *aux = *head;
    while (aux != NULL) {
        if (aux->x == p->x && aux->y == p->y) {
            return 1;
        }
        aux = aux->prox;
    }
    return 0;
}


//x=370 y=216 / x=412 y=239 mago 1
