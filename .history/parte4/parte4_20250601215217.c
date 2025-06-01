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

// Função que tenta pintar para as duas casas que tem iguais e ve se em alguns dos casos se obtem o jogo resolvido ou ent alguma restrição
void tentaRiscarColunas(int l, int c1, int c2, TABELA *t, bool *continuar)
{

    int cs[2] = {c1, c2};
    int keepGoing = 1; // variavel utilizada para parar o loop em caso de o tabuleiro estar resolvido
    for (int i = 0; keepGoing && i < 2; i++)
    {
        TABELA tentativa = copiarTabela(*t);
        int linha = l;
        int coluna = cs[i];
        char letra = (*t)->tabela[linha][coluna];
        tentativa->tabela[linha][coluna] = toupper(letra);
        aplicaA(&tentativa);
        if (jogoResolvido(tentativa))
        {
            *continuar = false;
            keepGoing = 0;
            TABELA temp = *t;
            *t = copiarTabela(tentativa);
            freeTabela(temp);
        }
        else if (verificaRestricoes(tentativa, false)) // Caso assim existam restrições eu mudo ja na tabela original porque sei que tem de ser da outra forma
        {
            keepGoing = 0;
            (*t)->tabela[linha][coluna] = '#';
            (*t)->tabela[linha][i == 0 ? c2 : c1] = toupper(letra);
        }
        freeTabela(tentativa);
    }
}

// Função auxiliar que tenta riscar e verifica se o jogo fica resolvido
void tentaColunasAux(int i, int j, int k, TABELA *t, bool *continuar)
{
    tentaRiscarColunas(i, j, k, t, continuar);
    if (*continuar)
        aplicaA(t);
    if (jogoResolvido(*t))
        *continuar = false;
}

// Função que vai a cada linha e verifica elementos iguas minusculos e tenta pintar um deles para ver se se consegue chegar à resposta
// Devolve true se a resposta for encontrada e false caso contrario
bool tentaColunas(TABELA *t)
{
    bool continuar = true;
    for (int i = 0; continuar && i < (*t)->l; i++)
    {
        for (int j = 0; continuar && j < (*t)->c; j++)
        {
            if (islower((*t)->tabela[i][j]))
            {
                for (int k = 0; continuar && k < (*t)->c; k++)
                {
                    if (k != j && (*t)->tabela[i][k] == (*t)->tabela[i][j])
                        tentaColunasAux(i, j, k, t, &continuar);
                }
            }
        }
    }
    return (!continuar);
}

// Função que tenta pintar para as duas casas que tem iguais e ve se em alguns dos casos se obtem o jogo resolvido ou ent alguma restrição
void tentaRiscarLinhas(int c, int l1, int l2, TABELA *t, bool *continuar)
{

    int ls[2] = {l1, l2};
    int keepGoing = 1;
    for (int i = 0; keepGoing && i < 2; i++)
    {
        TABELA tentativa = copiarTabela(*t);
        int linha = ls[i];
        int coluna = c;
        char letra = (*t)->tabela[linha][coluna];
        tentativa->tabela[linha][coluna] = toupper(letra);
        aplicaA(&tentativa);
        if (jogoResolvido(tentativa))
        {
            *continuar = false;
            keepGoing = 0;
            TABELA temp = *t;
            *t = copiarTabela(tentativa);
            freeTabela(temp);
        }
        else if (verificaRestricoes(tentativa, false)) // Caso assim existam restrições eu mudo ja na tabela original porque sei que tem de ser da outra forma
        {
            keepGoing = 0;
            (*t)->tabela[linha][coluna] = '#';
            (*t)->tabela[i == 0 ? l2 : l1][coluna] = toupper(letra); // uso letra porque ambas as posições tem a mesma letra minuscula la
        }
        freeTabela(tentativa);
    }
}

// Função auxiliar que tenta riscar e verifica se o jogo fica resolvido
void tentaLinhasAux(int i, int j, int k, TABELA *t, bool *continuar)
{
    tentaRiscarLinhas(i, j, k, t, continuar);
    if (*continuar)
        aplicaA(t);
    if (jogoResolvido(*t))
        *continuar = false;
}

// Função que vai a cada coluna e verifica elementos iguas minusculos e tenta pintar um deles para ver se se consegue chegar à resposta
// Devolve true se a resposta for encontrada e false caso contrario
bool tentaLinhas(TABELA *t)
{
    bool continuar = true;
    for (int i = 0; continuar && i < (*t)->l; i++)
    {
        for (int j = 0; continuar && j < (*t)->c; j++)
        {
            if (islower((*t)->tabela[j][i]))
            {
                for (int k = 0; continuar && k < (*t)->l; k++)
                {
                    if (k != j && (*t)->tabela[k][i] == (*t)->tabela[j][i])
                    {
                        tentaLinhasAux(i, j, k, t, &continuar);
                    }
                }
            }
        }
    }
    return (!continuar);
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
    if (!jogoResolvido(aux) && !tentaColunas(&aux) && !tentaLinhas(&aux))
    {
        freeTabela(aux);
        return NULL;
    }
    return aux;
}

bool solve(TABELA tabAtual, int l, int c)
{
    if (l == tabAtual->l)
    {
        return true;
    }
    else if (c == tabAtual->c)
    {
        return (solve(tabAtual, l + 1, 0));
    }
    else if (!('a' <= tabAtual->tabela[l][c] && tabAtual->tabela[l][c] <= 'z'))
    {
        return (solve(tabAtual, l, c + 1));
    }
    else
    {

        if (!verificaRestricoes(tabAtual, false))
        {
            char original = tabAtual->tabela[l][c];
            tabAtual->tabela[l][c] = toupper(tabAtual->tabela[l][c]);
            if (solve(tabAtual, l, c + 1))
            {
                return true;
            }
            tabAtual->tabela[l][c] = original;
        }
        if (ser_valido(tabAtual, l, c, '#'))
        {
            char original = tabAtual->grelha[l][c];
            tabAtual->grelha[l][c] = '#';
            if (solve(tabAtual, l, c + 1, modo))
            {
                return true;
            }
            tabAtual->grelha[l][c] = original;
        }
        return false;
    }
}

bool comando_R(Tabuleiro *tabAtual, Historico *hist, bool modo)
{
    Tabuleiro tabOriginal = copiar_tabuleiro(&hist->estados[0]);

    Historico histTemp;
    inicializar_historico(&histTemp);

    tecnicas_iniciais(&tabOriginal, &histTemp);
    comando_A(&tabOriginal, &histTemp);
    bool r = solve(&tabOriginal, 0, 0, modo);

    copiar_tabuleiro_para(&tabOriginal, tabAtual);

    return r;
}