#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parte4.h"

void test_verificaSeQuebraCaminho()
{
    // Inicializa uma tabela de teste
    TABELA aux = malloc(sizeof(struct Tabela));
    aux->l = 3; // 3 linhas
    aux->c = 3; // 3 colunas
    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < aux->l; i++)
    {
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < aux->c; j++)
        {
            aux->tabela[i][j] = ' '; // Inicializa todas as células como brancas
        }
    }

    // Configura uma célula específica para teste
    aux->tabela[1][1] = 'A'; // Define uma célula com uma letra

    bool changed = false;

    // Cenário 1: A célula não quebra o caminho
    verificaSeQuebraCaminho(aux, 1, 1, &changed, false);
    CU_ASSERT_EQUAL(aux->tabela[1][1], 'A'); // A célula deve permanecer inalterada
    CU_ASSERT_FALSE(changed);                // O valor de changed deve ser falso

    // Cenário 2: A célula quebra o caminho
    aux->tabela[0][0] = '#'; // Adiciona outra célula para criar restrições
    verificaSeQuebraCaminho(aux, 1, 1, &changed, true);
    CU_ASSERT_EQUAL(aux->tabela[1][1], 'A'); // A célula deve ser pintada de branco
    CU_ASSERT_TRUE(changed);                 // O valor de changed deve ser verdadeiro

    // Limpeza de memória
    for (int i = 0; i < aux->l; i++)
    {
        free(aux->tabela[i]);
    }
    free(aux->tabela);
    free(aux);
}

void test_riscaLetrasRepetidas()
{
    // Inicializa a tabela original (t) e a tabela auxiliar (aux)
    TABELA t = malloc(sizeof(struct Tabela));
    TABELA aux = malloc(sizeof(struct Tabela));
    t->l = aux->l = 3; // 3 linhas
    t->c = aux->c = 3; // 3 colunas

    t->tabela = malloc(t->l * sizeof(char *));
    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < t->l; i++)
    {
        t->tabela[i] = malloc(t->c * sizeof(char));
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < t->c; j++)
        {
            t->tabela[i][j] = ' ';   // Inicializa todas as células como brancas
            aux->tabela[i][j] = ' '; // Inicializa a tabela auxiliar como branca
        }
    }

    // Configura a tabela original com letras repetidas
    t->tabela[0][0] = 'A';
    t->tabela[0][1] = 'a'; // Repetição na linha
    t->tabela[1][0] = 'A'; // Repetição na coluna

    bool changed = false;

    // Executa a função para a célula (0, 0)
    riscaLetrasRepetidas(t, aux, 0, 0, true, &changed);

    // Verifica se as células repetidas foram riscadas
    CU_ASSERT_EQUAL(aux->tabela[0][1], '#'); // Repetição na linha
    CU_ASSERT_EQUAL(aux->tabela[1][0], '#'); // Repetição na coluna
    CU_ASSERT_TRUE(changed);                 // O valor de changed deve ser verdadeiro

    // Verifica se as outras células permanecem inalteradas
    CU_ASSERT_EQUAL(aux->tabela[0][0], ' ');
    CU_ASSERT_EQUAL(aux->tabela[1][1], ' ');

    // Limpeza de memória
    for (int i = 0; i < t->l; i++)
    {
        free(t->tabela[i]);
        free(aux->tabela[i]);
    }
    free(t->tabela);
    free(aux->tabela);
    free(t);
    free(aux);
}

