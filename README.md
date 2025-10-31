# Magic Battle


Um jogo de batalha de magos 2D, tático e por turnos, desenvolvido em **C** puro utilizando a biblioteca **Raylib**.

## 📖 Sobre o Jogo

`Magic Battle` coloca você no lugar de um mago poderoso em uma arena de combate místico. O objetivo é derrotar o mago oponente usando um arsenal de feitiços.

O núcleo do jogo é um **sistema de combate estratégico baseado em turnos**. A cada turno, tanto você quanto seu oponente devem escolher uma ação — seja lançar um feitiço ofensivo, usar uma magia de suporte ou preparar uma defesa. Pense com cuidado, pois cada feitiço tem seus próprios custos, poder e efeitos!

## ✨ Características Principais

* **Combate Tático por Turnos:** Selecione sua ação em um menu de combate e veja a rodada se desenrolar.
* **Sistema de Magias:** Um conjunto de magias selecionáveis para cada mago, cada uma com atributos únicos (dano, efeitos, etc.).
* **Gerenciamento de Recursos:** Fique de olho nos seus Pontos de Vida (HP) e Pontos de Magia (MP). Ficar sem HP significa derrota!
* **IA Desafiadora:** Um oponente controlado por computador com sua própria lógica de batalha.
* **Feito com Raylib:** Todo o jogo é renderizado usando a leve e poderosa biblioteca gráfica Raylib, ideal para desenvolvimento em C.

## 💻 Tecnologias Utilizadas

* **Linguagem:** C (C99)
* **Biblioteca Gráfica:** Raylib 5.0 (ou versão utilizada)
* **Compilação:** Makefile (ou CMake/GCC)

## 🚀 Como Compilar e Rodar

Você precisará ter o **GCC** (ou Clang) e a biblioteca **Raylib** instalada em seu sistema.

### Pré-requisitos

* [Compilador C (GCC)](https://gcc.gnu.org/)
* [Raylib (instruções de instalação)](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)

### Passos

1.  Clone este repositório:
    ```bash
    git clone [https://github.com/seu-usuario/magic-battle.git](https://github.com/seu-usuario/magic-battle.git)
    cd magic-battle
    ```

2.  Compile o projeto. Se estiver usando o Makefile incluído:
    ```bash
    make
    ```

    *Se não houver um Makefile, você pode compilar manualmente (exemplo para Linux):*
    ```bash
    gcc src/main.c -o magic_battle -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    ```

3.  Execute o jogo:
    ```bash
    ./magic_battle
    ```

## 🎮 Controles

| Tecla | Ação |
| :--- | :--- |
| `Setas Cima/Baixo` | Navegar no menu de magias |
| `Enter` ou `Z` | Confirmar seleção / Avançar diálogo |
| `ESC` ou `X` | Voltar / Cancelar (se aplicável) |

## 🖼️ Screenshots (Opcional)

*(Adicione aqui algumas capturas de tela do seu jogo em ação)*

[Imagem da tela de título]
[Imagem de uma batalha em andamento]

## 📝 A Fazer (Roadmap)

* [ ] Adicionar mais magias
* [ ] Implementar um sistema de itens (poções de HP/MP)
* [ ] Adicionar música de fundo e efeitos sonoros
* [ ] Criar uma tela de "Game Over" e "Vitória"
* [ ] Balancear a dificuldade da IA

## 📄 Licença

Este projeto é distribuído sob a licença [MIT / GPL / etc.]. Veja o arquivo `LICENSE` para mais detalhes.