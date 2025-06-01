#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parte1.h"
#include "../parte2/parte2.h"
#include "../parte4/parte4.h"

#define LINE_SIZE 1024

// Helper to create uma TABELA preenchida com um caractere específico
static TABELA create_filled_table(int l, int c, char fill)
{
    TABELA t = malloc(sizeof(struct Tabela));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t);
    initTabela(t, l, c);
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            t->tabela[i][j] = fill;
        }
    }
    return t;
}

// -----------------------------------------------------------------------------
// test_pintarBranco
// -----------------------------------------------------------------------------
void test_pintarBranco()
{
    TABELA t = malloc(sizeof(struct Tabela));
    TABELA p = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);
    initTabela(p, 3, 3);
    // Coloca um caractere minúsculo a converter
    t->tabela[1][1] = 'a';
    t->tabela[0][0] = '#';
    p->tabela[0][0] = 'a';
    CU_ASSERT_TRUE(pintarBranco(t, 1, 1, p));
    CU_ASSERT_EQUAL(t->tabela[1][1], 'A');
    CU_ASSERT_TRUE(pintarBranco(t, 0, 0, p));
    CU_ASSERT_EQUAL(t->tabela[0][0], 'A');

    // Índices fora dos limites
    CU_ASSERT_FALSE(pintarBranco(t, -1, 1, p));
    CU_ASSERT_FALSE(pintarBranco(t, 3, 3, p));

    freeTabela(t);
    freeTabela(p);
}

// -----------------------------------------------------------------------------
// test_riscar
// -----------------------------------------------------------------------------
void test_riscar()
{
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);

    // Risca célula (0,0)
    CU_ASSERT_TRUE(riscar(t, 0, 0));
    CU_ASSERT_EQUAL(t->tabela[0][0], '#');

    // Índices inválidos
    CU_ASSERT_FALSE(riscar(t, -1, 0));
    CU_ASSERT_FALSE(riscar(t, 3, 3));

    freeTabela(t);
}

// -----------------------------------------------------------------------------
// test_mostrarTabela
// -----------------------------------------------------------------------------
void test_mostrarTabela()
{
    GAME g;
    TABELA t = malloc(sizeof(struct Tabela));
    TABELA s = malloc(sizeof(struct Tabela));
    initTabela(t, 2, 2);
    initTabela(s, 2, 2);
    // Preenche tab e solution para cobrir isC = 0
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
    // isC = 0
    mostrarTabela(g, 0);

    // isC = 1 com célula riscada e célula maiúscula que não bate com solution
    g.tab->tabela[0][0] = '#';
    g.solution->tabela[0][0] = '#';
    g.tab->tabela[0][1] = 'B';
    g.solution->tabela[0][1] = 'C';
    mostrarTabela(g, 1);

    // Chamada com NULL (imprime "Tabuleiro não inicializado.")
    freeTabela(t);
    g.tab = NULL;
    mostrarTabela(g, 0);

    freeTabela(s);
}

// -----------------------------------------------------------------------------
// test_coordenadaParaIndice
// -----------------------------------------------------------------------------
void test_coordenadaParaIndice()
{
    int linha, coluna;
    // Valido
    CU_ASSERT_TRUE(coordenadaParaIndice("a1", &linha, &coluna));
    CU_ASSERT_EQUAL(linha, 0);
    CU_ASSERT_EQUAL(coluna, 0);

    CU_ASSERT_TRUE(coordenadaParaIndice("c3", &linha, &coluna));
    CU_ASSERT_EQUAL(linha, 2);
    CU_ASSERT_EQUAL(coluna, 2);

    // Inválido: string muito curta
    CU_ASSERT_FALSE(coordenadaParaIndice("a", &linha, &coluna));
}

// -----------------------------------------------------------------------------
// test_sair
// -----------------------------------------------------------------------------
void test_sair()
{
    GAME game;
    game.tab = NULL;
    game.estado.looping = true;

    // Se argumento != NULL retorna false
    CU_ASSERT_FALSE(sair('s', "arg", &game));

    // Sem argumento, cmd='s'
    CU_ASSERT_TRUE(sair('s', NULL, &game));
    CU_ASSERT_FALSE(game.estado.looping);
}