void test_pintaVizinhosDeRiscadas()
{
    // Inicializa a tabela original (t) e a tabela auxiliar (aux)
    TABELA t = malloc(sizeof(struct Tabela));
    TABELA aux = malloc(sizeof(struct Tabela));
    t->l = aux->l = 3; // 3 linhas
    t->c = aux->c = 3; // 3 colunas

    t->tabela = malloc(t->l * sizeof(char *));
    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < t->l; i++)
    {
        t->tabela[i] = malloc(t->c * sizeof(char));
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < t->c; j++)
        {
            t->tabela[i][j] = ' ';   // Inicializa todas as células como brancas
            aux->tabela[i][j] = ' '; // Inicializa a tabela auxiliar como branca
        }
    }

    // Configura a tabela original com uma célula riscada e vizinhos com letras minúsculas
    t->tabela[1][1] = '#'; // Célula riscada
    t->tabela[0][1] = 'a'; // Vizinho acima
    t->tabela[2][1] = 'b'; // Vizinho abaixo
    t->tabela[1][0] = 'c'; // Vizinho à esquerda
    t->tabela[1][2] = 'd'; // Vizinho à direita

    bool changed = false;

    // Executa a função para a célula riscada (1, 1)
    pintaVizinhosDeRiscadas(t, aux, 1, 1, true, &changed);

    // Verifica se os vizinhos foram pintados de branco (convertidos para maiúsculas)
    CU_ASSERT_EQUAL(aux->tabela[0][1], 'A'); // Vizinho acima
    CU_ASSERT_EQUAL(aux->tabela[2][1], 'B'); // Vizinho abaixo
    CU_ASSERT_EQUAL(aux->tabela[1][0], 'C'); // Vizinho à esquerda
    CU_ASSERT_EQUAL(aux->tabela[1][2], 'D'); // Vizinho à direita
    CU_ASSERT_TRUE(changed);                 // O valor de changed deve ser verdadeiro

    // Verifica se a célula riscada permanece inalterada
    CU_ASSERT_EQUAL(aux->tabela[1][1], ' ');

    // Limpeza de memória
    for (int i = 0; i < t->l; i++)
    {
        free(t->tabela[i]);
        free(aux->tabela[i]);
    }
    free(t->tabela);
    free(aux->tabela);
    free(t);
    free(aux);
}

void test_aplicaA()
{
    // Inicializa uma tabela de teste
    TABELA aux = malloc(sizeof(struct Tabela));
    aux->l = 3; // 3 linhas
    aux->c = 3; // 3 colunas

    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < aux->l; i++)
    {
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < aux->c; j++)
        {
            aux->tabela[i][j] = ' '; // Inicializa todas as células como brancas
        }
    }

    // Configura a tabela com um cenário inicial
    aux->tabela[0][0] = 'A'; // Exemplo de célula inicial
    aux->tabela[0][1] = 'a'; // Exemplo de célula que pode ser modificada
    aux->tabela[1][0] = '#'; // Exemplo de célula riscada

    // Executa a função aplicaA
    int mudancas = aplicaA(&aux);

    // Verifica se o número de mudanças está correto
    CU_ASSERT(mudancas > 0); // Deve haver pelo menos uma mudança

    // Verifica se a tabela foi modificada corretamente
    // (Adicione verificações específicas com base no comportamento esperado da função `ajuda`)

    // Limpeza de memória
    for (int i = 0; i < aux->l; i++)
    {
        free(aux->tabela[i]);
    }
    free(aux->tabela);
    free(aux);
}

void test_comandoA()
{
    // Inicializa uma tabela de teste
    TABELA aux = malloc(sizeof(struct Tabela));
    aux->l = 3; // 3 linhas
    aux->c = 3; // 3 colunas

    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < aux->l; i++)
    {
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < aux->c; j++)
        {
            aux->tabela[i][j] = ' '; // Inicializa todas as células como brancas
        }
    }

    // Configura a tabela com um cenário inicial
    aux->tabela[0][0] = 'A'; // Exemplo de célula inicial
    aux->tabela[0][1] = 'a'; // Exemplo de célula que pode ser modificada

    bool continuar = true;

    // Executa a função comandoA
    comandoA(&aux, &continuar);

    // Verifica se o tabuleiro foi alterado
    CU_ASSERT_TRUE(continuar); // Deve continuar se houve alterações

    // Configura a tabela para não sofrer alterações
    aplicaA(&aux);
    comandoA(&aux, &continuar);

    // Verifica se o tabuleiro não sofreu alterações
    CU_ASSERT_FALSE(continuar); // Não deve continuar se não houve alterações

    // Limpeza de memória
    for (int i = 0; i < aux->l; i++)
    {
        free(aux->tabela[i]);
    }
    free(aux->tabela);
    free(aux);
}

