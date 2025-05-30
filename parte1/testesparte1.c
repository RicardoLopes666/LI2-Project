#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parte1.h"
#include "../parte2/parte2.h"

// Testa a função pintarBranco
void test_pintarBranco(void)
{
    TABELA t = malloc(sizeof(struct Tabela));
    TABELA p = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);
    initTabela(p, 3, 3);
    // Coloca um caractere minúsculo para ser convertido
    t->tabela[1][1] = 'a';
    t->tabela[0][0] = '#';
    p->tabela[0][0] = 'a';
    CU_ASSERT_TRUE(pintarBranco(t, 1, 1, p));
    CU_ASSERT_EQUAL(t->tabela[1][1], 'A');
    CU_ASSERT_TRUE(pintarBranco(t, 0, 0, p));
    CU_ASSERT_EQUAL(t->tabela[0][0], 'A');

    // Testa índices fora dos limites
    CU_ASSERT_FALSE(pintarBranco(t, -1, 1, p));
    CU_ASSERT_FALSE(pintarBranco(t, 3, 3, p));

    freeTabela(t);
    freeTabela(p);
}

// Testa a função riscar
void test_riscar(void)
{
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);

    // Risca a célula (0,0)
    CU_ASSERT_TRUE(riscar(t, 0, 0));
    CU_ASSERT_EQUAL(t->tabela[0][0], '#');

    // Testa índices inválidos
    CU_ASSERT_FALSE(riscar(t, -1, 0));
    CU_ASSERT_FALSE(riscar(t, 3, 3));

    freeTabela(t);
}

// Testa a função mostrarTabela, incluindo o caso em que a tabela é NULL
void test_mostrarTabela(void)
{
    GAME g;
    TABELA t = malloc(sizeof(struct Tabela));
    TABELA s = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);
    initTabela(s, 3, 3);
    g.tab = t;
    g.solution = s;
    for (int i = 0; i < t->l; i++)
    {
        for (int j = 0; j < t->c; j++)
        {
            g.tab->tabela[i][j] = 'A';
            g.solution->tabela[i][j] = 'A';
        }
    }

    // Chamada normal
    mostrarTabela(g, 0);
    // Chamada com NULL (deve imprimir "Tabuleiro não inicializado.")
    freeTabela(t);
    g.tab = NULL;
    mostrarTabela(g, 0);
    freeTabela(g.solution);
}

// Testa a função coordenadaParaIndice
void test_coordenadaParaIndice(void)
{
    int linha, coluna;
    // Caso válido
    CU_ASSERT_TRUE(coordenadaParaIndice("a1", &linha, &coluna));
    CU_ASSERT_EQUAL(linha, 0);
    CU_ASSERT_EQUAL(coluna, 0);

    CU_ASSERT_TRUE(coordenadaParaIndice("c3", &linha, &coluna));
    CU_ASSERT_EQUAL(linha, 2);
    CU_ASSERT_EQUAL(coluna, 2);

    // Caso inválido: string com comprimento insuficiente
    CU_ASSERT_FALSE(coordenadaParaIndice("a", &linha, &coluna));
}

// Testa o comando sair
void test_sair(void)
{
    GAME game;
    game.tab = NULL;
    game.estado.looping = true;

    // Se o argumento não for NULL, a função deve retornar false
    CU_ASSERT_FALSE(sair('s', "arg", &game));

    // Caso correto: sem argumento e comando 's'
    CU_ASSERT_TRUE(sair('s', NULL, &game));
    CU_ASSERT_FALSE(game.estado.looping);
}

