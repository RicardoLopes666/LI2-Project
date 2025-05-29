#ifndef CORES_H
#define CORES_H

// Reset all attributes
#define REINICIAR "\033[0m"

// Text colors
#define PRETO "\033[30m"
#define VERMELHO "\033[31m"
#define VERDE "\033[32m"
#define AMARELO "\033[33m"
#define AZUL "\033[34m"
#define MAGENTA "\033[35m"
#define CIANO "\033[36m"
#define BRANCO "\033[37m"

// Background colors
#define FUNDO_PRETO "\033[40m"
#define FUNDO_VERMELHO "\033[41m"
#define FUNDO_VERDE "\033[42m"
#define FUNDO_AMARELO "\033[43m"
#define FUNDO_AZUL "\033[44m"
#define FUNDO_MAGENTA "\033[45m"
#define FUNDO_CIANO "\033[46m"
#define FUNDO_BRANCO "\033[47m"

// Text effects
#define NEGRITO "\033[1m"
#define TENUE "\033[2m"
#define ITALICO "\033[3m"
#define SUBLINHADO "\033[4m"
#define PISCAR "\033[5m"
#define INVERSO "\033[7m"
#define ESCONDIDO "\033[8m"

// Game-specific color combinations
#define COR_BOAS_VINDAS CIANO NEGRITO
#define COR_ERRO VERMELHO NEGRITO
#define COR_SUCESSO VERDE NEGRITO
#define COR_DICA AMARELO
#define COR_COMANDO AZUL NEGRITO
#define COR_COORDENADA CIANO
#define CELULA_RISCADA VERMELHO NEGRITO
#define LETRA_MAIUSCULA NEGRITO FUNDO_BRANCO PRETO
#define LETRA_NORMAL BRANCO

// Clear screen
#define LIMPAR_ECRA "\033[2J\033[H"

#endif // CORES_H