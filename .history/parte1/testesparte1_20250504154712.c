#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parte1.h" // Certifica-te que este header contém as declarações das funções e tipos usados
#include "../parte2/parte2.h"

// Testa a função pintarBranco
void test_pintarBranco(void)
{
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);

    // Coloca um caractere minúsculo para ser convertido
    t->tabela[1][1] = 'a';
    CU_ASSERT_TRUE(pintarBranco(t, 1, 1));
    CU_ASSERT_EQUAL(t->tabela[1][1], 'A');

    // Testa índices fora dos limites
    CU_ASSERT_FALSE(pintarBranco(t, -1, 1));
    CU_ASSERT_FALSE(pintarBranco(t, 3, 3));

    freeTabela(t);
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
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);

    t->tabela[0][0] = 'A';
    t->tabela[1][1] = 'B';
    t->tabela[2][2] = 'C';

    // Chamada normal
    mostrarTabela(t);
    // Chamada com NULL (deve imprimir "Tabuleiro não inicializado.")
    mostrarTabela(NULL);

    freeTabela(t);
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
    game.estado.looping = true;

    // gravação válida
    game.tab = malloc(sizeof(struct Tabela));
    initTabela(game.tab, 2, 2);
    game.tab->tabela[0][0] = 'A';
    game.tab->tabela[0][1] = 'B';
    game.tab->tabela[1][0] = 'C';
    game.tab->tabela[1][1] = 'D';

    CU_ASSERT_TRUE(gravar('g', (char *)filename, &game));

    FILE *f = fopen(filename, "r");
    CU_ASSERT_PTR_NOT_NULL(f);

    int l, c;
    if (fscanf(f, "%d %d", &l, &c) == 2)
    {
        CU_ASSERT_EQUAL(l, 2);
        CU_ASSERT_EQUAL(c, 2);
    }
    char buffer[LINE_SIZE];
    if (fscanf(f, "%s", buffer) == 1)
        CU_ASSERT_STRING_EQUAL(buffer, "AB");
    if (fscanf(f, "%s", buffer) == 1)
        CU_ASSERT_STRING_EQUAL(buffer, "CD");

    fclose(f);
    remove(filename);
    freeTabela(game.tab);

    // argumento NULL
    game.tab = malloc(sizeof(struct Tabela));
    initTabela(game.tab, 2, 2);
    CU_ASSERT_FALSE(gravar('g', NULL, &game));
    freeTabela(game.tab);

    // tabuleiro NULL
    game.tab = NULL;
    CU_ASSERT_FALSE(gravar('g', "teste.txt", &game));

    // erro ao abrir ficheiro (nome inválido)
    game.tab = malloc(sizeof(struct Tabela));
    initTabela(game.tab, 1, 1);
    CU_ASSERT_FALSE(gravar('g', "/csdfsdfsdf/ficheiro.txt", &game));
    freeTabela(game.tab);

    // comando inválido (diferente de 'g')
    CU_ASSERT_FALSE(gravar('x', "teste.txt", &game));
}

// Testa o comando lerCmd
void test_lerCmd(void)
{
    GAME game;
    game.estado.looping = true;
    game.tab = NULL;
    char *filename = "temp_test_ler.txt";
    game.stackTabs = NULL;
    FILE *f;

    // --- Bloco 1: caso válido ---
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL(f);
    fprintf(f, "2 2\n");
    fprintf(f, "EF\n");
    fprintf(f, "GH\n");
    fclose(f);

    CU_ASSERT_TRUE(lerCmd('l', (char *)filename, &game));
    CU_ASSERT_PTR_NOT_NULL(game.tab);
    CU_ASSERT_EQUAL(game.tab->l, 2);
    CU_ASSERT_EQUAL(game.tab->c, 2);
    CU_ASSERT_EQUAL(game.tab->tabela[0][0], 'E');
    CU_ASSERT_EQUAL(game.tab->tabela[0][1], 'F');
    CU_ASSERT_EQUAL(game.tab->tabela[1][0], 'G');
    CU_ASSERT_EQUAL(game.tab->tabela[1][1], 'H');
    freeTabela(game.tab);
    freeStackTabs(game.stackTabs);
    game.tab = NULL;
    game.stackTabs = NULL;
    remove(filename);

    // --- Bloco 2: ficheiro não existe ---
    CU_ASSERT_FALSE(lerCmd('l', "ficheiro_inexistente.txt", &game));

    // --- Bloco 3: ficheiro com dimensões inválidas ---
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL(f);
    fprintf(f, "duas palavras\n"); // não são números válidos
    fclose(f);

    CU_ASSERT_FALSE(lerCmd('l', (char *)filename, &game));
    remove(filename);

    // --- Bloco 4: conteúdo incompleto ---
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL(f);
    fprintf(f, "2 2\n"); // dimensões corretas
    fprintf(f, "A\n");   // conteúdo incompleto
    fclose(f);

    CU_ASSERT_FALSE(lerCmd('l', (char *)filename, &game));
    remove(filename);
}
