#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <raylib.h>
#include "menuprincipal.h"
#include "../mapa/hitbox_mapa.h" // Inclui o cabeçalho para usar as definições

int opcao_menu = 0;
static Texture menu_fundo = {0};
static bool menu_fundo_loaded = false;

void mostrar_menu(int *tela_opcao){
    // carrega a imagem original (somente uma vez)
    const char *imagem_path = "img/menuprincipal.png";

    if (!menu_fundo_loaded) {
        Image img = LoadImage(imagem_path);
        if (!img.data) {
            printf("Erro: nao foi possivel carregar %s\n", imagem_path);
            return;
        }
        ImageResize(&img, Map_x * Tile_size, Map_y * Tile_size);
        menu_fundo = LoadTextureFromImage(img);
        UnloadImage(img); // evita vazamento de RAM do Image
        menu_fundo_loaded = true;
    }

    if (IsKeyPressed(KEY_DOWN)){
        opcao_menu ++;
    }
    if (IsKeyPressed(KEY_UP)){
        opcao_menu --;
    } 


    if (opcao_menu > 1 || opcao_menu < 0){
        opcao_menu = 0; // Garante que a opção fique entre 0 e 1
    }
    

    if(IsKeyPressed(KEY_ENTER)){
        if (opcao_menu == 0){
            *tela_opcao = 1; // Muda para a tela de jogo
        }
        if (opcao_menu == 1){
            *tela_opcao = -1; // Fecha a janela e encerra o jogo
        } 
    }

    // Vector2 mouse = GetMousePosition();

    // char opcaoText[32];
    // sprintf(opcaoText, "Opcao selecionada: %d", opcao_menu);
    // char mouseText[32];
    // sprintf(mouseText, "Mouse X: %.2f  Mouse Y: %.2f", mouse.x, mouse.y);

    

    Color amarelosemi = (Color){238, 245, 39, 100 }; // cor do quadrado selecionado

    //desenhando a tela do menu
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawTexture(menu_fundo, 0, 0, WHITE); // fundo do menu

    // DrawText(opcaoText, 10, 10, 20, DARKGRAY); //opcao selecionada
    // DrawText(mouseText, 10, 40, 20, DARKGRAY); //coordenadas do mouse

    if(opcao_menu == 0){
        DrawRectangle(528, 534, 190, 50, amarelosemi); // Retângulo para "Iniciar Jogo"
    }
    if(opcao_menu == 1){
        DrawRectangle(540,629 , 160, 50, amarelosemi); // Retângulo para "Sair"
    }

    EndDrawing();
}

// Descarrega recursos persistentes do menu (no design atual, não há persistência)
// Mantemos a função para corresponder ao header e evitar erro de link.
void descarregar_menu(void) {
    if (menu_fundo_loaded) {
        UnloadTexture(menu_fundo);
        menu_fundo = (Texture){0};
        menu_fundo_loaded = false;
    }
}
