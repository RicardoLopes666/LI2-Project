#ifndef TIPOS_H
#define TIPOS_H

// Struct para controlar o estado do programa (se ele deve continuar ou parar)
typedef struct estado
{
    bool looping;
} ESTADO;

// Struct que representa o tabuleiro
typedef struct Tabela
{
    int l;
    int c;
    char **tabela;
} *TABELA;

typedef struct StackTabs
{
    int capacidade;
    int comprimento;
    TABELA *tabelas;
} *STACKTABS;

// Estrutura que agrupa o estado do jogo e o tabuleiro
typedef struct
{
    ESTADO estado;
    TABELA tab;
    STACKTABS stackTabs;
} GAME;

#endif // TIPOS_H