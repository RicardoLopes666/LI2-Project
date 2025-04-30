#include <stdio.h>
#include <stdlib.h>
#include "../tipos.h"
#include "../parte2/parte2.h"
#include <ctype.h>

// Função que dada uma tabela a percorre por ordem de linhas e colunas até encontrar o primeiro elemento que não estiver riscado, o qual vai ser utilizado para o bfs e devolve esse elemento como uma coordenada
COORDENADA devolvePrimeroNRiscado(TABELA t)
{
    int i = 0;
    while (i < (t->c * t->l) && t->tabela[i / t->c][i % t->c] == '#')
        i++;
    if (i >= t->c * t->l)
        return NULL;
    COORDENADA pos = malloc(sizeof(struct Coordenada));
    pos->l = i / t->c;
    pos->c = i % t->c;
    return pos;
}

// Função que inicializa o array de visitados com 0 exceto nas casas que estão riscas (0 -> não visitado, 1 -> visitado)
int **initVisited(TABELA t, int *count) // Count conta o número de casas visitadas
{
    int **visited = malloc(sizeof(int *) * t->l);
    for (int i = 0; i < t->l; i++)
    {
        visited[i] = malloc(sizeof(int) * t->c);
        for (int j = 0; j < t->c; j++)
            if (t->tabela[i][j] == '#')
            {
                visited[i][j] = 1;
                (*count)++; // Atualiza o número de casas visitadas
            }
            else
                visited[i][j] = 0;
    }
    return visited;
}

void initQueue(CQUEUE q)
{
    q->capacidade = 3;
    q->tamanho = 0;
    q->front = 0;
    q->list = malloc(sizeof(COORDENADA) * q->capacidade);
}

// Função responsável por inserir um array de coordenadas na queue
int insertQueue(CQUEUE q, COORDENADA *coordenadas, int coordSize)
{
    if (coordSize < 1)
        return 1;
    int tam = q->tamanho + coordSize;
    if (tam >= q->capacidade)
    {
        int oldCapacity = q->capacidade;
        while (tam >= q->capacidade)
            q->capacidade = q->capacidade == 0 ? 1 : q->capacidade * 2; // aumenta a capacidade até ter espaço para os novos elementos
        COORDENADA *temp = realloc(q->list, sizeof(COORDENADA) * q->capacidade);
        if (temp == NULL)
        {
            return 0;
        }
        q->list = temp;
        int end = q->front + q->tamanho;
        if (end > oldCapacity)
            for (int i = 0; i < end % oldCapacity; i++)
                q->list[i + oldCapacity] = q->list[i]; // volta a colocar a queue direita
    }
    for (int i = 0; i < coordSize; i++)
    {
        q->list[(q->front + q->tamanho) % q->capacidade] = coordenadas[i];
        q->tamanho++;
    }
    return 1;
}

void deleteQueue(CQUEUE q, COORDENADA *pos)
{
    if (q->tamanho == 0)
        return;
    // retira o elemento da frente, mas não o free — testa vai fazê-lo
    COORDENADA temp = q->list[q->front];
    *pos = temp;
    q->list[q->front] = NULL;
    q->front = (q->front + 1) % q->capacidade;
    q->tamanho--;
}

// Função que dado uma lista de visitados devolve um array com as posições que não foram visitadas
COORDENADA *devolveNaoVisitados(int **visited, int l, int c, int length)
{
    COORDENADA *naoVisitados = malloc(sizeof(COORDENADA) * length);
    int cont = 0;
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (visited[i][j] == 0)
            {
                COORDENADA pos = malloc(sizeof(struct Coordenada));
                pos->l = i;
                pos->c = j;
                naoVisitados[cont++] = pos;
            }
        }
    }
    return naoVisitados;
}

// Algoritmo de procura em largura
int existeCaminhoOrtogonal(CQUEUE q, TABELA t, int **visited, int *count)
{
    int direcoes[4][2] = {
        {-1, 0}, // Cima
        {1, 0},  // Baixo
        {0, -1}, // Esquerda
        {0, 1}   // Direita
    };
    while (q->tamanho > 0)
    {
        COORDENADA atual = NULL;
        deleteQueue(q, &atual); // passa o endereço de 'atual' (COORDENADA*)

        visited[atual->l][atual->c] = 1; // Visitamos a atual;
        (*count)++;
        int numeroVizinhosValidos = 0;
        COORDENADA *vizinhos = malloc(4 * sizeof(COORDENADA)); // no máximo existem 4 vizinhos válidos
        for (int i = 0; i < 4; i++)
        {
            int linha = direcoes[i][0] + atual->l;
            int coluna = direcoes[i][1] + atual->c;

            if (dentroDosLimites(t, linha, coluna) && visited[linha][coluna] == 0)
            {
                COORDENADA pos = malloc(sizeof(struct Coordenada));
                pos->l = linha;
                pos->c = coluna;
                vizinhos[numeroVizinhosValidos] = pos;
                visited[linha][coluna] = 1; // Tenho que marcar aqui para que nas proximas iterações ja se saiba que esta já esta na queue não voltar a entrar
                numeroVizinhosValidos++;
            }
        }
        insertQueue(q, vizinhos, numeroVizinhosValidos); // já trata do caso de não ter qualquer elemento
        free(atual);
        free(vizinhos);
    }
    if (*count == t->c * t->l)
        return 1;
    return 0;
}

bool trataCaminhoOrtogonal(TABELA t, int *contaRestricoes, bool *temRestrições)
{
    CQUEUE queue = malloc(sizeof(struct CQueue));
    initQueue(queue);

    COORDENADA inicial = devolvePrimeroNRiscado(t);
    if (inicial == NULL)
    {
        free(queue->list);
        free(queue);
        return true;
    }
    int linhaI = inicial->l;
    int colunaI = inicial->c;

    COORDENADA *primeiro = malloc(sizeof(COORDENADA));
    primeiro[0] = inicial;
    insertQueue(queue, primeiro, 1);

    int count = 0;
    int **visited = initVisited(t, &count);
    if (!existeCaminhoOrtogonal(queue, t, visited, &count))
    {
        *temRestrições = true;
        printf("\n---- Restrição nº %d ----\n", (*contaRestricoes)++ + 1);
        printf("Não existe um caminho ortogonal entre todas as casas que não estão riscadas\n");
        int length = t->c * t->l - count;
        COORDENADA *naoVisitados = devolveNaoVisitados(visited, t->l, t->c, length);
        printf("Por exemplo, começando da casa (%c%d) não existe um caminho ortogonal que passe nas casas:\n", colunaI + 'a', linhaI + 1);
        for (int i = 0; i < length; i++)
        {
            printf("  - Coluna: %c, Linha: %d\n", naoVisitados[i]->c + 'a', naoVisitados[i]->l + 1);
            free(naoVisitados[i]);
        }
        free(naoVisitados);
    }

    free(primeiro);
    for (int i = 0; i < t->l; i++)
        free(visited[i]);
    free(visited);

    free(queue->list);
    free(queue);

    return true;
}