#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include "menus/menuprincipal.h"
#include "../mapa/hitbox_mapa.h"

void mostrar_menu(int *opcao){
    // carrega a imagem original
    const char *imagem_path = "img/menuprincipal.png";

    // Carrega a imagem original
    Image img = LoadImage(imagem_path);
    if (!img.data) {
        printf("Erro: nao foi possivel carregar %s\n", imagem_path);
    }

    // Redimensiona a imagem para a resolução da janela
    ImageResize(&img, Map_x * Tile_size, Map_y * Tile_size);

    Texture fundo = LoadTextureFromImage(img);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(fundo,0,0,WHITE);
    EndDrawing();
    
    *opcao = 0; // Mantém a opção como 0 para continuar no menu
}