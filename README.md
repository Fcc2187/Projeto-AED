# Projeto-AED

Projeto da Cadeira de AED do terceiro período do semestre 2024.2

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
- 3- Rode: gcc -o game code.c -I/opt/homebrew/include/ -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf
- 4- Execute:./exec_jogo
