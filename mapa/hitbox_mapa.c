#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#define Tile_estrada 0
#define Tile_Nao_anda 1

#define Tile_size 16
#define Map_x (1280/Tile_size)
#define Map_y (720/Tile_size)

void carregar_mapa(char *img, int mapa[Map_y][Map_x])
{
    const char *imagem_path = "img/mapa.png";
    Image img = LoadImage(imagem_path);
    if (!img.data) {
        printf("Erro: nao foi possivel carregar '%s'\n", imagem_path);
        return;
    }

    for (int y = 0; y < MAP_Y; y++) {
        for (int x = 0; x < MAP_X; x++) {
            
            int pixelX = x * TILE_SIZE + TILE_SIZE / 2;
            int pixelY = y * TILE_SIZE + TILE_SIZE / 2;

            if (pixelX >= img.width)  pixelX = img.width - 1;
            if (pixelY >= img.height) pixelY = img.height - 1;

            Color cor = GetImageColor(img, pixelX, pixelY);

            // baseado nas teoria rgb e no codigo de cada uma ele analiza as cores que atendem as especificações abaixo
            Color cor = GetImageColor(img, pixelX, pixelY);
            int brilho = (cor.r + cor.g + cor.b) / 3; // para pegar a cor escura
            
            // Verde predominante -> não andável (grama/floresta)
            if (cor.g > cor.r && cor.g > cor.b) {
                mapa[y][x] = Tile_Nao_anda;
            } 
            // Azul -> água -> não andável
            else if (cor.b > 150 && cor.g < 100) {
                mapa[y][x] = Tile_Nao_anda;
            } 
            // Escuro -> não andável
            else if (brilho < 50) {
                mapa[y][x] = Tile_Nao_anda;
            } 
            // Vermelho e demais cores claras -> andável
            else {
                mapa[y][x] = Tile_estrada;
            }
        }
    }

    // Exportar mapa para debug(cria o txt que contem a matriz da hitbox do mapa)
    FILE *f = fopen("map_hitbox.txt", "w");
    if (f) {
        for (int y = 0; y < MAP_Y; y++) {
            for (int x = 0; x < MAP_X; x++) {
                fprintf(f, "%d", mapa[y][x]);
            }
            fprintf(f, "\n");
        }
        fclose(f);
        printf("Arquivo map_hitbox.txt gerado com sucesso!\n");
    } else {
        printf("Erro ao salvar map_hitbox.txt\n");
    }

    UnloadImage(img);
}


