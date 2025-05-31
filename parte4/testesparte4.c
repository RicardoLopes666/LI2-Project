#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "../tipos.h"

// ────── STUBS e HELPERS para as dependências externas de parte4.c ──────

// Nota: NÃO redefinimos `struct Tabela` aqui, pois já existe em tipos.h.

// Função auxiliar para criar uma TABELA de dimensões l x c, preenchida com `fill`
static TABELA criarTabela(int l, int c, char fill)
{
    TABELA t = malloc(sizeof(*t));
    t->l = l;
    t->c = c;
    t->tabela = malloc(l * sizeof(char *));
    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc(c * sizeof(char));
        for (int j = 0; j < c; j++)
        {
            t->tabela[i][j] = fill;
        }
    }
    return t;
}

// Liberta a memória de uma TABELA criada por `criarTabela`
static void freeTabela(TABELA t)
{
    if (!t)
        return;
    for (int i = 0; i < t->l; i++)
    {
        free(t->tabela[i]);
    }
    free(t->tabela);
    free(t);
}

// ─────────── STUB: copiarTabela ───────────
// Copia “profundamente” todas as células de `t`
TABELA copiarTabela(TABELA t)
{
    if (!t)
        return NULL;
    TABELA copia = malloc(sizeof(*copia));
    copia->l = t->l;
    copia->c = t->c;
    copia->tabela = malloc(t->l * sizeof(char *));
    for (int i = 0; i < t->l; i++)
    {
        copia->tabela[i] = malloc(t->c * sizeof(char));
        memcpy(copia->tabela[i], t->tabela[i], t->c * sizeof(char));
    }
    return copia;
}

// ─────────── STUB: dentroDosLimites ───────────
// Deve ter assinatura idêntica à declarada em parte2.h:
//     bool dentroDosLimites(TABELA t, int linha, int coluna);
bool dentroDosLimites(TABELA t, int l, int c)
{
    return (l >= 0 && l < t->l && c >= 0 && c < t->c);
}

// ─────────── STUB: trataCaminhoOrtogonal ───────────
// Assinatura idêntica ao declarado em parte3.h:
//     bool trataCaminhoOrtogonal(TABELA t, int *contaRestricoes, bool *temRestricoes, bool escreve);
static int stub_contaRestricoes = 0;
bool trataCaminhoOrtogonal(TABELA t, int *r, bool *changed, bool b)
{
    *r = stub_contaRestricoes;
    if (stub_contaRestricoes > 0)
    {
        *changed = true;
    }
    // Retornamos “true” sempre para não interromper loops,
    // mas, para os nossos testes, only r e changed importam.
    return true;
}

// ─────────── STUB: verificaRestricoes ───────────
// Assinatura idêntica à declarada em parte2.h (ou em parte4.h):
//     bool verificaRestricoes(TABELA t, bool escreve);
bool verificaRestricoes(TABELA t, bool b)
{
    return false;
}

// ─────────── STUB: colors (para `comandoA`) ───────────
#ifdef GREEN
#undef GREEN
#endif
#ifdef RESET
#undef RESET
#endif
#define GREEN ""
#define RESET ""

// ─────────── Inclui diretamente o código de parte4.c ───────────
// Assim, todas as chamadas internas a `copiarTabela`, `dentroDosLimites`,
// `trataCaminhoOrtogonal` e `verificaRestricoes` usarão estas STUBS acima.
#include "parte4.c"

// ─────────── Captura / Restaura stdout ───────────
static FILE *orig_stdout;
static void _begin_capture(void)
{
    orig_stdout = stdout;
    stdout = fopen("/dev/null", "w");
}
static void _end_capture(void)
{
    fclose(stdout);
    stdout = orig_stdout;
}

/*==================================================================
  test_verificaSeQuebraCaminho
  Cobre:
    - quando stub_contaRestricoes == 0  (não altera célula)
    - quando stub_contaRestricoes > 0   (pinta maiúscula e changed = true)
===================================================================*/
void test_verificaSeQuebraCaminho(void)
{
    // ─── Caso 1: stub_contaRestricoes == 0
    {
        TABELA t = criarTabela(2, 2, 'a');
        bool changed = false;
        stub_contaRestricoes = 0;
        // Deixa a célula [1][1] como 'b'
        t->tabela[1][1] = 'b';
        verificaSeQuebraCaminho(t, 1, 1, &changed, false);
        CU_ASSERT_EQUAL(t->tabela[1][1], 'b');
        CU_ASSERT_FALSE(changed);
        freeTabela(t);
    }
    // ─── Caso 2: stub_contaRestricoes > 0 e escreve == true
    {
        TABELA t = criarTabela(2, 2, 'c');
        bool changed = false;
        stub_contaRestricoes = 1;
        t->tabela[0][0] = 'd'; // minúscula
        _begin_capture();
        verificaSeQuebraCaminho(t, 0, 0, &changed, true);
        _end_capture();
        CU_ASSERT_EQUAL(t->tabela[0][0], 'D'); // tornou-se maiúscula
        CU_ASSERT_TRUE(changed);
        freeTabela(t);
    }
}

