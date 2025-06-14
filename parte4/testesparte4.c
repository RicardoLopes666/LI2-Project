#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parte4.h"

// Função auxiliar para criar
TABELA create_table(int l, int c, char fill)
{
    TABELA t = malloc(sizeof(struct Tabela));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t);
    t->l = l;
    t->c = c;
    t->tabela = malloc(l * sizeof(char *));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t->tabela);
    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc(c * sizeof(char));
        CU_ASSERT_PTR_NOT_NULL_FATAL(t->tabela[i]);
        for (int j = 0; j < c; j++)
            t->tabela[i][j] = fill;
    }
    return t;
}

void test_verificaSeQuebraCaminho()
{
    // não isola (contaRestricoes == 0)
    TABELA aux1 = create_table(2, 2, ' ');
    aux1->tabela[0][0] = 'a';
    bool changed = false;
    verificaSeQuebraCaminho(aux1, 0, 0, &changed, false);
    CU_ASSERT_EQUAL(aux1->tabela[0][0], 'a');
    CU_ASSERT_FALSE(changed);
    freeTabela(aux1);

    // isolar uma célula
    TABELA aux2 = create_table(2, 2, ' ');
    aux2->tabela[0][0] = 'a';
    aux2->tabela[1][1] = '#';
    changed = false;
    // Ao chamar em (0,0), marca '#'; então 'b' ficará isolado -> contaRestricoes > 0
    verificaSeQuebraCaminho(aux2, 0, 0, &changed, true);
    CU_ASSERT_EQUAL(aux2->tabela[0][0], 'A'); // pintado de branco
    CU_ASSERT_TRUE(changed);
    freeTabela(aux2);
}

// Função responsável por testar a função riscaLetrasRepetidas
void test_riscaLetrasRepetidas()
{
    TABELA t = create_table(3, 3, ' ');
    TABELA aux = create_table(3, 3, ' ');
    // Repetições: letra minúscula 'a' em (0,1) e (1,0)
    t->tabela[0][0] = 'A';
    t->tabela[0][1] = 'a';
    t->tabela[1][0] = 'a';
    bool changed = false;

    riscaLetrasRepetidas(t, aux, 0, 0, true, &changed);
    // Deve riscar (0,1) e (1,0)
    CU_ASSERT_EQUAL(aux->tabela[0][1], '#');
    CU_ASSERT_EQUAL(aux->tabela[1][0], '#');
    CU_ASSERT_TRUE(changed);
    // Verifica que outras posições permanecem inalteradas
    CU_ASSERT_EQUAL(aux->tabela[0][0], ' ');
    CU_ASSERT_EQUAL(aux->tabela[2][2], ' ');

    freeTabela(t);
    freeTabela(aux);
}

// Função responsável por testar a função pintaVizinhosDeRiscadas
void test_pintaVizinhosDeRiscadas()
{
    TABELA t = create_table(3, 3, ' ');
    TABELA aux = create_table(3, 3, ' ');
    // Célula riscada em (1,1), vizinhos minúsculos
    t->tabela[1][1] = '#';
    t->tabela[0][1] = 'a';
    t->tabela[2][1] = 'b';
    t->tabela[1][0] = 'c';
    t->tabela[1][2] = 'd';
    bool changed = false;

    pintaVizinhosDeRiscadas(t, aux, 1, 1, true, &changed);
    CU_ASSERT_EQUAL(aux->tabela[0][1], 'A');
    CU_ASSERT_EQUAL(aux->tabela[2][1], 'B');
    CU_ASSERT_EQUAL(aux->tabela[1][0], 'C');
    CU_ASSERT_EQUAL(aux->tabela[1][2], 'D');
    CU_ASSERT_TRUE(changed);
    // O local riscado (1,1) deve permanecer ' '
    CU_ASSERT_EQUAL(aux->tabela[1][1], ' ');

    freeTabela(t);
    freeTabela(aux);
}

// Função responsável por testar a função trataABA_linhas
void test_trataABA_linhas()
{
    TABELA aux = create_table(3, 3, ' ');
    // Grupo ABA em linha 0: 'A','b','A'
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'b';
    aux->tabela[0][2] = 'A';

    trataABA_linhas(aux);
    CU_ASSERT_EQUAL(aux->tabela[0][1], 'B'); // pintado de branco

    freeTabela(aux);
}

