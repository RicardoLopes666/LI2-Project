
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "parte2.h"
#include "testesparte2.h"
#include "../parte1/parte1.h"
#include "../parte3/parte3.h"
#include "../tipos.h"
#include "../colors.h"

// Cria uma TABELA de tamanho l×c, preenchida com fill_char.
// Aloca a estrutura e as linhas, terminando cada linha com '\0'.
TABELA criaTabelaExemplo(int l, int c, char fill_char)
{
    TABELA t = malloc(sizeof *t);
    CU_ASSERT_PTR_NOT_NULL_FATAL(t);
    t->l = l;
    t->c = c;
    t->tabela = malloc(sizeof(char *) * l);
    CU_ASSERT_PTR_NOT_NULL_FATAL(t->tabela);

    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc((c + 1) * sizeof(char)); /* +1 para '\0' */
        CU_ASSERT_PTR_NOT_NULL_FATAL(t->tabela[i]);
        for (int j = 0; j < c; j++)
        {
            t->tabela[i][j] = fill_char;
        }
        t->tabela[i][c] = '\0';
    }
    return t;
}

// Cria uma STACKTABS “vazia”, apenas alocando a struct. Não inicializa campos.
STACKTABS criaStackTabsVazio(void)
{
    STACKTABS s = malloc(sizeof *s);
    CU_ASSERT_PTR_NOT_NULL_FATAL(s);
    return s;
}

// Cria um GAME “vazio”: aloca a struct e define tab=NULL, stackTabs=NULL.
GAME *criaGameVazio(void)
{
    GAME *g = malloc(sizeof *g);
    CU_ASSERT_PTR_NOT_NULL_FATAL(g);
    g->tab = NULL;
    g->stackTabs = NULL;
    return g;
}

// Função responsável por testar a função initStackTabs
void test_initStackTabs(void)
{
    STACKTABS s = criaStackTabsVazio();
    bool r = initStackTabs(s);
    CU_ASSERT_TRUE(r);

    CU_ASSERT_EQUAL(s->capacidade, 3);
    CU_ASSERT_EQUAL(s->indice, 0);
    CU_ASSERT_EQUAL(s->comprimento, 0);
    CU_ASSERT_PTR_NOT_NULL(s->tabelas);

    free(s->tabelas);
    free(s);
}

// Função responsável por testar a função insereTabela
void test_insereTabela(void)
{

    // Caso “normal”: índice=0, comprimento=0
    STACKTABS s = criaStackTabsVazio();
    bool r0 = initStackTabs(s);
    CU_ASSERT_TRUE(r0);

    CU_ASSERT_EQUAL(s->comprimento, 0);
    CU_ASSERT_EQUAL(s->indice, 0);

    TABELA t1 = criaTabelaExemplo(2, 2, 'A');
    bool r1 = insereTabela(s, t1);
    CU_ASSERT_TRUE(r1);
    CU_ASSERT_EQUAL(s->comprimento, 1);
    CU_ASSERT_EQUAL(s->indice, 1);
    CU_ASSERT_PTR_EQUAL(s->tabelas[0], t1);

    // Caso s->indice < s->comprimento”:
    s->indice = 0;
    s->comprimento = 1;
    TABELA t2 = criaTabelaExemplo(2, 2, 'B');
    bool r2 = insereTabela(s, t2);
    CU_ASSERT_TRUE(r2);
    /* Verificar se t1 foi libertado e a nova t2 ficou em s->tabelas[0] */
    CU_ASSERT_EQUAL(s->comprimento, 1);
    CU_ASSERT_EQUAL(s->indice, 1);
    CU_ASSERT_PTR_EQUAL(s->tabelas[0], t2);

    // Ramo “s->comprimento >= s->capacidade”:
    STACKTABS s2 = criaStackTabsVazio();
    bool r3 = initStackTabs(s2);
    CU_ASSERT_TRUE(r3);
    TABELA a = criaTabelaExemplo(1, 1, 'X');
    TABELA b = criaTabelaExemplo(1, 1, 'Y');
    TABELA c = criaTabelaExemplo(1, 1, 'Z');
    CU_ASSERT_TRUE(insereTabela(s2, a));
    CU_ASSERT_TRUE(insereTabela(s2, b));
    CU_ASSERT_TRUE(insereTabela(s2, c));
    // Agora: capacidade==3, comprimento==3, índice==3
    CU_ASSERT_EQUAL(s2->capacidade, 3);
    CU_ASSERT_EQUAL(s2->comprimento, 3);
    CU_ASSERT_EQUAL(s2->indice, 3);

    TABELA d = criaTabelaExemplo(1, 1, 'W');
    int old_cap = s2->capacidade; // 3
    bool r4 = insereTabela(s2, d);
    CU_ASSERT_TRUE(r4);
    CU_ASSERT_EQUAL(s2->capacidade, old_cap * 2); // 6
    CU_ASSERT_EQUAL(s2->comprimento, 4);
    CU_ASSERT_EQUAL(s2->indice, 4);
    CU_ASSERT_PTR_EQUAL(s2->tabelas[3], d);

    // Libertar o que foi alocado
    for (int i = 0; i < s2->comprimento; i++)
    {
        freeTabela(s2->tabelas[i]);
    }
    free(s2->tabelas);
    free(s2);

    freeTabela(t2);
    free(s->tabelas);
    free(s);
}

