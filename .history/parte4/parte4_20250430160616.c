#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../tipos.h"
#include "../parte2/parte2.h" // Inclui a declaração de copiarTabela
#include "../parte1/parte1.h"
#include "../parte3/parte3.h"

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
            printf("A pintar a célula %c%d de branco por isolar as restantes casas ao ser riscada .\n", c + 'a', l + 1);
        *changed = true;
    }
    else
        aux->tabela[l][c] = caracter;
}

// ---- Codigo para o comando 'a' ----
TABELA ajuda(TABELA t, bool escreve, bool *changed)
{
    // Cria uma tabela auxiliar como cópia da tabela original
    TABELA aux = copiarTabela(t);

    // Verifica as restrições do tabuleiro original e aplica as mudanças na tabela auxiliar
    for (int i = 0; i < t->l; i++)
    {
        for (int j = 0; j < t->c; j++)
        {
            // Verifica letras iguais na mesma linha ou coluna
            if (isupper(t->tabela[i][j]))
            {
                char letraMaiuscula = t->tabela[i][j];
                char letraMinuscula = tolower(letraMaiuscula);

                // Verifica a linha
                for (int k = 0; k < t->c; k++)
                {
                    if (k != j && (t->tabela[i][k] == letraMaiuscula || t->tabela[i][k] == letraMinuscula))
                    {
                        if (escreve)
                            printf("A riscar a célula %c%d com letra '%c' devido à repetição na linha.\n", 'a' + k, i + 1, t->tabela[i][k]);
                        aux->tabela[i][k] = '#'; // Risca a célula na tabela auxiliar
                        *changed = true;
                    }
                }

                // Verifica a coluna
                for (int k = 0; k < t->l; k++)
                {
                    if (k != i && (t->tabela[k][j] == letraMaiuscula || t->tabela[k][j] == letraMinuscula))
                    {
                        if (escreve)
                            printf("A riscar célula %c%d com letra '%c' devido à repetição na coluna.\n", 'a' + j, k + 1, t->tabela[k][j]);
                        aux->tabela[k][j] = '#'; // Risca a célula na tabela auxiliar
                        *changed = true;
                    }
                }
            }

            // Verifica casas riscadas que não têm todas as vizinhas brancas
            if (t->tabela[i][j] == '#')
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

                    // Verifica se a célula vizinha está dentro dos limites do tabuleiro
                    if (dentroDosLimites(t, novaLinha, novaColuna))
                    {
                        // Verifica se a célula vizinha não é uma casa riscada ('#') ou já branca ('B')
                        if (islower(t->tabela[novaLinha][novaColuna]))
                        {
                            if (escreve)
                                printf("A pintar a célula %c%d de branco devido à casa riscada em %c%d.\n",
                                       'a' + novaColuna, novaLinha + 1, 'a' + j, i + 1);
                            aux->tabela[novaLinha][novaColuna] = toupper(t->tabela[novaLinha][novaColuna]); // Pinta de branco na tabela auxiliar
                            *changed = true;
                        }
                    }
                }
            }

            // verifica se a casa ao ser riscada introduzia a restrição de não existir caminho entre casas ortogonais e nesse caso pinta-a
            if (islower(t->tabela[i][j]))
                verificaSeQuebraCaminho(aux, i, j, changed, escreve);
        }
    }

    return aux; // Retorna a tabela auxiliar com as alterações aplicadas
}

// Código do comando A
// Aplica o comando "a" até não haver alterações
void aplicaA(TABELA *aux)
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        TABELA temp = *aux;
        *aux = ajuda(temp, false, &changed);
        freeTabela(temp);
    }
}

// --- Codigo para o comando R ---

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

// Função que identifica grupos do tipo xx .... x e devolve as coordenadas dos elementos que devem ser riscados  (neste caso o x)
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

void riscaABA(TABELA aux)
{
    trataABA_colunas(aux);
    trataABA_linhas(aux);
}

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
                    {
                        tentaRiscarColunas(i, j, k, t, &continuar);
                        if (continuar)
                            aplicaA(t);
                        if (jogoResolvido(*t))
                            continuar = false;
                    }
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
                        tentaRiscarLinhas(i, j, k, t, &continuar);
                        if (continuar)
                            aplicaA(t);
                        if (jogoResolvido(*t))
                            continuar = false;
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
    if (jogoResolvido(aux) || tentaColunas(&aux) || tentaLinhas(&aux))
    {
        printf("Tabuleiro resolvido.\n");
    }
    else
    {

        printf("Tabuleiro não pode ser resolvido.\n");
        freeTabela(aux);
        return NULL;
    }
    return aux;
}