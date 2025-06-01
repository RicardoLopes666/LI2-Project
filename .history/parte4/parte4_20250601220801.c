#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../tipos.h"
#include "../parte2/parte2.h" // Inclui a declaração de copiarTabela e dentroDosLimites
#include "../parte1/parte1.h"
#include "../parte3/parte3.h"
#include "../colors.h"

// Função que deteta as casas que se riscadas impediam um caminho ortogonal entre casas e nesse caso pinta-as
void verificaSeQuebraCaminho(TABELA aux, int l, int c, bool *changed, bool escreve)
{
    char caracter = aux->tabela[l][c];
    aux->tabela[l][c] = '#';
    int contaRestricoes = 0;
    trataCaminhoOrtogonal(aux, &contaRestricoes, changed, false);
    if (contaRestricoes > 0)
    {
        aux->tabela[l][c] = toupper(caracter);
        if (escreve)
            printf("A pintar a célula %c%d de branco por isolar as restantes casas ao ser riscada.\n", c + 'a', l + 1);
        *changed = true;
    }
    else
        aux->tabela[l][c] = caracter;
}

// ---- Codigo para o comando 'a' ----
// Função auxiliar para riscar letras repetidas na mesma linha ou coluna
void riscaLetrasRepetidas(TABELA t, TABELA aux, int i, int j, bool escreve, bool *changed)
{
    char letraMinuscula = tolower(t->tabela[i][j]);

    // Verifica a linha
    for (int k = 0; k < t->c; k++)
    {
        if (k != j && t->tabela[i][k] == letraMinuscula)
        {
            if (escreve)
                printf("A riscar a célula %c%d com letra '%c' devido à repetição na linha.\n", 'a' + k, i + 1, t->tabela[i][k]);
            aux->tabela[i][k] = '#';
            *changed = true;
        }
    }

    // Verifica a coluna
    for (int k = 0; k < t->l; k++)
    {
        if (k != i && t->tabela[k][j] == letraMinuscula)
        {
            if (escreve)
                printf("A riscar célula %c%d com letra '%c' devido à repetição na coluna.\n", 'a' + j, k + 1, t->tabela[k][j]);
            aux->tabela[k][j] = '#';
            *changed = true;
        }
    }
}

// Função auxiliar para pintar vizinhos de casas riscadas
void pintaVizinhosDeRiscadas(TABELA t, TABELA aux, int i, int j, bool escreve, bool *changed)
{
    int direcoes[4][2] = {
        {-1, 0}, // Cima
        {1, 0},  // Baixo
        {0, -1}, // Esquerda
        {0, 1}   // Direita
    };

    for (int d = 0; d < 4; d++)
    {
        int novaLinha = i + direcoes[d][0];
        int novaColuna = j + direcoes[d][1];

        if (dentroDosLimites(t, novaLinha, novaColuna) && islower(t->tabela[novaLinha][novaColuna]))
        {
            if (escreve)
                printf("A pintar a célula %c%d de branco devido à casa riscada em %c%d.\n",
                       'a' + novaColuna, novaLinha + 1, 'a' + j, i + 1);
            aux->tabela[novaLinha][novaColuna] = toupper(t->tabela[novaLinha][novaColuna]);
            *changed = true;
        }
    }
}

// Função principal 'a'
TABELA ajuda(TABELA t, bool escreve, bool *changed)
{
    TABELA aux = copiarTabela(t);

    for (int i = 0; i < t->l; i++)
    {
        for (int j = 0; j < t->c; j++)
        {
            if (isupper(t->tabela[i][j]))
            {
                riscaLetrasRepetidas(t, aux, i, j, escreve, changed);
            }

            if (t->tabela[i][j] == '#')
            {
                pintaVizinhosDeRiscadas(t, aux, i, j, escreve, changed);
            }

            if (islower(t->tabela[i][j]))
            {
                verificaSeQuebraCaminho(aux, i, j, changed, escreve);
            }
        }
    }

    return aux;
}

// Código do comando A
// Aplica o comando "a" até não haver alterações. Retorna > 0 caso haja mundanças e 0 caso não haja
int aplicaA(TABELA *aux)
{
    int mudancas = 0;
    bool changed = true;
    while (changed)
    {
        changed = false;
        TABELA temp = *aux;
        *aux = ajuda(temp, false, &changed);
        if (changed == true)
            mudancas++;
        freeTabela(temp);
    }
    return mudancas;
}

// Função responsável por executar o comando a ate não haver alterações no tabuleiro (comando 'A')
void comandoA(TABELA *aux, bool *continuar)
{
    int mudou = aplicaA(aux);
    if (!mudou)
    {
        printf("O tabuleiro não sofreu alterações.\n");
        *continuar = false; // Coloca -se a falso para não se fazer uma cópia do tabuleiro neste caso
    }
    else
        printf("%sTabuleiro alterado.%s\n", GREEN, RESET);
}

// --- Codigo para o comando R ---