// Função responsável por testar a função deleteTabela
void test_deleteTabela(void)
{
    // comprimento <= 1
    GAME *g1 = criaGameVazio();
    g1->stackTabs = criaStackTabsVazio();
    initStackTabs(g1->stackTabs);
    bool r1 = deleteTabela(g1);
    CU_ASSERT_FALSE(r1);
    free(g1->stackTabs->tabelas);
    free(g1->stackTabs);
    free(g1);

    // índice < 2 mas comprimento > 1
    GAME *g2 = criaGameVazio();
    g2->stackTabs = criaStackTabsVazio();
    initStackTabs(g2->stackTabs);
    TABELA tA = criaTabelaExemplo(1, 1, 'P');
    TABELA tB = criaTabelaExemplo(1, 1, 'Q');
    insereTabela(g2->stackTabs, tA);
    insereTabela(g2->stackTabs, tB);
    // comprimento=2, índice=2.
    g2->stackTabs->indice = 1;
    bool r2 = deleteTabela(g2);
    CU_ASSERT_FALSE(r2);
    // Libertar memória
    freeTabela(tA);
    freeTabela(tB);
    free(g2->stackTabs->tabelas);
    free(g2->stackTabs);
    free(g2);

    // comprimento > 1 e índice >= 2
    GAME *g3 = criaGameVazio();
    g3->stackTabs = criaStackTabsVazio();
    initStackTabs(g3->stackTabs);
    TABELA t1 = criaTabelaExemplo(2, 2, 'R');
    TABELA t2 = criaTabelaExemplo(2, 2, 'S');
    insereTabela(g3->stackTabs, t1);
    insereTabela(g3->stackTabs, t2);
    g3->tab = t2; /* tab corrente aponta para t2 */
    bool r3 = deleteTabela(g3);
    CU_ASSERT_TRUE(r3);

    CU_ASSERT_EQUAL(g3->tab->l, t1->l);
    CU_ASSERT_EQUAL(g3->tab->c, t1->c);
    CU_ASSERT_PTR_NOT_EQUAL(g3->tab, t1);

    // Libertar a memória
    freeTabela(t1);
    freeTabela(g3->tab);
    free(g3->stackTabs->tabelas);
    free(g3->stackTabs);
    free(g3);
}

// Função responsável por testar a função copiarTabela
void test_copiarTabela(void)
{
    /* Criar uma tabela 2×3 com mistura de chars */
    TABELA orig = criaTabelaExemplo(2, 3, 'Z');
    orig->tabela[0][1] = 'A';
    orig->tabela[1][2] = 'B';

    TABELA copia = copiarTabela(orig);
    CU_ASSERT_PTR_NOT_NULL(copia);
    CU_ASSERT_EQUAL(copia->l, orig->l);
    CU_ASSERT_EQUAL(copia->c, orig->c);

    for (int i = 0; i < orig->l; i++)
    {
        for (int j = 0; j < orig->c; j++)
        {
            CU_ASSERT_EQUAL(copia->tabela[i][j], orig->tabela[i][j]);
        }
        /* Verifica o terminador '\0' em cada linha */
        CU_ASSERT_EQUAL(copia->tabela[i][orig->c], '\0');
    }

    freeTabela(orig);
    freeTabela(copia);
}

// Função responsvel por testar a função freeStackTabs
void test_freeStackTabs(void)
{
    //  s != NULL
    STACKTABS s = criaStackTabsVazio();
    initStackTabs(s);
    TABELA t1 = criaTabelaExemplo(1, 1, 'X');
    TABELA t2 = criaTabelaExemplo(1, 1, 'Y');
    TABELA t3 = criaTabelaExemplo(1, 1, 'Z');
    insereTabela(s, t1);
    insereTabela(s, t2);
    insereTabela(s, t3);
    CU_ASSERT_EQUAL(s->comprimento, 3);

    freeStackTabs(s);

    freeStackTabs(NULL);
}

