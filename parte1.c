#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> // para toupper
#define LINE_SIZE 1024

// Struct para controlar o estado do programa (se ele deve continuar ou parar)
typedef struct estado
{
    bool looping;
} ESTADO;

// Struct que representa o tabuleiro
typedef struct Tabela
{
    int l;
    int c;
    char **tabela;
} *TABELA;

// Estrutura que agrupa o estado do jogo e o tabuleiro
typedef struct
{
    ESTADO estado;
    TABELA tab;
} GAME;

// Função que cria e aloca o tabuleiro com as dimensões fornecidas
void initTabela(TABELA t, int l, int c)
{
    t->l = l;                               // Atribui o número de linhas
    t->c = c;                               // Atribui o número de colunas
    t->tabela = malloc(l * sizeof(char *)); // Aloca memória para um array de ponteiros de char (linhas)
    if (t->tabela == NULL)
    {
        fprintf(stderr, "Erro na alocação de memória\n");
        return;
    }
    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc(c * sizeof(char)); // Aloca memória para cada linha (vetor de caracteres)
        if (t->tabela[i] == NULL)
        {
            fprintf(stderr, "Erro na alocação de memória\n");
            return;
        }
    }
}

// Função para libertar a memória alocada para um tabuleiro
void freeTabela(TABELA t)
{
    if (t == NULL)
        return;
    for (int i = 0; i < t->l; i++)
    {
        free(t->tabela[i]);
    }
    free(t->tabela);
    free(t);
}

/// Função que lê um ficheiro e cria um novo tabuleiro a partir dele
TABELA lerFicheiro(const char *nome)
{
    FILE *f = fopen(nome, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Erro ao abrir o ficheiro %s para leitura\n", nome);
        return NULL;
    }
    int l, c;
    if (fscanf(f, "%d %d", &l, &c) != 2)
    {
        fprintf(stderr, "Erro na leitura das dimensões do tabuleiro\n");
        fclose(f);
        return NULL;
    }
    TABELA nova = malloc(sizeof(*nova));
    if (nova == NULL)
    {
        fprintf(stderr, "Erro na alocação da estrutura Tabela\n");
        fclose(f);
        return NULL;
    }
    initTabela(nova, l, c);
    for (int i = 0; i < l; i++)
    {
        char buffer[LINE_SIZE];
        if (fscanf(f, "%s", buffer) != 1)
        {
            fprintf(stderr, "Erro na leitura da linha %d do tabuleiro\n", i + 1);
            fclose(f);
            freeTabela(nova);
            return NULL;
        }
        for (int j = 0; j < c; j++)
        {
            nova->tabela[i][j] = buffer[j];
        }
    }
    fclose(f);
    return nova;
}

// Tipo de função para comandos. Agora os comandos recebem um GAME* para acessar o estado e o tabuleiro.
typedef bool (*COMANDO)(char cmd, char *arg, GAME *game);

// Comando para gravar (exemplo simplificado)
bool gravar(char cmd, char *arg, GAME *game)
{
    (void)game; // Não é necessário para este exemplo
    if (cmd == 'g')
    {
        if (arg == NULL)
        {
            fprintf(stderr, "Erro: o comando gravar precisa de um argumento!\n");
            return false;
        }
        printf("Gravando em %s\n", arg);
        // Aqui podes implementar a funcionalidade de gravação
        return true;
    }
    return false;
}

// Comando para sair
bool sair(char cmd, char *arg, GAME *game)
{
    if (arg != NULL)
        return false;
    if (cmd == 's')
    {
        game->estado.looping = false;
        return true;
    }
    return false;
}

// Comando para ler o tabuleiro de um ficheiro
bool lerCmd(char cmd, char *arg, GAME *game)
{
    if (cmd == 'l')
    {
        if (arg == NULL)
        {
            fprintf(stderr, "Erro: o comando ler precisa de um argumento!\n");
            return false;
        }
        TABELA nova = lerFicheiro(arg);
        if (nova == NULL)
            return false;
        // Se já houver um tabuleiro carregado, liberta-o antes de atualizar
        if (game->tab != NULL)
            freeTabela(game->tab);
        game->tab = nova;
        printf("Tabuleiro lido de %s\n", arg);
        return true;
    }
    return false;
}

