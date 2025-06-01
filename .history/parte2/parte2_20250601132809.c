#include <stdlib.h>
#include <stdbool.h>
#include "../tipos.h"
#include "../parte1/parte1.h"
#include <ctype.h> // Para usar a função isupper
#include <stdio.h>
#include "../parte3/parte3.h"
#include "../colors.h"

// _________ Funções utilizadas para voltar atrás -> comando 'd' ____________

bool initStackTabs(STACKTABS s)
{
    s->capacidade = 3;
    s->indice = 0;
    s->comprimento = 0;
    s->tabelas = malloc(sizeof(TABELA) * 3);
    if (s->tabelas == NULL)
        return false;
    return true;
}

// Função que insere uma tabela ao array de tabelas
bool insereTabela(STACKTABS s, TABELA tab)
{
    if (s->indice < s->comprimento) // Neste caso tem de se que dar free das tabela que estão para a frente e adicionar a nova tabela
    {
        for (int i = s->indice; i < s->comprimento; i++)
            freeTabela(s->tabelas[i]);

        s->comprimento = s->indice;
    }

    else if (s->comprimento >= s->capacidade)
    {
        s->capacidade = s->capacidade == 0 ? 1 : 2 * s->capacidade;
        TABELA *new;
        new = realloc(s->tabelas, s->capacidade * sizeof(TABELA));
        if (new == NULL)
            return false;
        s->tabelas = new;
    }
    s->tabelas[s->comprimento++] = tab;
    s->indice++;
    return true;
}

TABELA copiarTabela(TABELA t); // Para poder usar em deleteTabela

bool deleteTabela(GAME *game)
{                                                                         // Apenas se reduz o indice caso possivel
    if (game->stackTabs->comprimento <= 1 || game->stackTabs->indice < 2) // Dá erro se tentar eliminar a tabela inicial
        return false;
    freeTabela(game->tab);
    game->tab = copiarTabela(game->stackTabs->tabelas[game->stackTabs->indice-- - 2]);
    return true;
}

copiaLinha(i, t, new)
{
    for (int j = 0; j <= t->c; j++)
    {
        if (j == t->c)
            new->tabela[i][j] = '\0';
        else
            new->tabela[i][j] = t->tabela[i][j];
    }
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
                free(new->tabela[j]);
            }
            free(new->tabela);
            free(new);
            return NULL;
        }
        copiaLinha(i, t, new);
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

void u(GAME *game, bool *comandoProcessado)
{
    if (game->stackTabs->indice >= game->stackTabs->comprimento)
        fprintf(stderr, "%sErro: Não existem tabuleiros mais para a frente%s\n", ERROR_COLOR, RESET);
    else
    {
        freeTabela(game->tab);
        game->tab = copiarTabela(game->stackTabs->tabelas[game->stackTabs->indice++]);
    }

    *comandoProcessado = true;
}

void d(GAME *game, bool *comandoProcessado)
{
    if (!deleteTabela(game))
    {
        fprintf(stderr, "%sErro: Não existem tabuleiros anteriores%s\n", ERROR_COLOR, RESET);
    }

    *comandoProcessado = true;
}

// _____ Funçãoes utilizadas para verificar as restrições -> comando 'v'______

// Função auxiliar para verificar se uma célula está dentro dos limites do tabuleiro
bool dentroDosLimites(TABELA t, int linha, int coluna)
{
    return linha >= 0 && linha < t->l && coluna >= 0 && coluna < t->c;
}

// Função que verifica a vizinhança de uma casa riscada e caso tenha alguma casa riscada conta como restrição
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
            if (t->tabela[novaLinha][novaColuna] == '#')
            {
                // Se encontrar uma casa riscada na vizinhança, adiciona como restrição
                restricoes[count][0] = novaLinha;
                restricoes[count][1] = novaColuna;
                count++;
            }
        }
    }

    return count; // Retorna o número de restrições violadas
}