// Função que detatas na linhas os grupos do tipo XX_ X e risca o que esta isolado
void trataAA_A_NasLinhas(TABELA aux)
{
    for (int i = 0; i < aux->l; i++)
    {
        for (int j = 0; j < aux->c - 1; j++)
        {
            if (tolower(aux->tabela[i][j]) == tolower(aux->tabela[i][j + 1]))
            {
                for (int k = 0; k < aux->c; k++)
                {
                    if (k != j && k != j + 1 && tolower(aux->tabela[i][j]) == tolower(aux->tabela[i][k]))
                    {
                        aux->tabela[i][k] = '#';
                    }
                }
            }
        }
    }
}

// Função que detatas na colunas os grupos do tipo XX_ X e risca o que esta isolado
void trataAA_A_NasColunas(TABELA aux)
{
    for (int j = 0; j < aux->c; j++)
    {
        for (int i = 0; i < aux->l - 1; i++)
        {
            if (tolower(aux->tabela[i][j]) == tolower(aux->tabela[i + 1][j]))
            {
                for (int k = 0; k < aux->l; k++)
                {
                    if (k != i && k != i + 1 && tolower(aux->tabela[i][j]) == tolower(aux->tabela[k][j]))
                    {
                        aux->tabela[k][j] = '#';
                    }
                }
            }
        }
    }
}

// Função que identifica grupos do tipo xx .... x e trata de riscar o x isolado
void risca_AA_A(TABELA aux)
{
    trataAA_A_NasColunas(aux);
    trataAA_A_NasLinhas(aux);
}

// Detetar grupos ABA nas linhas
void trataABA_linhas(TABELA aux)
{
    for (int i = 0; i < aux->l; i++)
    {
        for (int j = 0; j < aux->c - 2; j++)
        {
            if (aux->tabela[i][j] != '#' && aux->tabela[i][j] == aux->tabela[i][j + 2] && aux->tabela[i][j + 1] != '#')
            {
                // Pintar de branco o elemento do meio
                aux->tabela[i][j + 1] = toupper(aux->tabela[i][j + 1]);
            }
        }
    }
}

// Detetar grupos ABA nas colunas
void trataABA_colunas(TABELA aux)
{
    for (int j = 0; j < aux->c; j++)
    {
        for (int i = 0; i < aux->l - 2; i++)
        {
            if (aux->tabela[i][j] != '#' && aux->tabela[i][j] == aux->tabela[i + 2][j] && aux->tabela[i + 1][j] != '#')
            {
                // Pintar de branco o elemento do meio
                aux->tabela[i + 1][j] = toupper(aux->tabela[i + 1][j]);
            }
        }
    }
}

// Detetar grupos ABA
void riscaABA(TABELA aux)
{
    trataABA_colunas(aux);
    trataABA_linhas(aux);
}

// FUnção que verifica se exsitem letras minusculas numa tabela
int existemMinusculas(TABELA t)
{
    for (int i = 0; i < t->l; i++)
    {
        for (int j = 0; j < t->c; j++)
            if (islower(t->tabela[i][j]))
                return 1;
    }
    return 0;
}

// Verificar se o jogo está completo (sem minúsculas e sem erros)
int jogoResolvido(TABELA aux)
{
    // verificar se existem minúsculas ou regras violadas
    if (existemMinusculas(aux) || verificaRestricoes(aux, false))
        return 0;
    return 1;
}

bool encontraSolucao(TABELA tab, int l, int c)
{
    if (l == tab->l)
    {
        return true;
    }
    if (isupper(tab->tabela[l][c]) || tab->tabela[l][c] == '#')
    {
        return (encontraSolucao(tab, l, c + 1));
    }
    else if (c == tab->c)
    {
        return (encontraSolucao(tab, l + 1, 0));
    }
    else if (!('a' <= tab->tabela[l][c] && tab->tabela[l][c] <= 'z'))
    {
        return (encontraSolucao(tab, l, c + 1));
    }
    else
    {
        tab->tabela[l][c] = toupper(tab->tabela[l][c]);
        char original = tab->tabela[l][c];
        if (!verificaRestricoes(tab, false))
        {
            char original = tab->tabela[l][c];
            if (encontraSolucao(tab, l, c + 1))
            {
                return true;
            }
            tab->tabela[l][c] = original;
        }
        tab->tabela[l][c] = '#';
        if (!verificaRestricoes(tab, false))
        {
            char original = tab->tabela[l][c];
            if (encontraSolucao(tab, l, c + 1))
            {
                return true;
            }
            tab->tabela[l][c] = original;
        }
        return false;
    }
}

// Função que recebe a tabela inicial de quando o jogo foi carregado e tenta resolver o jogo -> Comando R
// Coloca ainda o aux a null null e o continuar a false caso não seja possivel resolver o tabuleiro
TABELA resolve(TABELA t)
{
    TABELA aux = copiarTabela(t);

    // Funções que riscam as casas que se tem a certeza que tem de ser riscadas
    riscaABA(aux);
    risca_AA_A(aux);

    // Aplica-se o comando 'a' repetidamente até ele não fazer mais alterações
    aplicaA(&aux);

    if (!jogoResolvido(aux) && !encontraSolucao(aux, 0, 0))
    {
        freeTabela(aux);
        return NULL;
    }
    return aux;
}
