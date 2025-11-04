#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include "../mapa.h" // Inclui o cabeçalho para usar as definições

// A definição da função agora corresponde à declaração em mapa.h
void carregar_mapa(int mapa[Map_y][Map_x])
{
    const char *imagem_path = "img/mapa.png";
    Image img = LoadImage(imagem_path);
    if (!img.data) {
        TraceLog(LOG_ERROR, "Erro: nao foi possivel carregar a imagem do mapa '%s'", imagem_path);
        return;
    }

    for (int y = 0; y < Map_y; y++) { // Corrigido para Map_y (minúsculo)
        for (int x = 0; x < Map_x; x++) { // Corrigido para Map_x (minúsculo)
            
            int pixelX = x * Tile_size + Tile_size / 2; // Corrigido para Tile_size
            int pixelY = y * Tile_size + Tile_size / 2; // Corrigido para Tile_size

            if (pixelX >= img.width)  pixelX = img.width - 1;
            if (pixelY >= img.height) pixelY = img.height - 1;

            Color cor = GetImageColor(img, pixelX, pixelY);

            // baseado nas teoria rgb e no codigo de cada uma ele analiza as cores que atendem as especificações abaixo
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
        for (int y = 0; y < Map_y; y++) { // Corrigido para Map_y
            for (int x = 0; x < Map_x; x++) { // Corrigido para Map_x
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
