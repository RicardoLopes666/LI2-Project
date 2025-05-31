#ifndef PARTE4_H
#define PARTE4_H

/*
 * Verifica se o comando de “riscar” (por exemplo, um caminho de letras) irá “quebrar”
 * o tabuleiro (i.e., se passa por células que não podem ser riscadas ou invalida
 * alguma restrição). Deve devolver true se “quebra” (inválido) ou false caso contrário.
 */
bool verificaSeQuebraCaminho(GAME *g, COORDENADA inicio, COORDENADA fim);

/*
 * Percorre toda a tabela e “risca” (marca como riscadas) todas as letras que
 * se repetem de acordo com as regras do jogo (ou seja, letras já “consumidas”
 * que não podem mais aparecer). Assume-se que o GAME já tem a tabela inicializada.
 */
void riscaLetrasRepetidas(GAME *g);

/*
 * Para cada célula já marcada como “riscada”, pinta (marca) todas as suas
 * células vizinhas (ortogonalmente) de acordo com a regra do jogo (ex.: bloqueia,
 * ou assinala de alguma forma). Normalmente, este passo impede que caminhos
 * futuros passem por essas vizinhas.
 */
void pintaVizinhosDeRiscadas(GAME *g);

/*
 * Função auxiliar que aplica uma única instrução “A” (linha ou coluna) ao GAME.
 * Por exemplo, percorre uma linha (ou coluna) inteira e executa a ação definida
 * pelo comando “A” (ex.: riscar a primeira célula válida, pintar vizinhos, etc.).
 */
void aplicaA(GAME *g, char tipoA, int indiceLinhaOuColuna);

/*
 * Interpreta e executa o comando ‘A’ vindo diretamente da leitura de input.
 * Por exemplo, se o input for “A 3”, deteta que é um comando ‘A’ para a linha 3
 * e chama internamente `aplicaA(g, 'L', 3)` ou `aplicaA(g, 'C', 3)`, dependendo
 * do formato.
 */
void comandoA(GAME *g, const char *param);

/*
 * Trata comandos do tipo “AA” que se aplicam a uma linha:
 * “AA A” (por exemplo) aplica duas vezes a operação ‘A’ na mesma linha ou
 * executa uma ação extendida definida pelo enunciado (ex.: dois passos de
 * riscar/pintar). Aqui, `indiceLinha` é o número da linha em questão.
 * A função deve fazer todos os efeitos correspondentes às regras.
 */
void trataAA_A_NasLinhas(GAME *g, int indiceLinha);

/*
 * Igual a `trataAA_A_NasLinhas`, mas aplicado a colunas. Ou seja, comando
 * “AA A” para a coluna X, com `indiceColuna = X`.
 */
void trataAA_A_NasColunas(GAME *g, int indiceColuna);

/*
 * Procura padrões do tipo “ABA” em linhas e colunas da tabela do GAME e
 * risca as células correspondentes ao padrão (por exemplo, risca tanto o A
 * inicial quanto o A final, segundo as regras). Deve percorrer todas as linhas
 * e colunas, encontrando cada ocorrência de ABA.
 */
void riscaABA(GAME *g);

/*
 * Verifica se o jogo está resolvido (i.e., não existem mais células passíveis
 * de riscar, todas as restrições atendidas e a solução final atingida). Devolve
 * true se estiver tudo resolvido, ou false caso contrário.
 */
bool jogoResolvido(GAME *g);

#endif // PARTE4_H
