#include <stdio.h>
#include <CUnit/Basic.h>
#include "parte1/testesparte1.h"
#include "parte2/testesparte2.h"
#include <ctype.h>
#include "parte1/parte1.h"
#include "parte2/parte2.h"

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

    // Testes parte 2

    CU_pSuite suite2 = CU_add_suite("Testes da parte 2", 0, 0);
    if (NULL == suite2)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite2, "Teste initStackTabs", test_initStackTabs) ||
        CU_add_test(suite2, "Teste insereTabela", test_insereTabela) ||
        CU_add_test(suite2, "Teste deleteTabela", test_deleteTabela) ||
        CU_add_test(suite2, "Teste copiarTabela", test_copiarTabela) ||
        CU_add_test(suite2, "Teste freeStackTabs", test_freeStackTabs) ||
        CU_add_test(suite2, "Teste dentroDosLimites", test_dentroDosLimites) ||
        CU_add_test(suite2, "Teste verificaRiscadaVizinhasBrancas", test_verificaRiscadaVizinhasBrancas) ||
        CU_add_test(suite2, "Teste verificaRestrições", test_verificaRestricoes))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}