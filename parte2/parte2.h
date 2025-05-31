#ifndef PARTE2_H
#define PARTE2_H

#include <stdbool.h>
#include "../tipos.h" /* Para as definições de TABELA, STACKTABS e GAME */

/* ========== Protótipos das funções definidas em parte2.c ========== */

/* _________ Funções utilizadas para voltar atrás -> comando 'd' __________ */

/**
 * Inicializa uma stack de tabelas.
 * - s: ponteiro para STACKTABS já alocado (não deve ser NULL).
 * - Define capacidade=3, indice=0, comprimento=0 e aloca espaço para 3 TABELA.
 * - Se malloc falhar, devolve false; caso contrário, true.
 */
bool initStackTabs(STACKTABS s);

/**
 * Insere uma nova tabela (tab) na stack s.
 * Regras:
 *  - Se indice < comprimento, apaga/free de todas as tabelas a partir de indice até comprimento-1,
 *    e define comprimento = indice.
 *  - Caso contrário, se comprimento >= capacidade, duplica a capacidade via realloc.
 *  - Acrescenta tab em s->tabelas[s->comprimento], incrementa comprimento e indice.
 *  - Devolve false só se realloc falhar; caso contrário, true.
 */
bool insereTabela(STACKTABS s, TABELA tab);

/**
 * Remove a tabela corrente:
 *  - Se stackTabs->comprimento <= 1 ou stackTabs->indice < 2, devolve false (não apaga).
 *  - Caso contrário, faz freeTabela(game->tab),
 *    depois copia a tabela que está em stackTabs->tabelas[ (indice - 2) ] para game->tab,
 *    decrementa indice em 1 e devolve true.
 */
bool deleteTabela(GAME *game);

/**
 * Faz uma cópia profunda de uma TABELA:
 *  - Aloca um novo struct Tabela e devolve o ponteiro.
 *  - Se falhar malloc em qualquer fase, devolve NULL.
 */
TABELA copiarTabela(TABELA t);

/**
 * Liberta toda a memória associada a uma stack de TABELAs:
 *  - Para cada i de 0 a s->comprimento-1, chama freeTabela(s->tabelas[i]);
 *  - Depois liberta s->tabelas (array) e liberta o próprio ponteiro s.
 */
void freeStackTabs(STACKTABS s);

/**
 * Comando 'u': avança para o próximo tabuleiro na stack
 *  - Se game->stackTabs->indice >= game->stackTabs->comprimento, imprime erro em stderr.
 *  - Caso contrário, faz freeTabela(game->tab) e copia stackTabs->tabelas[indice] para game->tab,
 *    incrementa indice em 1.
 *  - No fim, marca *comandoProcessado = true.
 */
void u(GAME *game, bool *comandoProcessado);

/**
 * Comando 'd': retrocede (undo) para a tabela anterior
 *  - Internamente chama deleteTabela(game). Se deleteTabela devolver false, imprime erro em stderr.
 *  - No fim, marca *comandoProcessado = true.
 */
void d(GAME *game, bool *comandoProcessado);

/* _____ Funções utilizadas para verificar as restrições -> comando 'v' _____ */

/**
 * Verifica se (linha,coluna) está dentro dos limites do tabuleiro t:
 *  - Retorna true se 0 <= linha < t->l e 0 <= coluna < t->c; senão false.
 */
bool dentroDosLimites(TABELA t, int linha, int coluna);

/**
 * Verifica quantas casas riscadas em redor (ortogonalmente) da posição (linha,coluna) são riscadas:
 *  - Se (linha,coluna) não está dentroDosLimites ou t->tabela[linha][coluna] != '#', devolve 0.
 *  - Senão, percorre as 4 direções (cima, baixo, esquerda, direita). Sempre que encontrar '#',
 *    armazena em restricoes[count] = {novaLinha, novaColuna} e incrementa count.
 *  - Devolve count (número de vizinhas riscadas).
 */
int verificaRiscadaVizinhasBrancas(TABELA t, int linha, int coluna, int restricoes[][2]);

/**
 * Verifica se há duas casas brancas iguais (letra maiúscula) na mesma linha ou coluna de (linha,coluna):
 *  - Retorna false se (!dentroDosLimites(t,linha,coluna) || !isupper(t->tabela[linha][coluna])).
 *  - Caso contrário, percorre a linha e coluna, procurando outra ocorrência de t->tabela[linha][coluna].
 *  - Se encontrar, devolve true; senão, false.
 */
bool verificaLetraIgualLinhaColuna(TABELA t, int linha, int coluna);

/**
 * Processa restrições localmente para a casa (i,j):
 *  - Se t->tabela[i][j] == '#', chama verificaRiscadaVizinhasBrancas.
 *    Se numRestricoes>0, marca *temRestricoes=true, imprime detalhes caso escreve==true, e incrementa *contaRestricoes.
 *  - Se isupper(t->tabela[i][j]), chama verificaLetraIgualLinhaColuna. Se true, marca *temRestricoes=true,
 *    imprime detalhes caso escreve==true, e incrementa *contaRestricoes.
 */
void verificaRiscadasERepetidas(TABELA t, int i, int j, int *contaRestricoes, bool *temRestricoes, bool escreve);

/**
 * Percorre todo o tabuleiro t para verificar restrições:
 *  - Inicialmente temRestricoes=false, contaRestricoes=0.
 *  - Para cada célula (i,j), chama verificaRiscadasERepetidas(...).
 *  - Depois chama trataCaminhoOrtogonal(t, &contaRestricoes, &temRestricoes, escreve).
 *    - Se trataCaminhoOrtogonal retornar false, devolve false imediatamente.
 *  - Se no fim temRestricoes == false, imprime “Nenhuma restrição foi violada no tabuleiro.” caso escreve==true.
 *  - Devolve temRestricoes.
 */
bool verificaRestricoes(TABELA t, bool escreve);

/**
 * Comando 'v': verifica restrições no tabuleiro actual:
 *  - Se game.tab == NULL, imprime erro em stderr e *continuar = false.
 *  - Caso contrário, imprime “A verificar as restrições do tabuleiro...”, chama verificaRestricoes(game.tab, true)
 *    e no fim *comandoProcessado = true.
 */
void v(GAME game, bool *continuar, bool *comandoProcessado);

#endif // PARTE2_H
