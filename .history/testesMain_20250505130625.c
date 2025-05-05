#include <stdio.h>
#include <CUnit/Basic.h>
#include "parte1/testesparte1.h"
#include "parte2/testesparte2.h"
#include "parte3/testesparte3.h"
#include "parte4/testesparte4.h"
#include <ctype.h>
#include "parte1/parte1.h"
#include "parte2/parte2.h"
#include "parte3/parte3.h"
#include "parte4/parte4.h"

int main()
{
    // Testes parte 1
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite1 = CU_add_suite("Testes da parte 1", NULL, NULL);
    if (NULL == suite1)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(suite1, "Teste pintarBranco", test_pintarBranco)) ||
        (NULL == CU_add_test(suite1, "Teste riscar", test_riscar)) ||
        (NULL == CU_add_test(suite1, "Teste mostrarTabela", test_mostrarTabela)) ||
        (NULL == CU_add_test(suite1, "Teste coordenadaParaIndice", test_coordenadaParaIndice)) ||
        (NULL == CU_add_test(suite1, "Teste sair", test_sair)) ||
        (NULL == CU_add_test(suite1, "Teste gravar", test_gravar)) ||
        (NULL == CU_add_test(suite1, "Teste lerCmd", test_lerCmd)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    // Testes parte 2

    CU_initialize_registry();

    CU_pSuite suite2 = CU_add_suite("Testes da parte 2", NULL, NULL);
    if (NULL == suite2)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite2, "Teste initStackTabs", test_initStackTabs) == NULL ||
        CU_add_test(suite2, "Teste insereTabela", test_insereTabela) == NULL ||
        CU_add_test(suite2, "Teste deleteTabela", test_deleteTabela) == NULL ||
        CU_add_test(suite2, "Teste copiarTabela", test_copiarTabela) == NULL ||
        CU_add_test(suite2, "Teste freeStackTabs", test_freeStackTabs) == NULL ||
        CU_add_test(suite2, "Teste dentroDosLimites", test_dentroDosLimites) == NULL ||
        CU_add_test(suite2, "Teste verificaRiscadaVizinhasBrancas", test_verificaRiscadaVizinhasBrancas) == NULL ||
        CU_add_test(suite2, "Teste verificaRestrições", test_verificaRestricoes) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    // Testes parte 3

    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Testes Parte 3", NULL, NULL);

    CU_add_test(suite, "Teste devolvePrimeroNRiscado", test_devolvePrimeroNRiscado);
    CU_add_test(suite, "Teste initVisited", test_initVisited);
    CU_add_test(suite, "Teste insertQueue", test_insertQueue);
    CU_add_test(suite, "Teste deleteQueue", test_deleteQueue);
    CU_add_test(suite, "Teste existeCaminhoOrtogonal", test_existeCaminhoOrtogonal);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    // Testes parte 4

    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Testes Parte 4", NULL, NULL);

    CU_add_test(suite, "verificaSeQuebraCaminho", test_verificaSeQuebraCaminho);
    CU_add_test(suite, "riscaLetrasRepetidas", test_riscaLetrasRepetidas);
    CU_add_test(suite, "pintaVizinhosDeRiscadas", test_pintaVizinhosDeRiscadas);
    CU_add_test(suite, "aplicaA", test_aplicaA);
    CU_add_test(suite, "comandoA", test_comandoA);
    CU_add_test(suite, "trataAA_A_NasLinhas", test_trataAA_A_NasLinhas);
    CU_add_test(suite, "trataAA_A_NasColunas", test_trataAA_A_NasColunas);
    CU_add_test(suite, "riscaABA", test_riscaABA);
    CU_add_test(suite, "jogoResolvido", test_jogoResolvido);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}