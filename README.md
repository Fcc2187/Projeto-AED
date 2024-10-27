# Projeto-AED

Projeto da Cadeira de AED do terceiro período do semestre 2024.2

## Jogo - 🌳 Jaqueirun 🌳
Nossa referência foi o Parque da Jaqueira, um dos espaços públicos ao ar livre mais visitados e conhecidos de Recife.

No Jaqueirun, você controla um pedestre caminhando pelo Parque da Jaqueira. 
Sua missão é desviar dos ciclistas que surgem pelo caminho e evitar colisões, preste bem atenção e divirta-se 🤗.

## Equipe

- André Castro - alcms@cesar.school 📩 
- Caio Lima - clb@cesar.school 📩
- Felipe Caminha - fcc3@cesar.school 📩
- Lucas Sukar - lfsw@cesar.school 📩
- Rodrigo Torres - rtmr@cesar.school 📩

## Para compilar Linux
- Atualize: sudo apt-get update
- Baixe: sudo apt-get install libsdl2-dev libsdl2-ttf-dev
- Rode: gcc code.c -o exec_jogo -lSDL2 -lSDL2_ttf
- Rode: ./exec_jogo

## Instruções para Apple Silicon
- 1- Baixe o homebrew: /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
- 2- Baixe: brew install sdl2 sdl2_ttf
- 3- troque a linha 239 para: TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/Apple Color Emoji.ttc", 24);
- 4- Rode: gcc code.c -o jogo -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf
- 5- Execute: ./jogo

## Pendências
- Adicionar Botões de Cicuitos e Ranking no Menu
- Implementar a Função de Ranking
- Estilizar o Menu
- Criar um ou dois circuitos diferentes ( a decidir ... )
- Ajustar o Contador ( em desenvolvimento )
