#include <stdlib.h>
#include <stdbool.h>
#include "../tipos.h"
#include "../parte1/parte1.h"

void initStackTabs(STACKTABS s)
{
    s->capacidade = 0;
    s->comprimento = 0;
    s->tabelas = malloc(sizeof(TABELA) * 3);
}

// Função que insere uma tabela ao array de tabelas
bool insereTabela(STACKTABS s, TABELA tab)
{
    if (s->comprimento >= s->capacidade)
    {
        s->capacidade = s->capacidade == 0 ? 1 : 2 * s->capacidade;
        TABELA *new;
        new = realloc(s->tabelas, s->capacidade * sizeof(TABELA));
        if (new == NULL)
            return false;
        s->tabelas = new;
    }
    s->tabelas[s->comprimento++] = tab;
    return true;
}

TABELA copiarTabela(TABELA t); // Para poder usar em deleteTabela

bool deleteTabela(GAME *game)
{
    if (game->stackTabs->comprimento <= 1) // Dá erro se tentar eliminar a tabela inicial
        return false;
    freeTabela(game->stackTabs->tabelas[--game->stackTabs->comprimento]); // Liberta o espaço da tabela
    freeTabela(game->tab);
    game->tab = copiarTabela(game->stackTabs->tabelas[game->stackTabs->comprimento - 1]);
    return true;
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
                free(t->tabela[i]);
            }
            free(new->tabela);
            free(new);
            return NULL;
        }
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
    for (int i = 0; i < s->comprimento; i++)
    {
        freeTabela(s->tabelas[i]);
    }
    free(s->tabelas);
    free(s);
}