// Função que verifica se existem duas casas brancas iguais na mesma linha ou coluna
bool verificaLetraIgualLinhaColuna(TABELA t, int linha, int coluna)
{
    if (!dentroDosLimites(t, linha, coluna) || !isupper(t->tabela[linha][coluna]))
    {
        return false; // A célula não é uma casa branca
    }

    char letraMaiuscula = t->tabela[linha][coluna];

    // Verifica a linha
    for (int j = 0; j < t->c; j++)
    {
        if (j != coluna && t->tabela[linha][j] == letraMaiuscula)
        {
            return true; // Encontrou outra casa branca igual na mesma linha
        }
    }

    // Verifica a coluna
    for (int i = 0; i < t->l; i++)
    {
        if (i != linha && t->tabela[i][coluna] == letraMaiuscula)
        {
            return true; // Encontrou outra casa branca igual na mesma coluna
        }
    }

    return false; // Não encontrou casas brancas iguais na mesma linha ou coluna
}

// Função que trata das restrições que são vistas casa a casa (colocar em branco à volta de uma riscada e avisar letras que se repetem maiusculas)
void verificaRiscadasERepetidas(TABELA t, int i, int j, int *contaRestricoes, bool *temRestricoes, bool escreve)
{
    // Verifica restrições para casas riscadas
    if (t->tabela[i][j] == '#')
    {
        int restricoes[4][2]; // Máximo de 4 vizinhos
        int numRestricoes = verificaRiscadaVizinhasBrancas(t, i, j, restricoes);

        if (numRestricoes > 0)
        {
            *temRestricoes = true;
            if (escreve)
            {
                printf("\n%s%s---- Restrição nº %d ----%s\n", HINT_COLOR, BOLD, *contaRestricoes + 1, RESET);
                printf("Casa riscada em (%c%d) tem restrições violadas (apenas deveria ter casas brancas na sua vizinhança) nas seguintes coordenadas:\n", 'a' + j, i + 1);

                for (int k = 0; k < numRestricoes; k++)
                {
                    printf("  - Coluna: %c, Linha: %d\n", 'a' + restricoes[k][1], restricoes[k][0] + 1);
                }
            }
            (*contaRestricoes)++;
        }
    }

    // Verifica restrições para casas brancas
    if (isupper(t->tabela[i][j]))
    {
        if (verificaLetraIgualLinhaColuna(t, i, j))
        {
            *temRestricoes = true;
            if (escreve)
            {
                printf("\n%s%s---- Restrição nº %d ----%s\n", HINT_COLOR, BOLD, *contaRestricoes + 1, RESET);
                printf("Casa branca em (%c%d) tem restrições violadas (letra repetida na mesma linha ou coluna).\n", 'a' + j, i + 1);
            }
            (*contaRestricoes)++;
        }
    }
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
            // Trata das restrições que são vistas casa a casa (colocar em branco à volta de uma riscada e avisar letras que se repetem maiusculas)
            verificaRiscadasERepetidas(t, i, j, &contaRestricoes, &temRestricoes, escreve);
        }
    }

    // Função que chama as funções auxiliares necessarias para verificar se existem caminhos ortogonais e trata as respetivas mensagens no terminal
    if (!trataCaminhoOrtogonal(t, &contaRestricoes, &temRestricoes, escreve))
    {
        return false;
    }

    if (!temRestricoes)
    {
        if (escreve)
            printf("%sNenhuma restrição foi violada no tabuleiro.%s\n", GREEN, RESET);
    }

    return temRestricoes;
}

void v(GAME game, bool *continuar, bool *comandoProcessado)
{
    if (game.tab == NULL)
    {
        fprintf(stderr, "%sErro: tabuleiro não carregado.%s\n", ERROR_COLOR, RESET);
        *continuar = false;
    }
    else
    {
        printf("%sA verificar as restrições do tabuleiro...%s\n", HINT_COLOR, RESET);
        verificaRestricoes(game.tab, true);
        *comandoProcessado = true;
    }
}