// -----------------------------------------------------------------------------
// test_gravar
// -----------------------------------------------------------------------------
void test_gravar()
{
    char *filename = "temp_test_gravar.txt";
    GAME game;

    // 1 tabela na stack
    game.estado.looping = true;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Cria e preenche tabela 2×2
    TABELA t = malloc(sizeof(struct Tabela));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t);
    initTabela(t, 2, 2);
    t->tabela[0][0] = 'A';
    t->tabela[0][1] = 'B';
    t->tabela[1][0] = 'C';
    t->tabela[1][1] = 'D';

    // Insere na stack e atualiza game.tab
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, t));
    game.tab = copiarTabela(t);
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.tab);

    // Grava
    CU_ASSERT_TRUE(gravar('g', filename, &game));

    // Verifica ficheiro
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

    // Erro a abrir ficheiro
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    t = malloc(sizeof(struct Tabela));
    initTabela(t, 1, 1);
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, t));
    game.tab = copiarTabela(t);
    CU_ASSERT_FALSE(gravar('g', "/nonexistent/path/file.txt", &game));
    freeTabela(game.tab);
    freeStackTabs(game.stackTabs);

    // Comando inválido
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

// -----------------------------------------------------------------------------
// test_lerCmd_fileNotFound
// -----------------------------------------------------------------------------
void test_lerCmd_fileNotFound()
{
    GAME game;
    game.tab = NULL;
    game.solution = NULL;
    game.estado.looping = true;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Arquivo não existe
    CU_ASSERT_FALSE(lerCmd('l', "arquivo_inexistente.txt", &game));

    freeStackTabs(game.stackTabs);
}

// -----------------------------------------------------------------------------
// test_lerCmd_freeOldTab
// -----------------------------------------------------------------------------
void test_lerCmd_freeOldTab()
{
    GAME game;
    char *filename = "temp_test_ler_free.txt";
    FILE *f;

    // Prepara game.tab
    game.tab = create_filled_table(1, 1, 'a');
    game.solution = NULL;
    game.estado.looping = true;

    // Inicializa stackTabs
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Grava um tabuleiro 1×1
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "1 1\n");
    fprintf(f, "a\n");
    fclose(f);

    // Ler: deve chamar freeTabela(old) e colocar nova tabela via resolve
    CU_ASSERT_TRUE(lerCmd('l', filename, &game));
    CU_ASSERT_PTR_NOT_NULL(game.tab);
    CU_ASSERT_EQUAL(game.tab->tabela[0][0], 'a');

    freeTabela(game.tab);
    freeTabela(game.solution);
    freeStackTabs(game.stackTabs);
    remove(filename);
}

// -----------------------------------------------------------------------------
// test_lerCmd_emptyFile
// -----------------------------------------------------------------------------
void test_lerCmd_emptyFile()
{
    GAME game;
    char *filename = "temp_test_ler_empty.txt";
    FILE *f;

    game.tab = NULL;
    game.solution = NULL;
    game.estado.looping = true;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Ficheiro vazio
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fclose(f);

    CU_ASSERT_FALSE(lerCmd('l', filename, &game));

    freeStackTabs(game.stackTabs);
    remove(filename);
}

// -----------------------------------------------------------------------------
// test_lerCmd_incompleteData
// -----------------------------------------------------------------------------
void test_lerCmd_incompleteData()
{
    GAME game;
    char *filename = "temp_test_ler_incomplete.txt";
    FILE *f;

    game.tab = NULL;
    game.solution = NULL;
    game.estado.looping = true;
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Dimensões 2×2 mas só um caractere
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "2 2\n");
    fprintf(f, "A\n");
    fclose(f);

    CU_ASSERT_FALSE(lerCmd('l', filename, &game));

    remove(filename);
}

// -----------------------------------------------------------------------------
// test_leTabuleiro_incomplete
// -----------------------------------------------------------------------------
void test_leTabuleiro_incomplete()
{
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 2, 2);

    // Cria um ficheiro com apenas um caractere em vez de 4
    char *filename = "temp_leTabuleiro_incomplete.txt";
    FILE *f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "A\n");
    fclose(f);

    // Abre modo leitura e avança o cursor para a parte do conteúdo
    f = fopen(filename, "r");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    // Tenta ler 2×2: deve falhar na primeira chamada a fscanf
    CU_ASSERT_FALSE(leTabuleiro(&t, 2, 2, f));
    fclose(f);

    freeTabela(t);
    remove(filename);
}

