#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_mixer.h>

Mix_Chunk *somColisao = NULL;
Mix_Music *musica = NULL;
#define MAX_INIMIGOS 100
#define DISTANCIA_MINIMA 50 // Distância mínima entre as árvores

// Estrutura para um inimigo
typedef struct Inimigo {
    int x, y, w, h;
    struct Inimigo* prox;
} Inimigo;

// Estrutura para a fila de inimigos
typedef struct FilaInimigos{
    Inimigo* frente;
    Inimigo* tras;
} FilaInimigos;

// Estrutura para uma manga
typedef struct Manga {
    int x, y, w, h;
    struct Manga* prox;
} Manga;

// Estrutura para a fila de mangas
typedef struct FilaManga{
    Manga* frente;
    Manga* tras;
} FilaManga;

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


// Função para inicializar a manga
void inicializarManga(FilaManga* fila) {
    fila->frente = fila->tras = NULL;
}

// Função para adicionar manga na fila
void adicionarManga(FilaManga* fila, int x, int y) {
    Manga* novaManga = (Manga*)malloc(sizeof(Manga));
    novaManga->x = x;
    novaManga->y = y;
    novaManga->w = 20;
    novaManga->h = 20;
    novaManga->prox = NULL;

    if (fila->tras == NULL) {
        fila->frente = fila->tras = novaManga;
    } else {
        fila->tras->prox = novaManga;
        fila->tras = novaManga;
    }
}

// Função para remover manga da fila (quando sair da tela)
void removerManga(FilaManga* fila) {
    if (fila->frente != NULL) {
        Manga* temp = fila->frente;
        fila->frente = fila->frente->prox;
        free(temp);
        if (fila->frente == NULL) {
            fila->tras = NULL;
        }
    }
}

