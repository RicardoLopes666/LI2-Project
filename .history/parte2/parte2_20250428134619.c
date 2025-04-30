#include <stdlib.h>
#include <stdbool.h>
#include "../tipos.h"
#include "../parte1/parte1.h"
#include <ctype.h> // Para usar a função isupper
#include <stdio.h>
#include "../parte3/parte3.h"

// _________ Funções utilizadas para voltar atrás -> comando 'd' ____________

bool initStackTabs(STACKTABS s)
{
    s->capacidade = 0;
    s->comprimento = 0;
    s->tabelas = malloc(sizeof(TABELA) * 3);
    if (s->tabelas == NULL)
        return false;
    return true;
}

// Função que insere uma tabela ao array de tabelas
bool insereTabela(STACKTABS s, TABELA tab)
{
    if (s->comprimento >= s->capacidade)
    {
        s->capacidade = s->capacidade == 0 ? 1 : 2 * s->capacidade;
        TABELA *new;
        new = realloc(s->tabelas, s->capacidade * sizeof(TABELA));
        if (new == NULL)
            return false;
        s->tabelas = new;
    }
    s->tabelas[s->comprimento++] = tab;
    return true;
}

TABELA copiarTabela(TABELA t); // Para poder usar em deleteTabela

bool deleteTabela(GAME *game)
{
    if (game->stackTabs->comprimento <= 1) // Dá erro se tentar eliminar a tabela inicial
        return false;
    freeTabela(game->stackTabs->tabelas[--game->stackTabs->comprimento]); // Liberta o espaço da tabela
    freeTabela(game->tab);
    game->tab = copiarTabela(game->stackTabs->tabelas[game->stackTabs->comprimento - 1]);
    return true;
}

// Função que faz uma cópia do elementos de uma tabela para uma outra criada com malloc e devolve o novo endereço
TABELA copiarTabela(TABELA t)
{
    TABELA new = malloc(sizeof(struct Tabela));
    if (new == NULL)
    {
        return NULL;
    }
    new->c = t->c;
    new->l = t->l;
    new->tabela = malloc(t->l * sizeof(char *));
    if (new->tabela == NULL)
    {
        free(new);
        return NULL;
    }
    for (int i = 0; i < t->l; i++)
    {
        new->tabela[i] = malloc((t->c + 1) * sizeof(char)); // + 1 para colocar o '\0'
        if (new->tabela[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(t->tabela[i]);
            }
            free(new->tabela);
            free(new);
            return NULL;
        }
        for (int j = 0; j <= t->c; j++)
        {
            if (j == t->c)
                new->tabela[i][j] = '\0';
            else
                new->tabela[i][j] = t->tabela[i][j];
        }
    }
    return new;
}

// Liberta a memória alocada para a lista de tabuleiros
void freeStackTabs(STACKTABS s)
{
    if (s != NULL)
    {
        for (int i = 0; i < s->comprimento; i++)
        {
            freeTabela(s->tabelas[i]);
        }
        free(s->tabelas);
        free(s);
    }
}

// _____ Funçãoes utilizadas para verificar as restrições -> comando 'v'______

// Função auxiliar para verificar se uma célula está dentro dos limites do tabuleiro
bool dentroDosLimites(TABELA t, int linha, int coluna)
{
    return linha >= 0 && linha < t->l && coluna >= 0 && coluna < t->c;
}

// Função que verifica se todas as casas ao redor de uma casa riscada estão pintadas de branco
// Retorna o número de restrições violadas e preenche o array `restricoes` com as coordenadas
int verificaRiscadaVizinhasBrancas(TABELA t, int linha, int coluna, int restricoes[][2])
{
    if (!dentroDosLimites(t, linha, coluna) || t->tabela[linha][coluna] != '#')
    {
        return 0; // A célula não é uma casa riscada
    }

    int direcoes[4][2] = {
        {-1, 0}, // Cima
        {1, 0},  // Baixo
        {0, -1}, // Esquerda
        {0, 1}   // Direita
    };

    int count = 0; // Contador de restrições violadas

    for (int i = 0; i < 4; i++)
    {
        int novaLinha = linha + direcoes[i][0];
        int novaColuna = coluna + direcoes[i][1];

        if (dentroDosLimites(t, novaLinha, novaColuna))
        {
            if (!isupper(t->tabela[novaLinha][novaColuna]))
            {
                // Adiciona a coordenada da restrição violada ao array
                restricoes[count][0] = novaLinha;
                restricoes[count][1] = novaColuna;
                count++;
            }
        }
    }

    return count; // Retorna o número de restrições violadas
}

