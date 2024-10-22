#include <SDL2/SDL.h>
#include <stdio.h>

void drawTree(SDL_Renderer* renderer, int x, int y) {
    // Desenhar o tronco da árvore (retângulo marrom)
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrom (RGB: 139, 69, 19)
    SDL_Rect trunk = { x + 20, y + 50, 20, 50 }; // Tronco da árvore
    SDL_RenderFillRect(renderer, &trunk);

    // Desenhar a copa da árvore (verde mais escuro logo acima do tronco)
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // Verde mais escuro (RGB: 0, 100, 0)
    SDL_Rect darkFoliage = { x, y + 20, 60, 30 };   // Folhagem superior mais escura
    SDL_RenderFillRect(renderer, &darkFoliage);

    // Desenhar a folhagem inferior da árvore (verde normal)
    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Verde escuro (RGB: 34, 139, 34)
    SDL_Rect foliage = { x + 10, y + 25, 40, 30 }; // Folhagem inferior, mais próxima do tronco
    SDL_RenderFillRect(renderer, &foliage);
}

int main(int argc, char* argv[]) {
    // Inicializar a SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Criar uma janela
    SDL_Window* window = SDL_CreateWindow(
        "Minha janela SDL com Árvores",   // Título da janela
        SDL_WINDOWPOS_CENTERED,           // Posição X
        SDL_WINDOWPOS_CENTERED,           // Posição Y
        800, 600,                         // Largura e altura
        SDL_WINDOW_SHOWN                  // Mostra a janela
    );

    if (!window) {
        printf("Erro ao criar a janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Criar um renderizador associado à janela
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erro ao criar renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Definir cor verde (cor de folha) para as laterais
    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Verde escuro (RGB: 34, 139, 34)
    SDL_Rect leftRect = { 0, 0, 200, 600 };  // Lateral esquerda
    SDL_Rect rightRect = { 600, 0, 200, 600 };  // Lateral direita
    SDL_RenderFillRect(renderer, &leftRect);
    SDL_RenderFillRect(renderer, &rightRect);

    // Definir cor cinza (cor de asfalto) para a parte do meio
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Cinza (RGB: 128, 128, 128)
    SDL_Rect middleRect = { 200, 0, 400, 600 };  // Parte do meio
    SDL_RenderFillRect(renderer, &middleRect);

    // Desenhar árvores na lateral esquerda
    drawTree(renderer, 50, 100);   // Árvore 1 na esquerda
    drawTree(renderer, 50, 300);   // Árvore 2 na esquerda
    drawTree(renderer, 50, 500);   // Árvore 3 na esquerda

    // Desenhar árvores na lateral direita
    drawTree(renderer, 650, 100);  // Árvore 1 na direita
    drawTree(renderer, 650, 300);  // Árvore 2 na direita
    drawTree(renderer, 650, 500);  // Árvore 3 na direita

    // Atualizar a janela com o novo conteúdo
    SDL_RenderPresent(renderer);

    // Manter a janela aberta por 10 segundos
    SDL_Delay(10000);

    // Limpar e fechar
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}