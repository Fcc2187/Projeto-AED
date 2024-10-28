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




// Rodar o menu
int exibirMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Event event;
    int menuRunning = 1;

    // Definindo o botão "Jogar" com dimensões reduzidas
    SDL_Rect botaoJogar = { 300, 260, 180, 70 }; // Dimensões do botão ajustadas
    SDL_Rect botaoSair = { 300, 360, 180, 70 }; // Botão "SAIR" abaixo do botão "Jogar", com mais espaço

    while (menuRunning) {
        // Processar eventos do menu
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0; // Saia do jogo
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // Verifica se o clique foi dentro do botão "Jogar"
                if (mouseX >= botaoJogar.x && mouseX <= botaoJogar.x + botaoJogar.w &&
                    mouseY >= botaoJogar.y && mouseY <= botaoJogar.y + botaoJogar.h) {
                    menuRunning = 0; // Inicie o jogo
                }
                // Verifica se o clique foi dentro do botão "Sair"
                else if (mouseX >= botaoSair.x && mouseX <= botaoSair.x + botaoSair.w &&
                         mouseY >= botaoSair.y && mouseY <= botaoSair.y + botaoSair.h) {
                    return 0; // Saia do jogo
                }
            }
        }

        // Limpar a tela com a cor verde
        SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255); // Cor de fundo verde
        SDL_RenderClear(renderer);

        // Texto do título "JAQUEIRUN" em emojis no centro superior
        SDL_Color white = { 255, 255, 255, 255 };
        const char* emojisTitulo = "🇯 🇦 🇶 🇺 🇪 🇮 🇷 🇺 🇳";
        
        // Criar uma superfície para os emojis do título
        SDL_Surface* surfaceTitulo = TTF_RenderUTF8_Blended(font, emojisTitulo, white);
        if (!surfaceTitulo) {
            printf("Erro ao criar superfície do título: %s\n", TTF_GetError());
        }

        SDL_Texture* tituloTexture = SDL_CreateTextureFromSurface(renderer, surfaceTitulo);
        SDL_FreeSurface(surfaceTitulo);
        if (!tituloTexture) {
            printf("Erro ao criar textura do título: %s\n", SDL_GetError());
        }

        // Obter as dimensões da textura do título
        int tituloLargura, tituloAltura;
        SDL_QueryTexture(tituloTexture, NULL, NULL, &tituloLargura, &tituloAltura);

        // Reduzir as dimensões do título e movê-lo mais para baixo
        float scaleTitulo = 0.3; // Ajuste a escala conforme necessário
        SDL_Rect tituloRect = {
            (800 - (tituloLargura * scaleTitulo)) / 2, // Centraliza horizontalmente
            40, // Margem do topo ajustada para mover o título para baixo
            (int)(tituloLargura * scaleTitulo),
            (int)(tituloAltura * scaleTitulo)
        };

        // Renderizar o título
        SDL_RenderCopy(renderer, tituloTexture, NULL, &tituloRect);
        SDL_DestroyTexture(tituloTexture);

        // Texto "INICIAR" em emojis
        const char* emojisIniciar = "🇮 🇳 🇮 🇨 🇮 🇦 🇷";
        
        // Criar uma superfície para os emojis "INICIAR"
        SDL_Surface* surfaceIniciar = TTF_RenderUTF8_Blended(font, emojisIniciar, white);
        if (!surfaceIniciar) {
            printf("Erro ao criar superfície do título: %s\n", TTF_GetError());
        }

        SDL_Texture* iniciarTexture = SDL_CreateTextureFromSurface(renderer, surfaceIniciar);
        SDL_FreeSurface(surfaceIniciar);
        if (!iniciarTexture) {
            printf("Erro ao criar textura do título: %s\n", SDL_GetError());
        }

        // Obter as dimensões da textura "INICIAR"
        int iniciarLargura, iniciarAltura;
        SDL_QueryTexture(iniciarTexture, NULL, NULL, &iniciarLargura, &iniciarAltura);

        // Reduzir as dimensões do "INICIAR"
        float scaleIniciar = 0.1; // Ajuste a escala para 0.1
        SDL_Rect iniciarRect = {
            botaoJogar.x + (botaoJogar.w - (iniciarLargura * scaleIniciar)) / 2, // Centraliza horizontalmente
            botaoJogar.y - (iniciarAltura * scaleIniciar) - 10, // Posiciona acima do botão, ajustando a altura
            (int)(iniciarLargura * scaleIniciar),
            (int)(iniciarAltura * scaleIniciar)
        };

        // Renderizar o texto "INICIAR"
        SDL_RenderCopy(renderer, iniciarTexture, NULL, &iniciarRect);
        SDL_DestroyTexture(iniciarTexture);

        // Desenhar o botão "Jogar" em cinza
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Cor cinza para o botão
        SDL_RenderFillRect(renderer, &botaoJogar);

        // Texto "SAIR" em emojis
        const char* emojisSair = "🇸 🇦 🇮 🇷";
        
        // Criar uma superfície para os emojis "SAIR"
        SDL_Surface* surfaceSair = TTF_RenderUTF8_Blended(font, emojisSair, white);
        if (!surfaceSair) {
            printf("Erro ao criar superfície do título: %s\n", TTF_GetError());
        }

        SDL_Texture* sairTexture = SDL_CreateTextureFromSurface(renderer, surfaceSair);
        SDL_FreeSurface(surfaceSair);
        if (!sairTexture) {
            printf("Erro ao criar textura do título: %s\n", SDL_GetError());
        }

        // Obter as dimensões da textura "SAIR"
        int sairLargura, sairAltura;
        SDL_QueryTexture(sairTexture, NULL, NULL, &sairLargura, &sairAltura);

        // Reduzir as dimensões do "SAIR"
        float scaleSair = 0.1; // Ajuste a escala para 0.1
        SDL_Rect sairRect = {
            botaoSair.x + (botaoSair.w - (sairLargura * scaleSair)) / 2, // Centraliza horizontalmente
            botaoSair.y - (sairAltura * scaleSair) - 10, // Posiciona acima do botão, ajustando a altura
            (int)(sairLargura * scaleSair),
            (int)(sairAltura * scaleSair)
        };

        // Renderizar o texto "SAIR"
        SDL_RenderCopy(renderer, sairTexture, NULL, &sairRect);
        SDL_DestroyTexture(sairTexture);

        // Desenhar o botão "Sair" em vermelho
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Cor vermelha para o botão
        SDL_RenderFillRect(renderer, &botaoSair);

        // Atualizar a tela
        SDL_RenderPresent(renderer);
    }

    return 1; // O jogo deve iniciar
}








