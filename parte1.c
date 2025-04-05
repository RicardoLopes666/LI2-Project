#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LINE_SIZE 1024

typedef struct estado
{
    bool looping;
} ESTADO;

typedef struct Tabela
{
    int l;
    int c;
    char **tabela;
} *TABELA;

void initTabela(TABELA t, int l, int c)
{
    t->l = l;                               // Atribui o número de linhas
    t->c = c;                               // Atribui o número de colunas
    t->tabela = malloc(l * sizeof(char *)); // Aloca memória para um array de ponteiros de char (linha de ponteiros)
    if (t->tabela == NULL)
    {
        fprintf(stderr, "Erro na alocação de memória");
        return;
    }
    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc(c * sizeof(char)); // Aloca memória para cada linha da tabela (vetor de caracteres)
        if (t->tabela[i] == NULL)
        {
            fprintf(stderr, "Erro na alocação de memória");
            return;
        }
    }
}

// Apontador para função
typedef bool (*COMANDO)(char cmd, char *arg, ESTADO *e); // Cria um tipo de dados para funções que recebem um char, um char* e um ESTADO* e devolvem um bool, permitindo assim criar um array de funções

bool gravar(char cmd, char *arg, ESTADO *e)
{
    (void)e; // Ignora o aviso de variável não utilizada
    if (cmd == 'g')
    {
        if (arg == NULL)
        {
            fprintf(stderr, "Erro: o comando gravar precisa de um argumento!\n"); // stderr é o canal de erro padrão
            return false;
        }
        printf("Gravando  em %s\n", arg);
        return true;
    }
    else
        return false;
}

bool sair(char cmd, char *arg, ESTADO *e)
{
    if (arg != NULL)
        return false;
    if (cmd == 's')
    {
        e->looping = false;
        return true;
    }
    else
        return false;
}

bool ler(char cmd, char *arg, ESTADO *e)
{
    (void)e; // Ignora o aviso de variável não utilizada
    if (cmd == 'l')
    {
        if (arg == NULL)
        {
            fprintf(stderr, "Erro: o comando ler precisa de um argumento!\n"); // stderr é o canal de erro padrão
            return false;
        }
        printf("Lendo de %s\n", arg);
        return true;
    }
    else
        return false;
}

int main()
{
    COMANDO comandos[] = {sair, ler, gravar, NULL};
    ESTADO estado;
    estado.looping = true;
    while (estado.looping)
    {
        char line[LINE_SIZE] = {0};
        if (fgets(line, LINE_SIZE, stdin) != NULL)
            assert(line[strlen(line) - 1] == '\n'); // Verifica se a última posição do array é \n se não for aborta o
        else
            estado.looping = false;

        char cmd[LINE_SIZE];
        char arg[LINE_SIZE];
        char resto[LINE_SIZE];
        int num_args = sscanf(line, "%[a-z] %s %[^\n]", cmd, arg, resto); // sscanf neste caso pega na variavel line e lê até 3 strings sendo que se a última existir é sinal de erro do input do utilizador
        if (strlen(cmd) > 1)
        {
            fprintf(stderr, "Erro: comando %s não é válido!\n", cmd); // stderr é o canal de erro padrão
            continue;
        }

        if (num_args > 2) // Verifica se existe o terceiro argumento (resto)
        {
            fprintf(stderr, "Erro: comando %s    foi invocado com argumentos extra: %s\n", cmd, resto); // stderr é o canal de erro padrão
            continue;                                                                                   // Continua a execução do programa (salta esta executa a próxima iteração do while)
        }

        bool ret = false; // variavel usada para sair do loop que corre todas as funções do array comandos
        for (int I = 0; !ret && comandos[I] != NULL; I++)
            ret = comandos[I](cmd[0], (num_args == 2) ? arg : NULL, &estado);
    }
    return 0;
}