// Converte coordenada do formato "a3" para índices numéricos (linha e coluna)
bool coordenadaParaIndice(const char *coord, int *linha, int *coluna)
{
    if (strlen(coord) < 2)
        return false;
    *coluna = coord[0] - 'a';     // 'a' corresponde à coluna 0
    *linha = atoi(coord + 1) - 1; // // se os números forem 1-indexados, atoi -> converte uma string para um int   NOTA (coord + 1) é para comecar na segunda letra da string, e tirmaos 1, por causa do indice zero
    return true;
}

// Pinta de branco (transforma para maiúscula)
bool pintarBranco(TABELA t, int linha, int coluna)
{
    if (linha >= 0 && linha < t->l && coluna >= 0 && coluna < t->c)
    {
        t->tabela[linha][coluna] = toupper(t->tabela[linha][coluna]);
        return true;
    }
    return false;
}

// Risca a casa (substitui por '#')
bool riscar(TABELA t, int linha, int coluna)
{
    if (linha >= 0 && linha < t->l && coluna >= 0 && coluna < t->c)
    {
        t->tabela[linha][coluna] = '#';
        return true;
    }
    return false;
}

// Função que mostra o tabuleiro
void mostrarTabela(TABELA t)
{
    if (t == NULL)
    {
        printf("Tabuleiro não inicializado.\n");
        return;
    }
    for (int i = 0; i < t->l; i++)
    {
        for (int j = 0; j < t->c; j++)
        {
            printf("%c ", t->tabela[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    GAME game;
    game.estado.looping = true;
    game.tab = NULL; // Inicialmente, nenhum tabuleiro está carregado

    // Array de comandos: sair, ler, gravar
    COMANDO comandos[] = {sair, lerCmd, gravar, NULL};

    while (game.estado.looping)
    {
        char line[LINE_SIZE] = {0};
        printf("\n> ");
        if (fgets(line, LINE_SIZE, stdin) == NULL)
        {
            game.estado.looping = false;
            break;
        }
        assert(line[strlen(line) - 1] == '\n');

        char cmd[LINE_SIZE] = {0};
        char arg[LINE_SIZE] = {0};
        char resto[LINE_SIZE] = {0};
        int num_args = sscanf(line, "%s %s %[^\n]", cmd, arg, resto);
        if (strlen(cmd) != 1)
        {
            fprintf(stderr, "Erro: comando %s não é válido!\n", cmd);
            continue;
        }
        if (num_args == 3)
        { // Se houver argumentos extras
            fprintf(stderr, "Erro: comando %s foi invocado com argumentos extra: %s\n", cmd, resto);
            continue;
        }

        bool comandoProcessado = false;
        // Processa os comandos padrão (s, l, g)
        for (int i = 0; !comandoProcessado && comandos[i] != NULL; i++)
        {
            comandoProcessado = comandos[i](cmd[0], (num_args >= 2) ? arg : NULL, &game);
        }

        // Processa os comandos de modificação do tabuleiro: 'b' (pintar) e 'r' (riscar)
        if (!comandoProcessado)
        {
            if (cmd[0] == 'b' || cmd[0] == 'r')
            {
                if (arg[0] == '\0')
                {
                    fprintf(stderr, "Erro: comando %c necessita de uma coordenada!\n", cmd[0]);
                    continue;
                }
                int linha, coluna;
                if (!coordenadaParaIndice(arg, &linha, &coluna))
                {
                    fprintf(stderr, "Coordenada inválida!\n");
                    continue;
                }
                if (game.tab == NULL)
                {
                    fprintf(stderr, "Erro: tabuleiro não carregado.\n");
                    continue;
                }
                if (cmd[0] == 'b')
                {
                    if (!pintarBranco(game.tab, linha, coluna))
                        fprintf(stderr, "Erro: Coordenada fora dos limites!\n");
                }
                else
                { // comando 'r'
                    if (!riscar(game.tab, linha, coluna))
                        fprintf(stderr, "Erro: Coordenada fora dos limites!\n");
                }
                comandoProcessado = true;
            }
        }

        if (!comandoProcessado)
        {
            fprintf(stderr, "Comando não reconhecido!\n");
        }

        printf("\nEstado atual do tabuleiro:\n");
        mostrarTabela(game.tab);
    }

    if (game.tab != NULL)
        freeTabela(game.tab);

    return 0;
}
