#include <stdio.h>
#include <stdlib.h>
#include "../tipos.h"
#include "../parte2/parte2.h"
#include "../parte1/parte1.h"
#include <ctype.h>
#include "../colors.h"

// Função que percorre a tabela por ordem de linhas e colunas até encontrar o primeiro elemento que não estiver riscado.
// Esse elemento será utilizado para o BFS e devolvido como uma coordenada.
COORDENADA devolvePrimeiroNRiscado(TABELA t)
{
    int i = 0;
    while (i < (t->c * t->l) && t->tabela[i / t->c][i % t->c] == '#') // i / t->c: Calcula a linha correspondente ao índice i.
                                                                      // i % t->c: Calcula a coluna correspondente ao índice i
        i++;
    if (i >= t->c * t->l) // Se todas as casas estão riscadas, retorna NULL
        return NULL;

    // Aloca memória para a coordenada e define os valores de linha e coluna
    COORDENADA pos = malloc(sizeof(struct Coordenada));
    pos->l = i / t->c;
    pos->c = i % t->c;
    return pos;
}

// Função que inicializa o array de visitados com 0, exceto nas casas riscadas (0 -> não visitado, 1 -> visitado).
// Também atualiza o contador `count` com o número de casas já visitadas.
int **initVisited(TABELA t, int *count)
{
    int **visited = malloc(sizeof(int *) * t->l); // Aloca memória para as linhas
    for (int i = 0; i < t->l; i++)
    {
        visited[i] = malloc(sizeof(int) * t->c); // Aloca memória para as colunas
        for (int j = 0; j < t->c; j++)
        {
            if (t->tabela[i][j] == '#') // Marca casas riscadas como visitadas
            {
                visited[i][j] = 1;
                (*count)++; // Atualiza o número de casas visitadas
            }
            else
                visited[i][j] = 0; // Marca casas não riscadas como não visitadas
        }
    }
    return visited;
}

// Inicializa a fila (queue) com capacidade inicial de 3 elementos.
void initQueue(CQUEUE q)
{
    q->capacidade = 3;
    q->tamanho = 0;
    q->front = 0;
    q->list = malloc(sizeof(COORDENADA) * q->capacidade); // Aloca memória para a lista de coordenadas
}

// Função que efetivamente coloca os elementos novos na queue
void enqueue(CQUEUE q, COORDENADA *coordenadas, int coordSize)
{
    // Insere os novos elementos na fila
    for (int i = 0; i < coordSize; i++)
    {
        q->list[(q->front + q->tamanho) % q->capacidade] = coordenadas[i];
        q->tamanho++;
    }
}

// Função responsável por inserir um array de coordenadas na fila.
// Expande a capacidade da fila, se necessário.
int insertQueue(CQUEUE q, COORDENADA *coordenadas, int coordSize)
{
    if (coordSize < 1) // Não há elementos para inserir
        return 1;

    int tam = q->tamanho + coordSize;
    if (tam >= q->capacidade) // Expande a capacidade da fila, se necessário
    {
        int oldCapacity = q->capacidade;
        while (tam >= q->capacidade)
            q->capacidade = q->capacidade == 0 ? 1 : q->capacidade * 2; // Dobra a capacidade até ter espaço suficiente

        COORDENADA *temp = realloc(q->list, sizeof(COORDENADA) * q->capacidade);
        if (temp == NULL) // Falha ao realocar memória
        {
            return 0;
        }
        q->list = temp;

        // Ajusta os elementos da fila para manter a ordem correta
        int end = q->front + q->tamanho;
        if (end > oldCapacity)
            for (int i = 0; i < end % oldCapacity; i++)
                q->list[i + oldCapacity] = q->list[i];
    }

    enqueue(q, coordenadas, coordSize);

    return 1;
}

// Função que remove o elemento da frente da fila.
// O elemento removido é retornado por meio do ponteiro `pos`.
void deleteQueue(CQUEUE q, COORDENADA *pos)
{
    if (q->tamanho == 0) // Fila vazia
        return;

    // Remove o elemento da frente
    COORDENADA temp = q->list[q->front];
    *pos = temp;
    q->list[q->front] = NULL;                  // Marca a posição como vazia
    q->front = (q->front + 1) % q->capacidade; // Atualiza o índice do início da fila
    q->tamanho--;
}