/*==================================================================
  test_riscaLetrasRepetidas
  Cobre:
    - repetição na linha  → marca '#'
    - repetição na coluna → marca '#'
    - ausência de repetição → changed permanece false
===================================================================*/
void test_riscaLetrasRepetidas(void)
{
    // ─── Caso 1: repetição na linha
    {
        TABELA t = criarTabela(2, 3, 'a');
        TABELA aux = copiarTabela(t);
        bool changed = false;
        // [0][0] = 'A', [0][2] = 'a' → repetição de 'a' na linha 0
        t->tabela[0][0] = 'A';
        t->tabela[0][2] = 'a';
        riscaLetrasRepetidas(t, aux, 0, 0, true, &changed);
        CU_ASSERT_EQUAL(aux->tabela[0][2], '#');
        CU_ASSERT_TRUE(changed);
        freeTabela(t);
        freeTabela(aux);
    }
    // ─── Caso 2: repetição na coluna
    {
        TABELA t = criarTabela(3, 2, 'b');
        TABELA aux = copiarTabela(t);
        bool changed = false;
        // [0][1] = 'B', [2][1] = 'b' → repetição de 'b' na coluna 1
        t->tabela[0][1] = 'B';
        t->tabela[2][1] = 'b';
        riscaLetrasRepetidas(t, aux, 0, 1, true, &changed);
        CU_ASSERT_EQUAL(aux->tabela[2][1], '#');
        CU_ASSERT_TRUE(changed);
        freeTabela(t);
        freeTabela(aux);
    }
    // ─── Caso 3: sem repetição → changed == false
    {
        TABELA t = criarTabela(2, 2, 'x');
        TABELA aux = copiarTabela(t);
        bool changed = false;
        // [1][1] = 'X', mas não existe 'x' em outro lugar
        t->tabela[1][1] = 'X';
        riscaLetrasRepetidas(t, aux, 1, 1, false, &changed);
        CU_ASSERT_FALSE(changed);
        CU_ASSERT_EQUAL(aux->tabela[0][0], 'x');
        freeTabela(t);
        freeTabela(aux);
    }
}

/*==================================================================
  test_pintaVizinhosDeRiscadas
  Cobre:
    - vizinhos válidos minúsculos → tornam‐se maiúsculos
    - sem vizinhos válidos → changed == false
===================================================================*/
void test_pintaVizinhosDeRiscadas(void)
{
    // ─── Caso 1: existem vizinhos minúsculos válidos
    {
        TABELA t = criarTabela(3, 3, 'c');
        TABELA aux = copiarTabela(t);
        bool changed = false;
        // [1][1] = '#'; vizinhos: [0][1]='d', [2][1]='e', [1][0]='f', [1][2]='g'
        t->tabela[1][1] = '#';
        t->tabela[0][1] = 'd';
        t->tabela[2][1] = 'e';
        t->tabela[1][0] = 'f';
        t->tabela[1][2] = 'g';
        pintaVizinhosDeRiscadas(t, aux, 1, 1, true, &changed);
        CU_ASSERT_EQUAL(aux->tabela[0][1], 'D');
        CU_ASSERT_EQUAL(aux->tabela[2][1], 'E');
        CU_ASSERT_EQUAL(aux->tabela[1][0], 'F');
        CU_ASSERT_EQUAL(aux->tabela[1][2], 'G');
        CU_ASSERT_TRUE(changed);
        freeTabela(t);
        freeTabela(aux);
    }
    // ─── Caso 2: sem vizinhos válidos
    {
        TABELA t = criarTabela(2, 2, 'h');
        TABELA aux = copiarTabela(t);
        bool changed = false;
        // [0][0] = '#'; mas não há vizinhos dentroDosLimites que sejam minúsculos
        t->tabela[0][0] = '#';
        pintaVizinhosDeRiscadas(t, aux, 0, 0, false, &changed);
        CU_ASSERT_FALSE(changed);
        CU_ASSERT_EQUAL(aux->tabela[1][1], 'h');
        freeTabela(t);
        freeTabela(aux);
    }
}

/*==================================================================
  test_aplicaA
  Cobre:
    - sem mudanças (retorna 0)
    - com mudanças via riscaLetrasRepetidas (retorna > 0)
    - com mudanças via verificaSeQuebraCaminho (forçando stub_contaRestricoes)
===================================================================*/
void test_aplicaA(void)
{
    // ─── Caso 1: sem mudanças → retorna 0
    {
        TABELA t = criarTabela(2, 2, 'a');
        int cnt = aplicaA(&t);
        CU_ASSERT_EQUAL(cnt, 0);
        freeTabela(t);
    }
    // ─── Caso 2: com mudança via riscaLetrasRepetidas
    {
        TABELA t = criarTabela(2, 2, 'a');
        t->tabela[0][0] = 'A'; // forçar passagem por “isupper”
        t->tabela[0][1] = 'a';
        int cnt = aplicaA(&t);
        CU_ASSERT_TRUE(cnt > 0);
        freeTabela(t);
    }
    // ─── Caso 3: com mudança via verificaSeQuebraCaminho (stub_contaRestricoes=1)
    {
        TABELA t = criarTabela(2, 2, 'x');
        t->tabela[0][0] = 'Y'; // forçar chamada a riscaLetrasRepetidas (mas sem repetição real)
        t->tabela[1][1] = 'y'; // isso forçará chamada a verificaSeQuebraCaminho
        stub_contaRestricoes = 1;
        TABELA aux = t; // `aplicaA` vai invocar “ajuda” em `aux`
        int cnt = aplicaA(&aux);
        CU_ASSERT_TRUE(cnt > 0);
        freeTabela(aux);
        stub_contaRestricoes = 0;
    }
}

