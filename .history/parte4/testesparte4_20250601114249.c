#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parte4.h"

// Helper para criar e liberar tabelas
static TABELA create_table(int l, int c, char fill)
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

static void free_table(TABELA t)
{
    if (!t)
        return;
    for (int i = 0; i < t->l; i++)
        free(t->tabela[i]);
    free(t->tabela);
    free(t);
}

void test_verificaSeQuebraCaminho()
{
    // Cenário 1: não isola (contaRestricoes == 0)
    TABELA aux1 = create_table(2, 2, ' ');
    aux1->tabela[0][0] = 'a';
    bool changed = false;
    verificaSeQuebraCaminho(aux1, 0, 0, &changed, false);
    CU_ASSERT_EQUAL(aux1->tabela[0][0], 'a');
    CU_ASSERT_FALSE(changed);
    free_table(aux1);

    // Cenário 2: isolando uma célula remanescente
    // Inicialmente, só duas células minúsculas desconectadas: (0,0) e (1,1)
    TABELA aux2 = create_table(2, 2, ' ');
    aux2->tabela[0][0] = 'a';
    aux2->tabela[1][1] = 'b';
    changed = false;
    // Ao chamar em (0,0), marca '#'; então 'b' ficará isolado -> contaRestricoes > 0
    verificaSeQuebraCaminho(aux2, 0, 0, &changed, true);
    CU_ASSERT_EQUAL(aux2->tabela[0][0], 'A'); // pintado de branco
    CU_ASSERT_TRUE(changed);
    free_table(aux2);
}

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

    free_table(t);
    free_table(aux);
}

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

    free_table(t);
    free_table(aux);
}

void test_trataABA_linhas()
{
    TABELA aux = create_table(3, 3, ' ');
    // Grupo ABA em linha 0: 'A','b','A'
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'b';
    aux->tabela[0][2] = 'A';

    trataABA_linhas(aux);
    CU_ASSERT_EQUAL(aux->tabela[0][1], 'B'); // pintado de branco

    free_table(aux);
}

void test_trataABA_colunas()
{
    TABELA aux = create_table(3, 3, ' ');
    // Grupo ABA em coluna 1: (0,1)='a', (1,1)='b', (2,1)='a'
    aux->tabela[0][1] = 'a';
    aux->tabela[1][1] = 'b';
    aux->tabela[2][1] = 'a';

    trataABA_colunas(aux);
    CU_ASSERT_EQUAL(aux->tabela[1][1], 'B'); // pintado de branco

    free_table(aux);
}

void test_trataAA_A_NasLinhas()
{
    TABELA aux = create_table(3, 3, ' ');
    // XX_X em linha 0: 'A','A','a'
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'A';
    aux->tabela[0][2] = 'a';

    trataAA_A_NasLinhas(aux);
    CU_ASSERT_EQUAL(aux->tabela[0][2], '#');

    free_table(aux);
}

void test_trataAA_A_NasColunas()
{
    TABELA aux = create_table(3, 3, ' ');
    // XX_X em coluna 0: (0,0)='A',(1,0)='A',(2,0)='a'
    aux->tabela[0][0] = 'A';
    aux->tabela[1][0] = 'A';
    aux->tabela[2][0] = 'a';

    trataAA_A_NasColunas(aux);
    CU_ASSERT_EQUAL(aux->tabela[2][0], '#');

    free_table(aux);
}

void test_riscaABA()
{
    TABELA aux = create_table(3, 3, ' ');
    // Grupo ABA em linha 0
    aux->tabela[0][0] = 'A';
    aux->tabela[0][1] = 'B';
    aux->tabela[0][2] = 'A';

    riscaABA(aux);
    CU_ASSERT_EQUAL(aux->tabela[0][1], 'B');

    free_table(aux);
}

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

    free_table(aux);
}

void test_aplicaA()
{
    // Constrói uma tabela em que exista repetição que o comando 'a' corrige
    TABELA aux = create_table(2, 2, ' ');
    aux->tabela[0][0] = 'a';
    aux->tabela[0][1] = 'a';
    int mudancas = aplicaA(&aux);
    CU_ASSERT(mudancas > 0);
    // Depois de aplicar, nenhuma minúscula deve persistir
    CU_ASSERT_FALSE(existemMinusculas(aux));

    free_table(aux);
}