// Função que, dado um array de visitados, devolve um array com as posições que não foram visitadas.
COORDENADA *devolveNaoVisitados(int **visited, int l, int c, int length)
{
    COORDENADA *naoVisitados = malloc(sizeof(COORDENADA) * length); // Aloca memória para o array de não visitados
    int cont = 0;
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (visited[i][j] == 0) // Casa não visitada
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

void atualizaEstado(int linha, int coluna, int **visited, COORDENADA *vizinhos, int *numeroVizinhosValidos)
{
    COORDENADA pos = malloc(sizeof(struct Coordenada));
    pos->l = linha;
    pos->c = coluna;
    vizinhos[*numeroVizinhosValidos] = pos;
    visited[linha][coluna] = 1; // Marca o vizinho como visitado
    (*numeroVizinhosValidos)++;
}

// Algoritmo de busca em largura (BFS) para verificar se existe um caminho ortogonal entre todas as casas não riscadas.
int existeCaminhoOrtogonal(CQUEUE q, TABELA t, int **visited, int *count)
{
    int direcoes[4][2] = {
        {-1, 0}, // Cima
        {1, 0},  // Baixo
        {0, -1}, // Esquerda
        {0, 1}   // Direita
    };

    while (q->tamanho > 0) // Enquanto houver elementos na fila
    {
        COORDENADA atual = NULL;
        deleteQueue(q, &atual); // Remove o elemento da frente da fila

        visited[atual->l][atual->c] = 1; // Marca a casa atual como visitada
        (*count)++;

        int numeroVizinhosValidos = 0;
        COORDENADA *vizinhos = malloc(4 * sizeof(COORDENADA)); // No máximo existem 4 vizinhos válidos
        for (int i = 0; i < 4; i++)
        {
            int linha = direcoes[i][0] + atual->l;
            int coluna = direcoes[i][1] + atual->c;

            if (dentroDosLimites(t, linha, coluna) && visited[linha][coluna] == 0) // Verifica se o vizinho é válido
                atualizaEstado(linha, coluna, vizinhos, vizinhos, numeroVizinhosValidos);
        }
        insertQueue(q, vizinhos, numeroVizinhosValidos); // Insere os vizinhos na fila
        free(atual);
        free(vizinhos);
    }

    // Verifica se todas as casas foram visitadas
    if (*count == t->c * t->l)
        return 1;
    return 0;
}

// Função que escreve as restrições caso não exista um caminho ortogonal entre todas as casas não riscadas.
void escreveInexistenciaCaminho(bool *temRestricoes, bool escreve, int *contaRestricoes, TABELA t, int **visited, int colunaI, int linhaI, int count)
{
    *temRestricoes = true;
    if (escreve)
    {
        printf("\n%s%s---- Restrição nº %d ----%s\n", HINT_COLOR, BOLD, *contaRestricoes + 1, RESET);
        printf("Não existe um caminho ortogonal entre todas as casas que não estão riscadas\n");
    }
    (*contaRestricoes)++;
    int length = t->c * t->l - count;
    COORDENADA *naoVisitados = devolveNaoVisitados(visited, t->l, t->c, length);
    if (escreve)
        printf("Por exemplo, começando da casa (%c%d) não existe um caminho ortogonal que passe nas casas:\n", colunaI + 'a', linhaI + 1);
    for (int i = 0; i < length; i++)
    {
        if (escreve)
            printf("  - Coluna: %c, Linha: %d\n", naoVisitados[i]->c + 'a', naoVisitados[i]->l + 1);
        free(naoVisitados[i]);
    }
    free(naoVisitados);
}

// Função que liberta o espaço alocado previamente
void libertaEspaco(COORDENADA *primeiro, int **visited, TABELA t, CQUEUE queue)
{
    free(primeiro);
    for (int i = 0; i < t->l; i++)
        free(visited[i]);
    free(visited);

    free(queue->list);
    free(queue);
}

// Função principal que verifica se existe um caminho ortogonal entre todas as casas não riscadas.
// Caso não exista, escreve as restrições.
bool trataCaminhoOrtogonal(TABELA t, int *contaRestricoes, bool *temRestricoes, bool escreve)
{
    CQUEUE queue = malloc(sizeof(struct CQueue));
    initQueue(queue);

    COORDENADA inicial = devolvePrimeiroNRiscado(t);
    if (inicial == NULL) // Se não há casas não riscadas, retorna true
    {
        free(queue->list);
        free(queue);
        return true;
    }

    int linhaI = inicial->l;
    int colunaI = inicial->c;

    COORDENADA *primeiro = malloc(sizeof(COORDENADA)); // Adiciona a primeira coordenada à fila
    primeiro[0] = inicial;
    insertQueue(queue, primeiro, 1);

    int count = 0;
    int **visited = initVisited(t, &count);
    if (!existeCaminhoOrtogonal(queue, t, visited, &count)) // Verifica se existe um caminho ortogonal
    {
        escreveInexistenciaCaminho(temRestricoes, escreve, contaRestricoes, t, visited, colunaI, linhaI, count);
    }

    libertaEspaco(primeiro, visited, t, queue);
    free(primeiro);
    for (int i = 0; i < t->l; i++)
        free(visited[i]);
    free(visited);

    free(queue->list);
    free(queue);

    return true;
}