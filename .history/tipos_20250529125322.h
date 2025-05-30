#ifndef TIPOS_H
#define TIPOS_H
#include <stdbool.h>

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
    int indice;
    int comprimento;
    TABELA *tabelas;
} *STACKTABS;

// Estrutura que agrupa o estado do jogo, o tabuleiro e ainda a stack de tabelas
typedef struct
{
    ESTADO estado;
    TABELA tab;
    STACKTABS stackTabs;
    TABELA solution;
} GAME;

typedef struct Coordenada
{
    int l;
    int c;
} *COORDENADA;

typedef struct CQueue
{
    int tamanho;
    int capacidade;
    int front; // Indice do proximo elemento a sair
    COORDENADA *list;
} *CQUEUE; // Queue de coordenadas

#endif // TIPOS_H
