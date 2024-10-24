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

    // Desenhar a folhagem inferior da árvore (verde mais escuro logo acima do tronco)
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // Verde mais escuro (RGB: 0, 100, 0)
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

    // Inicializar posição do player
    SDL_Rect player = { 390, 550, 20, 40 }; // Player no meio da estrada

    // Variáveis de controle do jogo
    int running = 1;
    SDL_Event event;
    int speed = 5;

    // Capturar o tempo inicial
    Uint32 startTime = SDL_GetTicks();

    while (running) {
        // Verificar se já se passaram 25 segundos (25000 milissegundos)
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime > 25000) {
            running = 0; // Sair do loop após 25 segundos
        }

        // Processar eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Mover o player para a esquerda
                        player.x -= speed;
                        if (player.x < 200) player.x = 200; // Limite da estrada à esquerda
                        break;
                    case SDLK_RIGHT:
                        // Mover o player para a direita
                        player.x += speed;
                        if (player.x + player.w > 600) player.x = 600 - player.w; // Limite da estrada à direita
                        break;
                }
            }
        }

        // Limpar a tela
        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Verde para as laterais
        SDL_RenderClear(renderer);

        // Redesenhar as laterais (verde)
        SDL_RenderFillRect(renderer, &leftRect);
        SDL_RenderFillRect(renderer, &rightRect);

        // Redesenhar a estrada (cinza)
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Cinza para a estrada
        SDL_RenderFillRect(renderer, &middleRect);

        // Desenhar um retângulo branco onde estava a árvore à esquerda
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Branco (RGB: 255, 255, 255)
        SDL_Rect whiteRect = { 50, 100, 70, 100 };  // Tamanho similar ao da árvore
        SDL_RenderFillRect(renderer, &whiteRect);

        // Desenhar um quadrado branco ao lado direito do retângulo
        SDL_Rect whiteSquare = { 110, 140, 60, 60 };  // Quadrado com 80x80 ao lado direito do retângulo
        SDL_RenderFillRect(renderer, &whiteSquare);

        // Desenhar uma porta marrom
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrom (RGB: 139, 69, 19)
        SDL_Rect door = { 60, 140, 40, 60 };  // Porta marrom
        SDL_RenderFillRect(renderer, &door);

        // Desenhar uma cruz preta acima do retângulo branco
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Cor preta (RGB: 0, 0, 0)

        // Coordenadas para a cruz
        int crossX = 80; // X central da cruz
        int crossY = 80; // Y central da cruz

        // Linha vertical da cruz
        SDL_RenderDrawLine(renderer, crossX, crossY - 10, crossX, crossY + 20); // Linha vertical

        // Linha horizontal da cruz
        SDL_RenderDrawLine(renderer, crossX - 10, crossY, crossX + 10, crossY); // Linha horizontal

        // Redesenhar as árvores restantes
        drawTree(renderer, 50, 300);
        drawTree(renderer, 50, 500);
        drawTree(renderer, 650, 100);
        drawTree(renderer, 650, 300);
        drawTree(renderer, 650, 500);

        // Desenhar uma faixa horizontal de pedestre seccionada na parte superior cinza
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Branco (RGB: 255, 255, 255)
        for (int i = 0; i < 400; i += 20) {
            SDL_Rect pedestrian = { 200 + i, 50, 10, 100 }; // Faixa de pedestre seccionada
            SDL_RenderFillRect(renderer, &pedestrian);
        }

        // Desenhar o player (retângulo vermelho)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Vermelho (RGB: 255, 0, 0)
        SDL_RenderFillRect(renderer, &player);

        // Atualizar a tela
        SDL_RenderPresent(renderer);

        // Controlar a taxa de atualização
        SDL_Delay(16); // Aproximadamente 60 FPS
    }

    // Limpar e fechar
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