// Testa o comando gravar
void test_gravar(void)
{
    char *filename = "temp_test_gravar.txt";
    GAME game;

    //  Estado válido com 1 tabela na stack
    game.estado.looping = true;

    // Inicializa stackTabs //
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs); // Testar se é NULL
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Cria e preenche 1 tabela 2×2
    TABELA t = malloc(sizeof(struct Tabela));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t);
    initTabela(t, 2, 2);
    t->tabela[0][0] = 'A';
    t->tabela[0][1] = 'B';
    t->tabela[1][0] = 'C';
    t->tabela[1][1] = 'D';

    // Insere na stack e actualiza game.tab
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, t));
    game.tab = copiarTabela(t);
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.tab);

    // Testar a gravacao
    CU_ASSERT_TRUE(gravar('g', filename, &game));

    // Verifica ficheiro gerado
    FILE *f = fopen(filename, "r");
    CU_ASSERT_PTR_NOT_NULL(f);

    int l, c;
    CU_ASSERT_EQUAL(fscanf(f, "%d %d", &l, &c), 2);
    CU_ASSERT_EQUAL(l, 2);
    CU_ASSERT_EQUAL(c, 2);

    char buffer[LINE_SIZE];
    CU_ASSERT_EQUAL(fscanf(f, "%s", buffer), 1);
    CU_ASSERT_STRING_EQUAL(buffer, "AB");
    CU_ASSERT_EQUAL(fscanf(f, "%s", buffer), 1);
    CU_ASSERT_STRING_EQUAL(buffer, "CD");

    fclose(f);
    remove(filename);

    freeTabela(game.tab);
    freeStackTabs(game.stackTabs);

    // Argumento NULL
    // Precisamos de uma stack válida para não causar segfault no loop
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    CU_ASSERT_FALSE(gravar('g', NULL, &game));
    freeStackTabs(game.stackTabs);

    // Tabuleiro NULL
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    game.tab = NULL;
    CU_ASSERT_FALSE(gravar('g', "teste.txt", &game));
    freeStackTabs(game.stackTabs);

    // Erro ao abrir ficheiro
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    // Insere uma tabela generica para passar o check de game->tab
    t = malloc(sizeof(struct Tabela));
    initTabela(t, 1, 1);
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, t));
    game.tab = copiarTabela(t);
    CU_ASSERT_FALSE(gravar('g', "/csdfsdfsdf/ficheiro.txt", &game));
    freeTabela(game.tab);
    freeStackTabs(game.stackTabs);

    // Comando inválido
    // Stack e tab já não são usados se cmd!='g', mas evitamos segfault
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    t = malloc(sizeof(struct Tabela));
    initTabela(t, 1, 1);
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, t));
    game.tab = copiarTabela(t);
    CU_ASSERT_FALSE(gravar('x', "teste.txt", &game));
    freeTabela(game.tab);
    freeStackTabs(game.stackTabs);
}

// Testa o comando lerCmd
void test_lerCmd(void)
{
    GAME game;
    char *filename = "temp_test_ler.txt";
    FILE *f;

    // Caso válido
    game.estado.looping = true;
    game.tab = NULL;
    game.solution = NULL;

    // Prepara stack vazio
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Cria ficheiro com um tabuleiro 2×2
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "2 2\nee\nge\n");
    fclose(f);

    //  ler com sucesso
    CU_ASSERT_TRUE(lerCmd('l', filename, &game));
    CU_ASSERT_PTR_NOT_NULL(game.tab);
    // Stack com 1 tabela
    CU_ASSERT_EQUAL(game.stackTabs->comprimento, 1);
    // Verifica dimensões e conteúdo
    CU_ASSERT_EQUAL(game.tab->l, 2);
    CU_ASSERT_EQUAL(game.tab->c, 2);
    CU_ASSERT_EQUAL(game.tab->tabela[0][0], 'e');
    CU_ASSERT_EQUAL(game.tab->tabela[0][1], 'e');
    CU_ASSERT_EQUAL(game.tab->tabela[1][0], 'g');
    CU_ASSERT_EQUAL(game.tab->tabela[1][1], 'e');

    freeTabela(game.tab);
    freeTabela(game.solution);
    freeStackTabs(game.stackTabs);
    remove(filename);

    // Comando inválido
    game.estado.looping = true;
    game.tab = NULL;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    CU_ASSERT_FALSE(lerCmd('x', filename, &game));
    freeStackTabs(game.stackTabs);

    //  Argumento NULL
    game.estado.looping = true;
    game.tab = NULL;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    CU_ASSERT_FALSE(lerCmd('l', NULL, &game));
    freeStackTabs(game.stackTabs);

    //  Ficheiro inexistente
    game.estado.looping = true;
    game.tab = NULL;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    CU_ASSERT_FALSE(lerCmd('l', "ficheiro_inexistente.txt", &game));
    freeStackTabs(game.stackTabs);

    // Formato de dimensões inválido
    game.estado.looping = true;
    game.tab = NULL;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "duas palavras\n");
    fclose(f);

    CU_ASSERT_FALSE(lerCmd('l', filename, &game));
    freeStackTabs(game.stackTabs);
    remove(filename);

    // Conteúdo incompleto
    game.estado.looping = true;
    game.tab = NULL;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    // Dimensões corretas, mas apenas uma linha de dados
    fprintf(f, "2 2\nA\n");
    fclose(f);

    CU_ASSERT_FALSE(lerCmd('l', filename, &game)); // A stackTabs já é libertada no na função lerCmd
    remove(filename);
}