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

    CU_add_test(suite1, "test_pintarBranco", test_pintarBranco);
    CU_add_test(suite1, "test_riscar", test_riscar);
    CU_add_test(suite1, "test_mostrarTabela", test_mostrarTabela);
    CU_add_test(suite1, "test_coordenadaParaIndice", test_coordenadaParaIndice);
    CU_add_test(suite1, "test_sair", test_sair);
    CU_add_test(suite1, "test_gravar", test_gravar);
    CU_add_test(suite1, "test_lerCmd_fileNotFound", test_lerCmd_fileNotFound);
    CU_add_test(suite1, "test_lerCmd_freeOldTab", test_lerCmd_freeOldTab);
    CU_add_test(suite1, "test_lerCmd_emptyFile", test_lerCmd_emptyFile);
    CU_add_test(suite1, "test_lerCmd_incompleteData", test_lerCmd_incompleteData);
    CU_add_test(suite1, "test_colocaSolucao", test_colocaSolucao);
    CU_add_test(suite1, "test_i", test_i);
    CU_add_test(suite1, "test_e_correctMatches", test_e_correctMatches);

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

    CU_pSuite suite3 = CU_add_suite("Testes Parte 3", NULL, NULL);

    CU_add_test(suite3, "test_devolvePrimeiroNRiscado_tudo_riscado", test_devolvePrimeiroNRiscado_tudo_riscado);
    CU_add_test(suite3, "test_devolvePrimeiroNRiscado_encontra", test_devolvePrimeiroNRiscado_encontra);
    CU_add_test(suite3, "test_initVisited", test_initVisited);
    CU_add_test(suite3, "test_queue_insert_delete", test_queue_insert_delete);
    CU_add_test(suite3, "test_devolveNaoVisitados", test_devolveNaoVisitados);
    CU_add_test(suite3, "test_existeCaminhoOrtogonal_simples", test_existeCaminhoOrtogonal_simples);
    CU_add_test(suite3, "test_trataCaminhoOrtogonal_erro", test_trataCaminhoOrtogonal_erro);
    CU_add_test(suite3, "test_trataCaminhoOrtogonal_ok", test_trataCaminhoOrtogonal_ok);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    // Testes parte 4

    CU_initialize_registry();

    CU_pSuite suite4 = CU_add_suite("Testes Parte 4", NULL, NULL);

    CU_add_test(suite4, "test_verificaSeQuebraCaminho", test_verificaSeQuebraCaminho);
    CU_add_test(suite4, "test_riscaLetrasRepetidas", test_riscaLetrasRepetidas);
    CU_add_test(suite4, "test_pintaVizinhosDeRiscadas", test_pintaVizinhosDeRiscadas);
    CU_add_test(suite4, "test_trataABA_linhas", test_trataABA_linhas);
    CU_add_test(suite4, "test_trataABA_colunas", test_trataABA_colunas);
    CU_add_test(suite4, "test_trataAA_A_NasLinhas", test_trataAA_A_NasLinhas);
    CU_add_test(suite4, "test_trataAA_A_NasColunas", test_trataAA_A_NasColunas);
    CU_add_test(suite4, "test_riscaABA", test_riscaABA);
    CU_add_test(suite4, "test_existemMinusculas_and_jogoResolvido", test_existemMinusculas_and_jogoResolvido);
    CU_add_test(suite4, "test_aplicaA", test_aplicaA);
    CU_add_test(suite4, "test_comandoA", test_comandoA);
    CU_add_test(suite4, "test_tentaRiscarColunas_resolve", test_tentaRiscarColunas_resolve);
    CU_add_test(suite4, "test_tentaRiscarColunas_restricao", test_tentaRiscarColunas_restricao);
    CU_add_test(suite4, "test_tentaColunas", test_tentaColunas);
    CU_add_test(suite4, "test_tentaRiscarLinhas_resolve", test_tentaRiscarLinhas_resolve);
    CU_add_test(suite4, "test_tentaRiscarLinhas_restricao", test_tentaRiscarLinhas_restricao);
    CU_add_test(suite4, "test_tentaLinhas", test_tentaLinhas);
    CU_add_test(suite4, "test_resolve_null", test_resolve_null);
    CU_add_test(suite4, "test_resolve_success", test_resolve_success);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}