// Função responsável por testar a função dentroDosLimites
void test_dentroDosLimites(void)
{
    TABELA t = criaTabelaExemplo(3, 4, 'W');
    CU_ASSERT_TRUE(dentroDosLimites(t, 0, 0));
    CU_ASSERT_TRUE(dentroDosLimites(t, 2, 3));
    CU_ASSERT_FALSE(dentroDosLimites(t, -1, 2));
    CU_ASSERT_FALSE(dentroDosLimites(t, 1, -1));
    CU_ASSERT_FALSE(dentroDosLimites(t, 3, 1));
    CU_ASSERT_FALSE(dentroDosLimites(t, 0, 4));
    freeTabela(t);
}

// Função responsável por testar a função verificaRiscadaVizinhasBrancas
void test_verificaRiscadaVizinhasBrancas(void)
{
    TABELA t = criaTabelaExemplo(3, 3, 'X');
    int restr[4][2] = {{0}};

    // celula não é '#'
    t->tabela[1][1] = 'A';
    CU_ASSERT_EQUAL(verificaRiscadaVizinhasBrancas(t, 1, 1, restr), 0);

    // célula = '#' mas sem vizinhas '#'
    t->tabela[1][1] = '#';
    CU_ASSERT_EQUAL(verificaRiscadaVizinhasBrancas(t, 1, 1, restr), 0);

    // uma vizinha '#' em (0,1)
    t->tabela[0][1] = '#';
    int c1 = verificaRiscadaVizinhasBrancas(t, 1, 1, restr);
    CU_ASSERT_EQUAL(c1, 1);
    CU_ASSERT_EQUAL(restr[0][0], 0);
    CU_ASSERT_EQUAL(restr[0][1], 1);

    // duas vizinhas '#' em (0,1) e (1,0)
    t->tabela[1][0] = '#';
    int c2 = verificaRiscadaVizinhasBrancas(t, 1, 1, restr);
    CU_ASSERT_EQUAL(c2, 2);

    // fora dos limites
    CU_ASSERT_EQUAL(verificaRiscadaVizinhasBrancas(t, -1, -1, restr), 0);

    freeTabela(t);
}