// Função para verificar colisão da manga com o player
int mangaColisao(SDL_Rect* player, Manga* manga) {
    if (player->x < manga->x + manga->w &&
        player->x + player->w > manga->x &&
        player->y < manga->y + manga->h &&
        player->y + player->h > manga->y) {
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

        SDL_Texture* tituloTexture = SDL_CreateTextureFromSurface(renderer, surfaceTitulo);
        SDL_FreeSurface(surfaceTitulo);

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

        SDL_Texture* iniciarTexture = SDL_CreateTextureFromSurface(renderer, surfaceIniciar);
        SDL_FreeSurface(surfaceIniciar);

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

        SDL_Texture* sairTexture = SDL_CreateTextureFromSurface(renderer, surfaceSair);
        SDL_FreeSurface(surfaceSair);

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

// Função para inicializar SDL e TTF, criar janela e renderizador
int inicializarSDL(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) return 1;
    if (TTF_Init() == -1) {
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erro ao inicializar SDL_Mixer: %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    musica = Mix_LoadMUS("musica/trilhasonora.mp3");  // Substitua pelo caminho correto
    if (!musica) {
        printf("Erro ao carregar música: %s\n", Mix_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    somColisao = Mix_LoadWAV("musica/comer.mp3"); // Substitua pelo caminho correto
    if (!somColisao) {
        printf("Erro ao carregar som de colisão: %s\n", Mix_GetError());
    }

    *font = TTF_OpenFont("/usr/share/fonts/truetype/noto/NotoColorEmoji.ttf", 24); // Fonte para Linux
    if (!*font) return 1;

    *window = SDL_CreateWindow("Tela jogo principal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!*window) {
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        SDL_DestroyWindow(*window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    return 0;
}

// Função para desenhar objetos estáticos (árvores, igreja, playground e estrada)
void desenharObjetosFixos(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect* playgroundRect) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surfaceTree = TTF_RenderUTF8_Blended(font, "🌳", white);
    SDL_Texture* treeTexture = SDL_CreateTextureFromSurface(renderer, surfaceTree);
    SDL_FreeSurface(surfaceTree);

    int posicoesArvores[10] = {50, 50, 650, 650, 650, 50, 650, 50};
    int alturasArvores[10] = {180, 300, 100, 200, 300, 400, 400, 500};

    for (int i = 0; i < 8; i++) {
        SDL_Rect treeRect = {posicoesArvores[i], alturasArvores[i], 50, 80};
        SDL_RenderCopy(renderer, treeTexture, NULL, &treeRect);
    }
    SDL_DestroyTexture(treeTexture);

    SDL_Surface* surfaceChurch = TTF_RenderUTF8_Blended(font, "⛪️", white);
    SDL_Texture* churchTexture = SDL_CreateTextureFromSurface(renderer, surfaceChurch);
    SDL_FreeSurface(surfaceChurch);
    SDL_Rect churchRect = {10, 20, 130, 130};
    SDL_RenderCopy(renderer, churchTexture, NULL, &churchRect);
    SDL_DestroyTexture(churchTexture);

    SDL_Surface* surfacePlayground = TTF_RenderUTF8_Blended(font, "🛝", white);
    SDL_Texture* playgroundTexture = SDL_CreateTextureFromSurface(renderer, surfacePlayground);
    SDL_FreeSurface(surfacePlayground);
    SDL_RenderCopy(renderer, playgroundTexture, NULL, playgroundRect);
    SDL_DestroyTexture(playgroundTexture);

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_Rect middleRect = {200, 0, 400, 600};
    SDL_RenderFillRect(renderer, &middleRect);
}

// Função para atualizar o movimento e renderização do player
void atualizarPlayer(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect* player, int playerDirection) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surfacePlayer = TTF_RenderUTF8_Blended(font, "🚴", white);
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, surfacePlayer);
    SDL_FreeSurface(surfacePlayer);

    SDL_Rect playerRect = {player->x, player->y, 30, 40};
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
    SDL_DestroyTexture(playerTexture);
}

// Função para atualizar inimigos, adicionar novos e verificar colisões
void atualizarInimigos(SDL_Renderer* renderer, TTF_Font* font, FilaInimigos* filaInimigos, SDL_Rect* player, int* running, int* tempoParaNovoInimigo) {
    if (*tempoParaNovoInimigo > 150) {
        int inimigoX = 200 + rand() % 380;
        adicionarInimigo(filaInimigos, inimigoX, 0);
        *tempoParaNovoInimigo = 0;
    }

    Inimigo* atual = filaInimigos->frente;
    while (atual != NULL) {
        atual->y += 5;
        if (atual->y > 600) removerInimigo(filaInimigos);
        if (verificarColisao(player, atual)) *running = 0;
        atual = atual->prox;
    }

    atual = filaInimigos->frente;
    while (atual != NULL) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surfaceBike = TTF_RenderUTF8_Blended(font, "🏃‍♂️", white);
        SDL_Texture* bikeTexture = SDL_CreateTextureFromSurface(renderer, surfaceBike);
        SDL_FreeSurface(surfaceBike);

        SDL_Rect bikeRect = {atual->x, atual->y, 50, 50};
        SDL_RenderCopy(renderer, bikeTexture, NULL, &bikeRect);
        SDL_DestroyTexture(bikeTexture);
        atual = atual->prox;
    }
}

void atualizarMangas(SDL_Renderer* renderer, TTF_Font* font, FilaManga* filaManga, SDL_Rect* player, int* running, int* tempoParaNovaManga, int *contadorMangas) {
    if (*tempoParaNovaManga > 150) {
        int mangaX = 200 + rand() % 380;
        adicionarManga(filaManga, mangaX, 0);
        *tempoParaNovaManga = 0;
    }

    Manga* atual = filaManga->frente;
    while (atual != NULL) {
        atual->y += 5;
        if (atual->y > 600) removerManga(filaManga);
        if (mangaColisao(player, atual)){
            *contadorMangas += 1;
            Mix_PlayChannel(-1, somColisao, 0); // Toca o som de colisão
            removerManga(filaManga); // Remove a manga após a colisão
         }
        atual = atual->prox;
    }

    atual = filaManga->frente;
    while (atual != NULL) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surfaceBike = TTF_RenderUTF8_Blended(font, "🥭", white);
        SDL_Texture* bikeTexture = SDL_CreateTextureFromSurface(renderer, surfaceBike);
        SDL_FreeSurface(surfaceBike);

        SDL_Rect bikeRect = {atual->x, atual->y, 30, 30};
        SDL_RenderCopy(renderer, bikeTexture, NULL, &bikeRect);
        SDL_DestroyTexture(bikeTexture);
        atual = atual->prox;
    }
}

// Loop principal do jogo
void loopJogo(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect* player) {
    FilaInimigos filaInimigos;
    inicializarFila(&filaInimigos);
    FilaManga filaMangas;
    inicializarManga(&filaMangas);

    int running = 1;
    int speed = 5;
    int tempoParaNovoInimigo = 0;
    int tempoParaNovaManga = 0;
    int contadorSegundos = 0;
    int contadorMangas = 0;
    Uint32 startTime = SDL_GetTicks();

    SDL_Event event;
    SDL_Rect playgroundRect = {650, 500, 80, 80};
    int playerDirection = 0;

    while (running) {
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime / 1000 > contadorSegundos) {
            contadorSegundos++;
            printf("Tempo decorrido: %d segundos\n", contadorSegundos);
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        player->x -= speed;
                        if (player->x < 200) player->x = 200;
                        playerDirection = 1;
                        break;
                    case SDLK_RIGHT:
                        player->x += speed;
                        if (player->x + player->w > 600) player->x = 600 - player->w;
                        playerDirection = 0;
                        break;
                }
            }
        }


        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
        SDL_RenderClear(renderer);

        desenharObjetosFixos(renderer, font, &playgroundRect);
        atualizarPlayer(renderer, font, player, playerDirection);
        atualizarInimigos(renderer, font, &filaInimigos, player, &running, &tempoParaNovoInimigo);
        atualizarMangas(renderer, font, &filaMangas, player, &running, &tempoParaNovaManga, &contadorMangas);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
        tempoParaNovoInimigo += 12;
        tempoParaNovaManga += 3;
    }

    int conTotal = contadorSegundos + contadorMangas;
    FILE *arquivo = fopen("tempo_final.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, " %d \n", conTotal);
        fclose(arquivo);
    }
}


