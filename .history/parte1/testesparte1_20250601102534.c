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

// Helper to create a small table filled with a specific character
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

// Testa a função pintarBranco
void test_pintarBranco()
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
void test_riscar()
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
void test_mostrarTabela()
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

    // Chamada normal (apenas para cobertura; não validamos saída)
    mostrarTabela(g, 0);

    // Chamada com NULL (deve imprimir "Tabuleiro não inicializado.")
    freeTabela(t);
    g.tab = NULL;
    mostrarTabela(g, 0);

    freeTabela(s);
}

// Testa a função coordenadaParaIndice
void test_coordenadaParaIndice()
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
void test_sair()
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
void test_gravar()
{
    char *filename = "temp_test_gravar.txt";
    GAME game;

    // Estado válido com 1 tabela na stack
    game.estado.looping = true;

    // Inicializa stackTabs
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Cria e preenche 1 tabela 2×2
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

    // Testar a gravação
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
    t = malloc(sizeof(struct Tabela));
    initTabela(t, 1, 1);
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, t));
    game.tab = copiarTabela(t);
    CU_ASSERT_FALSE(gravar('g', "/nonexistent/path/file.txt", &game));
    freeTabela(game.tab);
    freeStackTabs(game.stackTabs);

    // Comando inválido (cmd != 'g')
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

// Testa o comando lerCmd (cenários básicos já cobertos)
// Agora adicionamos casos adicionais para atingir 100% de cobertura

// 1) Ler com game.tab não nulo - garante que freeTabela(game->tab) é chamado
void test_lerCmd_freeOldTab()
{
    GAME game;
    char *filename = "temp_test_ler_free.txt";
    FILE *f;

    // Prepara game.tab existente
    game.tab = create_filled_table(1, 1, 'x');
    game.solution = NULL;
    game.estado.looping = true;

    // Inicializa stackTabs
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Cria ficheiro com um tabuleiro 1×1
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "1 1\n");
    fprintf(f, "z\n");
    fclose(f);

    // Ler deve chamar freeTabela(old) e depois atribuir nova tabela
    CU_ASSERT_TRUE(lerCmd('l', filename, &game));
    CU_ASSERT_PTR_NOT_NULL(game.tab);
    CU_ASSERT_EQUAL(game.tab->l, 1);
    CU_ASSERT_EQUAL(game.tab->c, 1);
    CU_ASSERT_EQUAL(game.tab->tabela[0][0], 'z');

    freeTabela(game.tab);
    freeTabela(game.solution);
    freeStackTabs(game.stackTabs);
    remove(filename);
}

// 2) Ler arquivo vazio (comprimento == 0) - cobre o ramo de erro "o ficheiro não contem nenhum tabuleiro"
void test_lerCmd_emptyFile()
{
    GAME game;
    char *filename = "temp_test_ler_empty.txt";
    FILE *f;

    game.tab = NULL;
    game.solution = NULL;
    game.estado.looping = true;

    // Inicializa stackTabs
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Cria ficheiro vazio (sem números)
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fclose(f);

    // Ler deve falhar com comprimento == 0
    CU_ASSERT_FALSE(lerCmd('l', filename, &game));

    freeStackTabs(game.stackTabs);
    remove(filename);
}

// 3) Ler arquivo com formato de dimensões válido, mas conteúdo incompleto no leTabuleiro
void test_lerCmd_incompleteData()
{
    GAME game;
    char *filename = "temp_test_ler_incomplete.txt";
    FILE *f;

    game.tab = NULL;
    game.solution = NULL;
    game.estado.looping = true;

    // Inicializa stackTabs
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Cria ficheiro com dimensões 2×2, mas apenas 1 caractere
    f = fopen(filename, "w");
    CU_ASSERT_PTR_NOT_NULL_FATAL(f);
    fprintf(f, "2 2\n");
    fprintf(f, "A\n");
    fclose(f);

    // Ler deve falhar dentro de leTabuleiro, cobrindo freeTabela(t), freeStackTabs, e retorno false
    CU_ASSERT_FALSE(lerCmd('l', filename, &game));

    remove(filename);
}

// Testa colocaSolucao cobrindo o freeTabela(temp) quando game.solution já existe
void test_colocaSolucao()
{
    GAME game;

    // Inicializa stackTabs com uma tabela
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));
    TABELA t1 = create_filled_table(1, 1, 'm');
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, t1));

    // Prepara game.solution existente
    game.solution = create_filled_table(1, 1, 'n');

    // Chama colocaSolucao: temp (n) deve ser freeTabela, e game.solution recebe cópia de t1
    colocaSolucao(&game);
    CU_ASSERT_PTR_NOT_NULL(game.solution);
    CU_ASSERT_EQUAL(game.solution->tabela[0][0], 'M');

    freeTabela(game.solution);
    freeStackTabs(game.stackTabs);
}

// Testa a função i (volta ao primeiro estado da stack)
void test_i()
{
    GAME game;
    bool comandoProcessed = false;

    // Inicializa stackTabs com duas tabelas diferentes
    game.stackTabs = malloc(sizeof(struct StackTabs));
    CU_ASSERT_PTR_NOT_NULL_FATAL(game.stackTabs);
    CU_ASSERT_TRUE_FATAL(initStackTabs(game.stackTabs));

    // Cria tabela1 cheia de 'X'
    TABELA tabela1 = create_filled_table(2, 2, 'X');
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, tabela1));

    // Cria tabela2 cheia de 'Y'
    TABELA tabela2 = create_filled_table(2, 2, 'Y');
    CU_ASSERT_TRUE_FATAL(insereTabela(game.stackTabs, tabela2));

    // Define game.tab com uma tabela temporária (preenchida com 'Z')
    TABELA temp = create_filled_table(2, 2, 'Z');
    game.tab = temp;

    // Chama i(): deve substituir game.tab pela cópia de tabela1
    i(&game, &comandoProcessed);
    CU_ASSERT_TRUE(comandoProcessed);
    // Agora game.tab deve conter 'X' em toda parte
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

// Testa a função e (imprime estatísticas e marca comandoProcessado)
// Agora cobre também o ramo onde verifica restrições e imprime violação
void test_e()
{
    GAME game;
    bool comandoProcessed = false;

    // Cria tabuleiro 2x2 e solução 2x2 com restrição violada
    TABELA t = malloc(sizeof(struct Tabela));
    TABELA s = malloc(sizeof(struct Tabela));
    initTabela(t, 2, 2);
    initTabela(s, 2, 2);

    // Preenche t de forma que haja minúscula e riscada e incorreta
    t->tabela[0][0] = 'a'; // minúscula -> não conta como resolvida
    t->tabela[0][1] = '#'; // riscada
    t->tabela[1][0] = 'B'; // incorreta (não bate com solução abaixo)
    t->tabela[1][1] = 'C'; // iremos definir solução diferente

    // Solução diferente para criar restrição
    s->tabela[0][0] = 'A';
    s->tabela[0][1] = 'B';
    s->tabela[1][0] = 'C';
    s->tabela[1][1] = 'D';

    game.tab = t;
    game.solution = s;

    // Chama e(): espera que comandoProcessed seja true e que o ramo else seja executado
    e(game, &comandoProcessed);
    CU_ASSERT_TRUE(comandoProcessed);

    freeTabela(t);
    freeTabela(s);
}
