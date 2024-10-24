#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_INIMIGOS 100

// Estrutura para um inimigo
typedef struct Inimigo {
    int x, y, w, h;
    struct Inimigo* prox;
} Inimigo;

// Estrutura para a fila de inimigos
typedef struct {
    Inimigo* frente;
    Inimigo* tras;
} FilaInimigos;

// Função para inicializar a fila
void inicializarFila(FilaInimigos* fila) {
    fila->frente = fila->tras = NULL;
}

// Função para adicionar inimigo na fila
void adicionarInimigo(FilaInimigos* fila, int x, int y) {
    Inimigo* novoInimigo = (Inimigo*)malloc(sizeof(Inimigo));
    novoInimigo->x = x;
    novoInimigo->y = y;
    novoInimigo->w = 20;
    novoInimigo->h = 20;
    novoInimigo->prox = NULL;

    if (fila->tras == NULL) {
        fila->frente = fila->tras = novoInimigo;
    } else {
        fila->tras->prox = novoInimigo;
        fila->tras = novoInimigo;
    }
}

// Função para remover inimigo da fila (quando sair da tela)
void removerInimigo(FilaInimigos* fila) {
    if (fila->frente != NULL) {
        Inimigo* temp = fila->frente;
        fila->frente = fila->frente->prox;
        free(temp);
        if (fila->frente == NULL) {
            fila->tras = NULL;
        }
    }
}

// Função para verificar colisão
int verificarColisao(SDL_Rect* player, Inimigo* inimigo) {
    if (player->x < inimigo->x + inimigo->w &&
        player->x + player->w > inimigo->x &&
        player->y < inimigo->y + inimigo->h &&
        player->y + player->h > inimigo->y) {
        return 1; // Houve colisão
    }
    return 0;
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

    // Inicializar a fila de inimigos
    FilaInimigos filaInimigos;
    inicializarFila(&filaInimigos);

    // Inicializar posição do player
    SDL_Rect player = { 390, 550, 20, 40 }; // Player no meio da estrada

    // Variáveis de controle do jogo
    int running = 1;
    SDL_Event event;
    int speed = 5;
    int tempoParaNovoInimigo = 0; // Controlar o tempo para criar novos inimigos

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
        SDL_Rect leftRect = { 0, 0, 200, 600 };
        SDL_Rect rightRect = { 600, 0, 200, 600 };
        SDL_RenderFillRect(renderer, &leftRect);
        SDL_RenderFillRect(renderer, &rightRect);

        // Redesenhar a estrada (cinza)
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Cinza para a estrada
        SDL_Rect middleRect = { 200, 0, 400, 600 };
        SDL_RenderFillRect(renderer, &middleRect);

        // Desenhar o player (retângulo vermelho)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Vermelho (RGB: 255, 0, 0)
        SDL_RenderFillRect(renderer, &player);

        // Criar novos inimigos a cada 1 segundo (1000 ms)
        if (tempoParaNovoInimigo > 200) {
            int inimigoX = 200 + rand() % 380; // Posição aleatória na estrada (parte cinza)
            adicionarInimigo(&filaInimigos, inimigoX, 0); // Adicionar inimigo no topo
            tempoParaNovoInimigo = 0; // Resetar o tempo
        }

        // Atualizar posição dos inimigos
        Inimigo* atual = filaInimigos.frente;
        while (atual != NULL) {
            atual->y += 5; // Descer o inimigo
            if (atual->y > 600) {
                removerInimigo(&filaInimigos); // Remover inimigos que saíram da tela
            }
            if (verificarColisao(&player, atual)) {
                running = 0; // Fim de jogo se colidir com o player
            }
            atual = atual->prox;
        }

        // Desenhar os inimigos
        atual = filaInimigos.frente;
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul para os inimigos
        while (atual != NULL) {
            SDL_Rect inimigoRect = { atual->x, atual->y, atual->w, atual->h };
            SDL_RenderFillRect(renderer, &inimigoRect);
            atual = atual->prox;
        }

        // Atualizar a tela
        SDL_RenderPresent(renderer);

        // Controlar a taxa de atualização
        SDL_Delay(16); // Aproximadamente 60 FPS

        // Incrementar o tempo para novos inimigos
        tempoParaNovoInimigo += 16;
    }

    // Limpar e fechar
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
