// mapa.h
#ifndef HITBOX_MAPA_H
#define HITBOX_MAPA_H




#define Tile_size 2
#define Map_x (1280/Tile_size)
#define Map_y (720/Tile_size)
#define Tile_estrada 0
#define Tile_Nao_anda 1

typedef struct p{
    float x;
    float y;
    float speed;
    // Texture *t;
}Personagem;

typedef struct coordenadas {
    int mago;
    int x;
    int y;
    int xf;
    int yf;
    struct coordenadas *prox;
} coordenadas;



Texture carregar_mapa(int mapa[Map_y][Map_x]);
void logica_de_colisao_movimentacao(Personagem *p, int mapa[Map_y][Map_x]);
void criar_no(coordenadas **head, int cx, int cy,int cxf , int cyf, int mago);
int verificacao_de_area(coordenadas **head, Personagem *p,int mago);

#endif
