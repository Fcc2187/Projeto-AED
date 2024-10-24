#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INIMIGOS 100
#define DISTANCIA_MINIMA 50 // Distância mínima entre as árvores

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

    // Inicializar a SDL_ttf
    if (TTF_Init() == -1) {
        printf("Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/noto/NotoColorEmoji.ttf", 24);
    if (!font) {
        printf("Erro ao carregar a fonte: %s\n", TTF_GetError());
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
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Criar um renderizador associado à janela
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erro ao criar renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
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

    // Inicializar a semente para números aleatórios
    srand(time(NULL));

    // Posições fixas das árvores (X e Y) - agora com 10 árvores
    int posicoesArvores[10] = { 50, 50, 50, 650, 650, 650, 50, 650, 50, 650 }; // X fixo para a esquerda e direita
    int alturasArvores[10] = { 100, 200, 300, 100, 200, 300, 400, 400, 500, 500 }; // Alturas fixas das árvores

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

        // Desenhar as árvores estáticas (emojis de árvore)
        SDL_Color white = { 255, 255, 255, 255 };
        SDL_Surface* surfaceTree = TTF_RenderUTF8_Blended(font, "🌳", white);
        SDL_Texture* treeTexture = SDL_CreateTextureFromSurface(renderer, surfaceTree);
        SDL_FreeSurface(surfaceTree);

        // Posicionamento das árvores
        for (int i = 0; i < 10; i++) { // 10 árvores no total
            SDL_Rect treeRect = { posicoesArvores[i], alturasArvores[i], 50, 80 }; // Ajustar a posição e o tamanho da árvore
            SDL_RenderCopy(renderer, treeTexture, NULL, &treeRect);
        }

        SDL_DestroyTexture(treeTexture);

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

        // Desenhar os inimigos (emoji de bicicleta 🚲)
        atual = filaInimigos.frente;
        while (atual != NULL) {
            SDL_Color white = { 255, 255, 255, 255 };
            SDL_Surface* surfaceBike = TTF_RenderUTF8_Blended(font, "🚲", white);
            SDL_Texture* bikeTexture = SDL_CreateTextureFromSurface(renderer, surfaceBike);
            SDL_FreeSurface(surfaceBike);

            SDL_Rect bikeRect = { atual->x, atual->y, 30, 30 }; // Tamanho da bicicleta
            SDL_RenderCopy(renderer, bikeTexture, NULL, &bikeRect);

            SDL_DestroyTexture(bikeTexture);
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
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