void test_trataAA_A_NasLinhas()
{
    // Inicializa uma tabela de teste
    TABELA aux = malloc(sizeof(struct Tabela));
    aux->l = 3; // 3 linhas
    aux->c = 3; // 3 colunas

    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < aux->l; i++)
    {
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < aux->c; j++)
        {
            aux->tabela[i][j] = ' '; // Inicializa todas as células como brancas
        }
    }

    // Configura a tabela com grupos do tipo XX_X
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'A';
    aux->tabela[0][2] = 'a'; // Deve ser riscado

    // Executa a função trataAA_A_NasLinhas
    trataAA_A_NasLinhas(aux);

    // Verifica se a célula foi riscada
    CU_ASSERT_EQUAL(aux->tabela[0][2], '#');

    // Limpeza de memória
    for (int i = 0; i < aux->l; i++)
    {
        free(aux->tabela[i]);
    }
    free(aux->tabela);
    free(aux);
}

void test_trataAA_A_NasColunas()
{
    // Inicializa uma tabela de teste
    TABELA aux = malloc(sizeof(struct Tabela));
    aux->l = 3; // 3 linhas
    aux->c = 3; // 3 colunas

    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < aux->l; i++)
    {
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < aux->c; j++)
        {
            aux->tabela[i][j] = ' '; // Inicializa todas as células como brancas
        }
    }

    // Configura a tabela com grupos do tipo XX_X
    aux->tabela[0][0] = 'A';
    aux->tabela[1][0] = 'A';
    aux->tabela[2][0] = 'a'; // Deve ser riscado

    // Executa a função trataAA_A_NasColunas
    trataAA_A_NasColunas(aux);

    // Verifica se a célula foi riscada
    CU_ASSERT_EQUAL(aux->tabela[2][0], '#');

    // Limpeza de memória
    for (int i = 0; i < aux->l; i++)
    {
        free(aux->tabela[i]);
    }
    free(aux->tabela);
    free(aux);
}

void test_riscaABA()
{
    // Inicializa uma tabela de teste
    TABELA aux = malloc(sizeof(struct Tabela));
    aux->l = 3; // 3 linhas
    aux->c = 3; // 3 colunas

    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < aux->l; i++)
    {
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < aux->c; j++)
        {
            aux->tabela[i][j] = ' '; // Inicializa todas as células como brancas
        }
    }

    // Configura a tabela com grupos do tipo ABA
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'B'; // Deve ser pintado de branco
    aux->tabela[0][2] = 'A';

    // Executa a função riscaABA
    riscaABA(aux);

    // Verifica se a célula foi pintada de branco
    CU_ASSERT_EQUAL(aux->tabela[0][1], 'B');

    // Limpeza de memória
    for (int i = 0; i < aux->l; i++)
    {
        free(aux->tabela[i]);
    }
    free(aux->tabela);
    free(aux);
}

void test_jogoResolvido()
{
    // Inicializa uma tabela de teste
    TABELA aux = malloc(sizeof(struct Tabela));
    aux->l = 3; // 3 linhas
    aux->c = 3; // 3 colunas

    aux->tabela = malloc(aux->l * sizeof(char *));
    for (int i = 0; i < aux->l; i++)
    {
        aux->tabela[i] = malloc(aux->c * sizeof(char));
        for (int j = 0; j < aux->c; j++)
        {
            aux->tabela[i][j] = ' '; // Inicializa todas as células como brancas
        }
    }

    // Configura a tabela como resolvida
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'B';
    aux->tabela[0][2] = 'C';

    // Verifica se o jogo está resolvido
    CU_ASSERT_TRUE(jogoResolvido(aux));

    // Configura a tabela com minúsculas
    aux->tabela[1][0] = 'a';

    // Verifica se o jogo não está resolvido
    CU_ASSERT_FALSE(jogoResolvido(aux));

    // Limpeza de memória
    for (int i = 0; i < aux->l; i++)
    {
        free(aux->tabela[i]);
    }
    free(aux->tabela);
    free(aux);
}