//Rodar o jogo
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

    //MacBook
    //TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/Apple Color Emoji.ttc", 24);
    //Linux
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/noto/NotoColorEmoji.ttf", 24);
    
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

    if (!exibirMenu(renderer, font)) {
        // Se o menu retornar 0, o usuário decidiu sair do jogo
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Inicializar a fila de inimigos
    FilaInimigos filaInimigos;
    inicializarFila(&filaInimigos);

    // Inicializar posição do player (emoji de pessoa em movimento)
    SDL_Rect player = { 390, 550, 30, 40 }; // Player no meio da estrada
    int playerDirection = 0; // 0 para direita, 1 para esquerda

    // Variáveis de controle do jogo
    int running = 1;
    SDL_Event event;
    int speed = 5;
    int tempoParaNovoInimigo = 0; // Controlar o tempo para criar novos inimigos

    // Capturar o tempo inicial
    Uint32 startTime = SDL_GetTicks();

    // Inicializar a semente para números aleatórios
    srand(time(NULL));

    // Posições fixas das árvores (X e Y) - agora com 9 árvores
    int posicoesArvores[10] = {50, 50, 650, 650, 650, 50, 650, 50 }; // X fixo para a esquerda e direita
    int alturasArvores[10] = {180, 300, 100, 200, 300, 400, 400, 500}; // Alturas fixas das árvores

    // Definindo tamanhos dos emojis
    int playerEmojiWidth = 30; // Largura do emoji do jogador
    int playerEmojiHeight = 40; // Altura do emoji do jogador
    int bikeEmojiWidth = 50;    // Largura do emoji da bicicleta
    int bikeEmojiHeight = 50;    // Altura do emoji da bicicleta

    // Adicionar posição e altura da igreja
    int churchWidth = 130; // Largura da igreja
    int churchHeight = 130; // Altura da igreja

    // Posição do playground
    SDL_Rect playgroundRect = { 650, 500, 80, 80 }; // Ajuste a posição e tamanho do playground

    int contadorSegundos = 0;
    while (running) {
        // Verificar se já se passaram 25 segundos (25000 milissegundos)
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime > 25000) {
            running = 0; // Sair do loop após 25 segundos
        }


         if (elapsedTime / 1000 > contadorSegundos) {
        contadorSegundos++;
        printf("Tempo decorrido: %d segundos\n", contadorSegundos); // Exibir o tempo decorrido em segundos
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
                        playerDirection = 1; // Direção para a esquerda
                        break;
                    case SDLK_RIGHT:
                        // Mover o player para a direita
                        player.x += speed;
                        if (player.x + player.w > 600) player.x = 600 - player.w; // Limite da estrada à direita
                        playerDirection = 0; // Direção para a direita
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
        for (int i = 0; i < 8; i++) { // 9 árvores no total
            SDL_Rect treeRect = { posicoesArvores[i], alturasArvores[i], 50, 80 }; // Ajustar a posição e o tamanho da árvore
            SDL_RenderCopy(renderer, treeTexture, NULL, &treeRect);
        }

        SDL_DestroyTexture(treeTexture);

        // Desenhar a igreja (emoji de igreja) na posição da última árvore
        SDL_Surface* surfaceChurch = TTF_RenderUTF8_Blended(font, "⛪️", white);
        SDL_Texture* churchTexture = SDL_CreateTextureFromSurface(renderer, surfaceChurch);
        SDL_FreeSurface(surfaceChurch);
        int posicaoIgreja = 10;
        int alturaIgreja = 20;
        SDL_Rect churchRect = { posicaoIgreja, alturaIgreja, churchWidth, churchHeight }; // Posição e tamanho da igreja
        SDL_RenderCopy(renderer, churchTexture, NULL, &churchRect);
        SDL_DestroyTexture(churchTexture);
        SDL_Surface* surfacePlayground = TTF_RenderUTF8_Blended(font, "🛝", white); // Emoji de parque infantil
        SDL_Texture* playgroundTexture = SDL_CreateTextureFromSurface(renderer, surfacePlayground);
        SDL_FreeSurface(surfacePlayground);

        SDL_RenderCopy(renderer, playgroundTexture, NULL, &playgroundRect); // Posição do playground
        SDL_DestroyTexture(playgroundTexture);

        // Redesenhar a estrada (cinza)
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Cinza para a estrada
        SDL_Rect middleRect = { 200, 0, 400, 600 };
        SDL_RenderFillRect(renderer, &middleRect);

        // Desenhar o player (emoji de pessoa em movimento)
        SDL_Surface* surfacePlayer;
        surfacePlayer = TTF_RenderUTF8_Blended(font, "🏃‍♂️", white); // Emoji de pessoa correndo
        SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, surfacePlayer);
        SDL_FreeSurface(surfacePlayer);

        // Ajustar o tamanho do emoji do personagem
        SDL_Rect playerRect = { player.x, player.y, playerEmojiWidth, playerEmojiHeight }; // Ajustar a largura e altura do personagem
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
        SDL_DestroyTexture(playerTexture);

        // Criar novos inimigos a cada 1 segundo (1000 ms)
        if (tempoParaNovoInimigo > 150) {
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

        atual = filaInimigos.frente;
        while (atual != NULL) {
            SDL_Color white = { 255, 255, 255, 255 };
            SDL_Surface* surfaceBike = TTF_RenderUTF8_Blended(font, "🚴", white);
            SDL_Texture* bikeTexture = SDL_CreateTextureFromSurface(renderer, surfaceBike);
            SDL_FreeSurface(surfaceBike);

            // Ajustar o tamanho do emoji da bicicleta
            SDL_Rect bikeRect = { atual->x, atual->y, bikeEmojiWidth, bikeEmojiHeight }; // Ajustar a largura e altura da bicicleta
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

    FILE *arquivo = fopen("tempo_final.txt", "w");
if (arquivo != NULL) {
    fprintf(arquivo, "Tempo final: %d segundos\n", contadorSegundos);
    fclose(arquivo);
} else {
    printf("Erro ao abrir o arquivo!\n");
}

    // Limpar e fechar
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