// Função que verifica se existe alguma letra igual (maiúscula ou minúscula) na mesma linha ou coluna de uma casa branca
bool verificaLetraIgualLinhaColuna(TABELA t, int linha, int coluna)
{
    if (!dentroDosLimites(t, linha, coluna) || !isupper(t->tabela[linha][coluna]))
    {
        return false; // A célula não é uma casa branca
    }

    char letraMaiuscula = t->tabela[linha][coluna];
    char letraMinuscula = tolower(letraMaiuscula); // Converte a letra para minúscula

    // Verifica a linha
    for (int j = 0; j < t->c; j++)
    {
        if (j != coluna && (t->tabela[linha][j] == letraMaiuscula || t->tabela[linha][j] == letraMinuscula))
        {
            return true; // Encontrou uma letra igual (maiúscula ou minúscula) na mesma linha
        }
    }

    // Verifica a coluna
    for (int i = 0; i < t->l; i++)
    {
        if (i != linha && (t->tabela[i][coluna] == letraMaiuscula || t->tabela[i][coluna] == letraMinuscula))
        {
            return true; // Encontrou uma letra igual (maiúscula ou minúscula) na mesma coluna
        }
    }

    return false; // Não encontrou letras iguais (maiúsculas ou minúsculas) na mesma linha ou coluna
}

// Função que imprime as restrições do jogo caso estas existam
bool verificaRestricoes(TABELA t, bool escreve)
{
    bool temRestricoes = false;
    int contaRestricoes = 0;
    // Percorre todas as células do tabuleiro
    for (int i = 0; i < t->l; i++)
    {
        for (int j = 0; j < t->c; j++)
        {
            // Verifica restrições para casas riscadas
            if (t->tabela[i][j] == '#')
            {
                int restricoes[4][2]; // Máximo de 4 vizinhos
                int numRestricoes = verificaRiscadaVizinhasBrancas(t, i, j, restricoes);

                if (numRestricoes > 0)
                {
                    temRestricoes = true;
                    if (escreve)
                    {
                        printf("\n---- Restrição nº %d ----\n", contaRestricoes + 1);
                        printf("Casa riscada em (%c%d) tem restrições violadas (apenas deveria ter casas brancas na sua vizinhança) nas seguintes coordenadas:\n", 'a' + j, i + 1);

                        for (int k = 0; k < numRestricoes; k++)
                        {
                            printf("  - Coluna: %c, Linha: %d\n", 'a' + restricoes[k][1], restricoes[k][0] + 1);
                        }
                    }
                    contaRestricoes++;
                }
            }

            // Verifica restrições para casas brancas
            if (isupper(t->tabela[i][j]))
            {
                if (verificaLetraIgualLinhaColuna(t, i, j))
                {
                    temRestricoes = true;
                    printf("\n---- Restrição nº %d ----\n", contaRestricoes + 1);
                    printf("Casa branca em (%c%d) tem restrições violadas (letra repetida na mesma linha ou coluna).\n", 'a' + j, i + 1);
                    contaRestricoes++;
                }
            }
        }
    }

    // Função que chama as funções auxiliares necessarias para verificar se existem caminhos ortogonais e trata as resppetivas mensagens no terminal
    if (!trataCaminhoOrtogonal(t, &contaRestricoes, &temRestricoes))
    {
        return false;
    }

    if (!temRestricoes)
    {
        printf("Nenhuma restrição foi violada no tabuleiro.\n");
    }

    return temRestricoes;
}