void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Arrays temporários para dividir e mesclar
    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Mesclando os arrays temporários de volta ao original
    while (i < n1 && j < n2) {
        if (L[i] >= R[j]) {  // Ordenando em ordem decrescente
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copia os elementos restantes de L[], se houver
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copia os elementos restantes de R[], se houver
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void salvarPontuacao(const char* nome, int pontuacao) {
    FILE* arquivo = fopen("ranking.txt", "a");
    if (arquivo) {
        fprintf(arquivo, "%s: %d\n", nome, pontuacao);
        fclose(arquivo);
    }
}

void solicitarNome(char* nomeJogador, int tamanho) {
    printf("Digite seu nome: ");
    fgets(nomeJogador, tamanho, stdin);
    nomeJogador[strcspn(nomeJogador, "\n")] = 0;
}

// Função para ler os números do arquivo tempo_final.txt
void lerTempoFinal(int* numbers, int* count) {
    FILE* inputFile = fopen("tempo_final.txt", "r");
    if (inputFile == NULL) {
        *count = 0; // Se não conseguir abrir o arquivo, define count como 0
        return;
    }

    // Leitura dos números do arquivo
    while (fscanf(inputFile, "%d", &numbers[*count]) != EOF) {
        (*count)++;
    }

    fclose(inputFile);
}

// Função para ordenar os números e escrever no arquivo ranking.txt
void ordenarEEscreverRanking(int* numbers, int count) {
    FILE* outputFile = fopen("ranking.txt", "w");
    if (outputFile == NULL) {
        return;
    }

    // Ordenação dos números com Merge Sort
    mergeSort(numbers, 0, count - 1);

    // Escrita dos números ordenados no arquivo ranking.txt
    for (int i = 0; i < count; i++) {
        fprintf(outputFile, "%d\n", numbers[i]);
    }

    fclose(outputFile);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;

    if (inicializarSDL(&window, &renderer, &font) != 0) {
        printf("Erro ao inicializar SDL\n");
        return 1;
    }

    Mix_Music* musicaFundo = Mix_LoadMUS("musica/trilhasonora.mp3");
    if (!musicaFundo) {
        printf("Erro ao carregar música: %s\n", Mix_GetError());
    } else {
        Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
        Mix_PlayMusic(musicaFundo, 0);  // Toca a música indefinidamente
    }

    if (!exibirMenu(renderer, font)) return 0;

    SDL_Rect player = {390, 550, 30, 40};
    int pontuacao = 0;

    // Iniciar o loop do jogo
    loopJogo(renderer, font, &player);

    // Aumentar a pontuação (exemplo)
    pontuacao += 100; // Suponha que o jogador ganhou 100 pontos

    // Solicitar o nome do jogador após o término do jogo
    char nomeJogador[50];
    solicitarNome(nomeJogador, sizeof(nomeJogador));

    // Salvar a pontuação no arquivo
    salvarPontuacao(nomeJogador, pontuacao);

    Mix_FreeChunk(somColisao);
    Mix_HaltMusic();
    Mix_FreeMusic(musica);
    Mix_CloseAudio(); 
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Leitura dos números do arquivo e escrita no ranking
    int numbers[100];
    int count = 0;
    lerTempoFinal(numbers, &count);
    ordenarEEscreverRanking(numbers, count);

    return 0;
}
