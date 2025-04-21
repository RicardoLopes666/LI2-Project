#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../tipos.h"
#include "../parte2/parte2.h" // Inclui a declaração de copiarTabela

TABELA ajuda(TABELA t)
{
    // Cria uma tabela auxiliar como cópia da tabela original
    TABELA aux = copiarTabela(t);

    // Verifica as restrições do tabuleiro original e aplica as mudanças na tabela auxiliar
    for (int i = 0; i < t->l; i++)
    {
        for (int j = 0; j < t->c; j++)
        {
            // Verifica letras iguais na mesma linha ou coluna
            if (isupper(t->tabela[i][j]))
            {
                char letraMaiuscula = t->tabela[i][j];
                char letraMinuscula = tolower(letraMaiuscula);

                // Verifica a linha
                for (int k = 0; k < t->c; k++)
                {
                    if (k != j && (t->tabela[i][k] == letraMaiuscula || t->tabela[i][k] == letraMinuscula))
                    {
                        printf("A riscar a célula %c%d com letra '%c' devido à repetição na linha.\n", 'a' + k, i + 1, t->tabela[i][k]);
                        aux->tabela[i][k] = '#'; // Risca a célula na tabela auxiliar
                    }
                }

                // Verifica a coluna
                for (int k = 0; k < t->l; k++)
                {
                    if (k != i && (t->tabela[k][j] == letraMaiuscula || t->tabela[k][j] == letraMinuscula))
                    {
                        printf("A riscar célula %c%d com letra '%c' devido à repetição na coluna.\n", 'a' + j, k + 1, t->tabela[k][j]);
                        aux->tabela[k][j] = '#'; // Risca a célula na tabela auxiliar
                    }
                }
            }

            // Verifica casas riscadas que não têm todas as vizinhas brancas
            if (t->tabela[i][j] == '#')
            {
                int direcoes[4][2] = {
                    {-1, 0}, // Cima
                    {1, 0},  // Baixo
                    {0, -1}, // Esquerda
                    {0, 1}   // Direita
                };

                for (int d = 0; d < 4; d++)
                {
                    int novaLinha = i + direcoes[d][0];
                    int novaColuna = j + direcoes[d][1];

                    // Verifica se a célula vizinha está dentro dos limites do tabuleiro
                    if (novaLinha >= 0 && novaLinha < t->l && novaColuna >= 0 && novaColuna < t->c)
                    {
                        // Verifica se a célula vizinha não é uma casa riscada ('#') ou já branca ('B')
                        if (islower(t->tabela[novaLinha][novaColuna]))
                        {
                            printf("A pintar a célula %c%d de branco devido à casa riscada em %c%d.\n",
                                   'a' + novaColuna, novaLinha + 1, 'a' + j, i + 1);
                            aux->tabela[novaLinha][novaColuna] = toupper(t->tabela[novaLinha][novaColuna]); // Pinta de branco na tabela auxiliar
                        }
                    }
                }
            }
        }
    }

    return aux; // Retorna a tabela auxiliar com as alterações aplicadas
}