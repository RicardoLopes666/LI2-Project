#ifndef PARTE4_H
#define PARTE4_H

#include <stdbool.h>
#include "../tipos.h"
#include "../parte2/parte2.h" // Inclui a declaração de copiarTabela e dentroDosLimites
#include "../parte1/parte1.h"
#include "../parte3/parte3.h"

// Função que deteta as casas que se riscadas impediam um caminho ortogonal entre casas e nesse caso pinta-as
void verificaSeQuebraCaminho(TABELA aux, int l, int c, bool *changed, bool escreve);

// Função auxiliar para riscar letras repetidas na mesma linha ou coluna
void riscaLetrasRepetidas(TABELA t, TABELA aux, int i, int j, bool escreve, bool *changed);

// Função auxiliar para pintar vizinhos de casas riscadas
void pintaVizinhosDeRiscadas(TABELA t, TABELA aux, int i, int j, bool escreve, bool *changed);

// Função principal 'a'
TABELA ajuda(TABELA t, bool escreve, bool *changed);

// Aplica o comando 'a' até não haver alterações. Retorna > 0 caso haja mudanças e 0 caso não haja
int aplicaA(TABELA *aux);

// Executa o comando 'a' no tabuleiro
void comandoA(TABELA *aux, bool *continuar);

// Função que deteta na linha os grupos do tipo XX_ X e risca o que está isolado
void trataAA_A_NasLinhas(TABELA aux);

// Função que deteta na coluna os grupos do tipo XX_ X e risca o que está isolado
void trataAA_A_NasColunas(TABELA aux);

// Função que identifica grupos do tipo xx .... x e trata de riscar o x isolado
void risca_AA_A(TABELA aux);

// Detetar grupos ABA nas linhas
void trataABA_linhas(TABELA aux);

// Detetar grupos ABA nas colunas
void trataABA_colunas(TABELA aux);

// Detetar grupos ABA
void riscaABA(TABELA aux);

// Verifica se existem minúsculas no tabuleiro
int existemMinusculas(TABELA t);

// Verifica se o jogo está completo (sem minúsculas e sem erros)
int jogoResolvido(TABELA aux);

bool encontraSolucao(TABELA tab, int l, int c);

// Função que recebe a tabela inicial de quando o jogo foi carregado e tenta resolver o jogo -> Comando R
// Coloca ainda o aux a null e o continuar a false caso não seja possível resolver o tabuleiro
TABELA resolve(TABELA t);

#endif /* PARTE4_H */