void test_comandoA()
{
    TABELA aux = create_table(2, 2, ' ');
    aux->tabela[0][0] = 'a';
    aux->tabela[1][0] = 'a';
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

    free_table(aux);
}

void test_tentaRiscarColunas_resolve()
{
    // Tabela 1×2: 'a','A' → transformando 'a' em 'A' resolve o jogo imediatamente
    TABELA t = create_table(1, 2, ' ');
    t->tabela[0][0] = 'a';
    t->tabela[0][1] = 'A';
    bool continuar = true;

    tentaRiscarColunas(0, 0, 1, &t, &continuar);
    CU_ASSERT_FALSE(continuar);
    // Agora t deve ser toda maiúscula
    CU_ASSERT_EQUAL(t->tabela[0][0], 'A');
    CU_ASSERT_EQUAL(t->tabela[0][1], 'A');

    free_table(t);
}

void test_tentaRiscarColunas_restricao()
{
    // Tabela 1×2: 'a','a' → transformando um cria restrição (duas letras iguais sem fluxo)
    TABELA t = create_table(1, 2, ' ');
    t->tabela[0][0] = 'a';
    t->tabela[0][1] = 'a';
    bool continuar = true;

    tentaRiscarColunas(0, 0, 1, &t, &continuar);
    CU_ASSERT_FALSE(continuar);
    // Deve ter riscado uma e pintado a outra
    // Como o código risca (0,0) e pinta (0,1)
    CU_ASSERT_EQUAL(t->tabela[0][0], '#');
    CU_ASSERT_EQUAL(t->tabela[0][1], 'A');

    free_table(t);
}

void test_tentaColunas()
{
    // Tabela 1×2: 'a','a' → tentaColunas deve retornar true
    TABELA t = create_table(1, 2, ' ');
    t->tabela[0][0] = 'a';
    t->tabela[0][1] = 'a';

    bool result = tentaColunas(&t);
    CU_ASSERT_TRUE(result);

    free_table(t);
}

void test_tentaRiscarLinhas_resolve()
{
    // Tabela 2×1: ['a'; 'A'] → transformar (0,0) resolve
    TABELA t = create_table(2, 1, ' ');
    t->tabela[0][0] = 'a';
    t->tabela[1][0] = 'A';
    bool continuar = true;

    tentaRiscarLinhas(0, 0, 1, &t, &continuar);
    CU_ASSERT_FALSE(continuar);
    CU_ASSERT_EQUAL(t->tabela[0][0], 'A');
    CU_ASSERT_EQUAL(t->tabela[1][0], 'A');

    free_table(t);
}

void test_tentaRiscarLinhas_restricao()
{
    // Tabela 2×1: ['a'; 'a'] → criar restrição após aplicar
    TABELA t = create_table(2, 1, ' ');
    t->tabela[0][0] = 'a';
    t->tabela[1][0] = 'a';
    bool continuar = true;

    tentaRiscarLinhas(0, 0, 1, &t, &continuar);
    CU_ASSERT_FALSE(continuar);
    // Deve riscar (0,0) e pintar (1,0)
    CU_ASSERT_EQUAL(t->tabela[0][0], '#');
    CU_ASSERT_EQUAL(t->tabela[1][0], 'A');

    free_table(t);
}

void test_tentaLinhas()
{
    // Tabela 2×1: ['a'; 'a'] → tentaLinhas retorna true
    TABELA t = create_table(2, 1, ' ');
    t->tabela[0][0] = 'a';
    t->tabela[1][0] = 'a';

    bool result = tentaLinhas(&t);
    CU_ASSERT_TRUE(result);

    free_table(t);
}

void test_resolve_null()
{
    // Tabela contrária: duas minúsculas isoladas, sem solução
    TABELA t = create_table(2, 2, ' ');
    t->tabela[0][0] = 'a';
    t->tabela[1][1] = 'b';
    TABELA sol = resolve(t);
    CU_ASSERT_PTR_NULL(sol);
    free_table(t);
}

void test_resolve_success()
{
    // Tabela já resolvida: apenas maiúsculas
    TABELA t = create_table(1, 1, 'A');
    TABELA sol = resolve(t);
    CU_ASSERT_PTR_NOT_NULL(sol);
    CU_ASSERT_EQUAL(sol->tabela[0][0], 'A');
    free_table(sol);
    free_table(t);
}
