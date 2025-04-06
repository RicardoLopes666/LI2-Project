#include <stdio.h>
#include <stdlib.h> // Para malloc e free
#include "parte1.h"

int main()
{
    printf("Programa principal iniciado.\n");

    // Exemplo de uso das funções
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);

    printf("Tabuleiro inicializado:\n");
    mostrarTabela(t);

    riscar(t, 1, 1);
    printf("Tabuleiro após riscar (1, 1):\n");
    mostrarTabela(t);

    pintarBranco(t, 1, 1);
    printf("Tabuleiro após pintar de branco (1, 1):\n");
    mostrarTabela(t);

    freeTabela(t);
    printf("Tabuleiro liberado.\n");

    return 0;
}