// Função responsável por a função verificaRestricoes
void test_verificaRestricoes(void)
{
    // Tabuleiro 2×2 só com minúsculas 'x'.
    TABELA t_a = criaTabelaExemplo(2, 2, 'x');
    CU_ASSERT_FALSE(verificaRestricoes(t_a, false));
    freeTabela(t_a);

    // Repetição de letra maiúscula
    TABELA t_b = criaTabelaExemplo(2, 2, 'Z');
    t_b->tabela[0][0] = 'H';
    t_b->tabela[1][0] = 'H';
    CU_ASSERT_TRUE(verificaRestricoes(t_b, false));
    freeTabela(t_b);

    // Caminho ortogonal não existe
    TABELA t_c = criaTabelaExemplo(2, 2, '#');
    CU_ASSERT_TRUE(verificaRestricoes(t_c, false));
    freeTabela(t_c);

    // Cobrir verificaRiscadasERepetidas com todos os ramos escreve=true
    TABELA t_viz = criaTabelaExemplo(2, 2, 'X');
    int conta = 0;
    bool tem = false;
    // sem vizinha '#' → não incrementa, não imprime
    t_viz->tabela[0][0] = '#';
    verificaRiscadasERepetidas(t_viz, 0, 0, &conta, &tem, true);
    CU_ASSERT_FALSE(tem);
    CU_ASSERT_EQUAL(conta, 0);
    // '#' com vizinha '#'
    t_viz->tabela[1][0] = '#';
    conta = 0;
    tem = false;
    verificaRiscadasERepetidas(t_viz, 0, 0, &conta, &tem, true);
    CU_ASSERT_TRUE(tem);
    CU_ASSERT_EQUAL(conta, 1);
    // Repetição de letra maiúscula
    for (int i = 0; i < t_viz->l; i++)
        for (int j = 0; j < t_viz->c; j++)
            t_viz->tabela[i][j] = 'A';
    t_viz->tabela[0][1] = 'A'; // repetição ‘A’ em (0,0)-(0,1)
    conta = 1;
    tem = true;
    verificaRiscadasERepetidas(t_viz, 0, 0, &conta, &tem, true);
    CU_ASSERT_TRUE(tem);
    CU_ASSERT_EQUAL(conta, 2);
    freeTabela(t_viz);

    // Cobrir u() e d()
    GAME *g_u1 = criaGameVazio();
    g_u1->stackTabs = criaStackTabsVazio();
    initStackTabs(g_u1->stackTabs);
    bool cmd_u1 = false;
    FILE *old_err_u = stderr;
    stderr = fopen("/dev/null", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(stderr);
    u(g_u1, &cmd_u1);
    fclose(stderr);
    stderr = old_err_u;
    CU_ASSERT_TRUE(cmd_u1);
    free(g_u1->stackTabs->tabelas);
    free(g_u1->stackTabs);
    free(g_u1);

    // u: caso de sucesso (copia de tabela)
    GAME *g_u2 = criaGameVazio();
    g_u2->stackTabs = criaStackTabsVazio();
    initStackTabs(g_u2->stackTabs);
    TABELA uu1 = criaTabelaExemplo(1, 1, 'A');
    TABELA uu2 = criaTabelaExemplo(1, 1, 'B');
    insereTabela(g_u2->stackTabs, uu1);
    insereTabela(g_u2->stackTabs, uu2);
    g_u2->stackTabs->indice = 1; /* índice < comprimento */
    g_u2->tab = uu1;
    bool cmd_u2 = false;
    u(g_u2, &cmd_u2);
    CU_ASSERT_TRUE(cmd_u2);
    CU_ASSERT_PTR_NOT_EQUAL(g_u2->tab, uu1);
    CU_ASSERT_EQUAL(g_u2->tab->l, uu2->l);
    CU_ASSERT_EQUAL(g_u2->tab->c, uu2->c);
    freeTabela(g_u2->tab);
    freeTabela(uu2);
    free(g_u2->stackTabs->tabelas);
    free(g_u2->stackTabs);
    free(g_u2);

    // d: ramo fprintf(stderr) */
    GAME *g_d1 = criaGameVazio();
    g_d1->stackTabs = criaStackTabsVazio();
    initStackTabs(g_d1->stackTabs);
    bool cmd_d1 = false;
    FILE *old_err_d = stderr;
    stderr = fopen("/dev/null", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(stderr);
    d(g_d1, &cmd_d1);
    fclose(stderr);
    stderr = old_err_d;
    CU_ASSERT_TRUE(cmd_d1);
    free(g_d1->stackTabs->tabelas);
    free(g_d1->stackTabs);
    free(g_d1);

    // d: ramo de sucesso (deleteTabela==true) */
    GAME *g_d2 = criaGameVazio();
    g_d2->stackTabs = criaStackTabsVazio();
    initStackTabs(g_d2->stackTabs);
    TABELA dd1 = criaTabelaExemplo(1, 1, 'L');
    TABELA dd2 = criaTabelaExemplo(1, 1, 'M');
    insereTabela(g_d2->stackTabs, dd1);
    insereTabela(g_d2->stackTabs, dd2);
    g_d2->tab = dd2;
    bool cmd_d2 = false;
    d(g_d2, &cmd_d2);
    CU_ASSERT_TRUE(cmd_d2);
    CU_ASSERT_PTR_NOT_EQUAL(g_d2->tab, dd2);
    CU_ASSERT_EQUAL(g_d2->tab->l, dd1->l);
    CU_ASSERT_EQUAL(g_d2->tab->c, dd1->c);
    freeTabela(dd1);
    /* dd2 já foi liberado internamente */
    freeTabela(g_d2->tab);
    free(g_d2->stackTabs->tabelas);
    free(g_d2->stackTabs);
    free(g_d2);

    // v
    // game.tab == NULL
    GAME gv1;
    gv1.tab = NULL;
    gv1.stackTabs = NULL;
    bool cont_v1 = true;
    bool cmd_v1 = false;
    FILE *old_err_v = stderr;
    stderr = fopen("/dev/null", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(stderr);
    v(gv1, &cont_v1, &cmd_v1);
    fclose(stderr);
    stderr = old_err_v;
    CU_ASSERT_FALSE(cont_v1);
    CU_ASSERT_FALSE(cmd_v1);

    // game.tab != NULL
    GAME gv2;
    TABELA tv = criaTabelaExemplo(1, 1, 'x');
    gv2.tab = tv;
    gv2.stackTabs = NULL;
    bool cont_v2 = true;
    bool cmd_v2 = false;
    FILE *old_out_v = stdout;
    stdout = fopen("/dev/null", "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(stdout);
    v(gv2, &cont_v2, &cmd_v2);
    fclose(stdout);
    stdout = old_out_v;
    CU_ASSERT_TRUE(cmd_v2);
    CU_ASSERT_TRUE(cont_v2);

    freeTabela(tv);
}