// Função responsável por testar trataABA_colunas
void test_trataABA_colunas()
{
    TABELA aux = create_table(3, 3, ' ');
    // Grupo ABA na coluna 1
    aux->tabela[0][1] = 'a';
    aux->tabela[1][1] = 'b';
    aux->tabela[2][1] = 'a';

    trataABA_colunas(aux);
    CU_ASSERT_EQUAL(aux->tabela[1][1], 'B'); // pintado de branco

    freeTabela(aux);
}

// Função responsável por testar afunção trataAA_A_NasLinhas
void test_trataAA_A_NasLinhas()
{
    TABELA aux = create_table(3, 3, ' ');
    // XX_X em linha 0: 'A','A','a'
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'A';
    aux->tabela[0][2] = 'a';

    trataAA_A_NasLinhas(aux);
    CU_ASSERT_EQUAL(aux->tabela[0][2], '#');

    freeTabela(aux);
}

// Função responsável por testar a função trataAA_A_NasColunas
void test_trataAA_A_NasColunas()
{
    TABELA aux = create_table(3, 3, ' ');
    // XX_X em coluna 0: (0,0)='A',(1,0)='A',(2,0)='a'
    aux->tabela[0][0] = 'A';
    aux->tabela[1][0] = 'A';
    aux->tabela[2][0] = 'a';

    trataAA_A_NasColunas(aux);
    CU_ASSERT_EQUAL(aux->tabela[2][0], '#');

    freeTabela(aux);
}

// Função responsável por testar a função riscaABA
void test_riscaABA()
{
    TABELA aux = create_table(3, 3, ' ');
    // Grupo ABA em linha 0
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'B';
    aux->tabela[0][2] = 'A';

    riscaABA(aux);
    CU_ASSERT_EQUAL(aux->tabela[0][1], 'B');

    freeTabela(aux);
}

// Função responsável por testar as funções existemMinusculas e jogoResolvido
void test_existemMinusculas_and_jogoResolvido()
{
    TABELA aux = create_table(2, 2, ' ');
    // Sem minúsculas: jogoResolvido == true
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'B';
    CU_ASSERT_FALSE(existemMinusculas(aux));
    CU_ASSERT_TRUE(jogoResolvido(aux));

    // Insere minúscula: game not resolved
    aux->tabela[1][0] = 'c';
    CU_ASSERT_TRUE(existemMinusculas(aux));
    CU_ASSERT_FALSE(jogoResolvido(aux));

    freeTabela(aux);
}

// Função responsável por testar a função aplicaA
void test_aplicaA()
{
    // Constrói uma tabela em que exista repetição que o comando 'a' corrige
    TABELA aux = create_table(2, 2, ' ');
    aux->tabela[0][0] = 'a';
    aux->tabela[0][1] = 'a';
    int mudancas = aplicaA(&aux);
    CU_ASSERT(mudancas == 0);
    // Depois de aplicar, nada deve ser alterado
    CU_ASSERT_TRUE(existemMinusculas(aux));

    freeTabela(aux);
}

// Função responsável por testar a função A
void test_comandoA()
{
    TABELA aux = create_table(2, 2, ' ');
    aux->tabela[0][0] = 'a';
    aux->tabela[1][0] = 'A';
    aux->tabela[1][1] = 'a';
    aux->tabela[0][1] = 'a';
    bool continuar = true;

    // Deve alterar e deixar continuar = true
    comandoA(&aux, &continuar);
    CU_ASSERT_TRUE(continuar);

    // Após aplicarA, não há mais mudanças
    aplicaA(&aux);
    comandoA(&aux, &continuar);
    CU_ASSERT_FALSE(continuar);

    freeTabela(aux);
}

// Função responsável por testar a função resolve no caso de dar null
void test_resolve_null()
{
    // Tabela contrária: duas minúsculas isoladas, sem solução
    TABELA t = create_table(2, 2, ' ');
    t->tabela[0][0] = 'a';
    t->tabela[1][1] = 'a';
    t->tabela[1][0] = 'a';
    t->tabela[0][1] = 'a';
    TABELA sol = resolve(t);
    CU_ASSERT_PTR_NULL(sol);
    freeTabela(t);
}

// Função responsável por testar a função resolve
void test_resolve_success()
{
    // Tabela já resolvida: apenas maiúsculas
    TABELA t = create_table(1, 1, 'A');
    TABELA sol = resolve(t);
    CU_ASSERT_PTR_NOT_NULL(sol);
    CU_ASSERT_EQUAL(sol->tabela[0][0], 'A');
    freeTabela(sol);
    freeTabela(t);
}