/*==================================================================
  test_comandoA
  Cobre:
    - sem mudança   → imprime mensagem e *continuar = false
    - com mudança   → imprime “Tabuleiro alterado.” e deixar *continuar = true
===================================================================*/
void test_comandoA(void)
{
    // ─── Caso 1: sem mudanças
    {
        TABELA t = criarTabela(1, 1, 'z');
        bool cont = true;
        _begin_capture();
        comandoA(&t, &cont);
        _end_capture();
        CU_ASSERT_FALSE(cont);
        freeTabela(t);
    }
    // ─── Caso 2: com mudanças
    {
        TABELA t = criarTabela(2, 2, 'a');
        t->tabela[0][0] = 'A';
        t->tabela[0][1] = 'a';
        bool cont = true;
        _begin_capture();
        comandoA(&t, &cont);
        _end_capture();
        CU_ASSERT_TRUE(cont);
        freeTabela(t);
    }
}

/*==================================================================
  test_trataAA_A_NasLinhas
  Cobre o loop que varre todas as linhas e risca o isolado em padrão “XX_X”
===================================================================*/
void test_trataAA_A_NasLinhas(void)
{
    TABELA t = criarTabela(1, 4, 'a');
    // Linha: 'a','a','b','a'  → as duas primeiras formam par, última (índice 3) fica isolada
    t->tabela[0][0] = 'a';
    t->tabela[0][1] = 'a';
    t->tabela[0][2] = 'b';
    t->tabela[0][3] = 'a';
    trataAA_A_NasLinhas(t);
    CU_ASSERT_EQUAL(t->tabela[0][3], '#');
    freeTabela(t);
}

/*==================================================================
  test_trataAA_A_NasColunas
  Cobre o loop que varre todas as colunas e risca o isolado em padrão “XX_X” vertical
===================================================================*/
void test_trataAA_A_NasColunas(void)
{
    TABELA t = criarTabela(4, 1, 'c');
    // Coluna: 'c','c','d','c' → última (índice 3) fica isolada
    t->tabela[0][0] = 'c';
    t->tabela[1][0] = 'c';
    t->tabela[2][0] = 'd';
    t->tabela[3][0] = 'c';
    trataAA_A_NasColunas(t);
    CU_ASSERT_EQUAL(t->tabela[3][0], '#');
    freeTabela(t);
}

/*==================================================================
  test_riscaABA
  Cobre:
    - padrão “ABA” em linha (trataABA_linhas)
    - padrão “ABA” em coluna (trataABA_colunas)
    - a chamada geral riscaABA invoca ambos sem erro
===================================================================*/
void test_riscaABA(void)
{
    // ─── Caso 1: padrão ABA em linha
    {
        TABELA t = criarTabela(1, 3, 'x');
        // linha: 'x','y','x'
        t->tabela[0][0] = 'x';
        t->tabela[0][1] = 'y';
        t->tabela[0][2] = 'x';
        trataABA_linhas(t);
        CU_ASSERT_EQUAL(t->tabela[0][1], 'Y');
        freeTabela(t);
    }
    // ─── Caso 2: padrão ABA em coluna
    {
        TABELA t = criarTabela(3, 1, 'z');
        // coluna: 'z','y','z'
        t->tabela[0][0] = 'z';
        t->tabela[1][0] = 'y';
        t->tabela[2][0] = 'z';
        trataABA_colunas(t);
        CU_ASSERT_EQUAL(t->tabela[1][0], 'Y');
        freeTabela(t);
    }
    // ─── Caso 3: riscaABA invoca ambas sem erro
    {
        TABELA t = criarTabela(2, 4, 'm');
        // Forçar ao menos um ABA em linha:
        t->tabela[0][0] = 'm';
        t->tabela[0][1] = 'n';
        t->tabela[0][2] = 'm';
        // Forçar ao menos um ABA em coluna (criamos “z’ em (0,3),(1,3),(2,3)” só que linha 2 não existe,
        // então minimal: definimos (0,3)=’o’, (1,3)=’o’, (1,2)=’p’, para não gerar '#')
        t->tabela[0][3] = 'o';
        t->tabela[1][3] = 'o';
        t->tabela[1][2] = 'p';
        riscaABA(t);
        bool meioLinha = isupper(t->tabela[0][1]);
        bool meioColuna = false;
        if (t->tabela[1][3] == 'O' || t->tabela[0][3] == 'O')
        {
            meioColuna = true;
        }
        CU_ASSERT_TRUE(meioLinha || meioColuna);
        freeTabela(t);
    }
}