void test_colocaSolucao_nullResolve()
{
    GAME game;

    // Inicializa stackTabs com um tabuleiro impossível de resolver,
    // por exemplo, 1×1 contendo '#'. Resolve deve devolver NULL.
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    TABELA t1 = create_filled_table(1, 1, '#');
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, t1));

    game.solution = create_filled_table(1, 1, 'X');

    // Chama colocaSolucao: como resolve(...) retora NULL, game.solution deverá ficar NULL
    colocaSolucao(&game);
    CU_ASSERT_PTR_NULL(game.solution);

    freeStackTabs(game.stackTabs);
}

// Teste de colocar a tabela na stack
void test_colocaTabelaNaStack_fail()
{
    GAME game;
    // Cria um stackTabs “não inicializado”: malloc sem initStackTabs
    game.stackTabs = malloc(sizeof(struct StackTabs));
    // Forçamos insereTabela a ler members inválidos e a falhar
    // Aqui apenas verificamos que colocaTabelaNaStack retorna false sem segfault
    TABELA t = create_filled_table(1, 1, 'A');
    CU_ASSERT_FALSE(colocaTabelaNaStack(&game, t));
    // Como falhou, t já foi freeTabela dentro da função
    // E game.stackTabs foi freeStackTabs. Evitamos double‐free.
}

// -----------------------------------------------------------------------------
// test_i
// -----------------------------------------------------------------------------
void test_i()
{
    GAME game;
    bool comandoProcessed = false;

    // Inicializa stackTabs com 2 tabelas diferentes
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    TABELA tabela1 = create_filled_table(2, 2, 'X');
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, tabela1));

    TABELA tabela2 = create_filled_table(2, 2, 'Y');
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, tabela2));

    // game.tab temporário
    TABELA temp = create_filled_table(2, 2, 'Z');
    game.tab = temp;

    // Chama i(): substitui game.tab pela cópia de tabela1
    i(&game, &comandoProcessed);
    CU_ASSERT_TRUE(comandoProcessed);
    for (int i = 0; i < game.tab->l; i++)
    {
        for (int j = 0; j < game.tab->c; j++)
        {
            CU_ASSERT_EQUAL(game.tab->tabela[i][j], 'X');
        }
    }

    freeTabela(game.tab);
    freeStackTabs(game.stackTabs);
}

// -----------------------------------------------------------------------------
// test_e_correctMatches
// -----------------------------------------------------------------------------
void test_e_correctMatches()
{
    GAME game;
    bool comandoProcessed = false;

    TABELA t = malloc(sizeof(struct Tabela));
    TABELA s = malloc(sizeof(struct Tabela));
    initTabela(t, 1, 2);
    initTabela(s, 1, 2);

    // Preenche de modo que apenas uma célula bate
    t->tabela[0][0] = 'A'; // bate
    t->tabela[0][1] = 'b'; // minúscula
    s->tabela[0][0] = 'A';
    s->tabela[0][1] = 'B';

    game.tab = t;
    game.solution = s;

    e(game, &comandoProcessed);
    CU_ASSERT_TRUE(comandoProcessed);

    freeTabela(t);
    freeTabela(s);
}

// -----------------------------------------------------------------------------
// test_e_violation
// -----------------------------------------------------------------------------
void test_e_violation()
{
    GAME game;
    bool comandoProcessed = false;

    // Cria tabuleiro 1×2 e solution 1×2 que viola restrição:
    // duas letras iguais na mesma linha (por ex., 'A' e 'A')
    TABELA t = malloc(sizeof(struct Tabela));
    TABELA s = malloc(sizeof(struct Tabela));
    initTabela(t, 1, 2);
    initTabela(s, 1, 2);

    t->tabela[0][0] = 'A';
    t->tabela[0][1] = 'A'; // duplicado -> viola restrição

    s->tabela[0][0] = 'A';
    s->tabela[0][1] = 'B';

    game.tab = t;
    game.solution = s;

    e(game, &comandoProcessed);
    CU_ASSERT_TRUE(comandoProcessed);

    freeTabela(t);
    freeTabela(s);
}
