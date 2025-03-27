#include <stdio.h>
#include <time.h>
#include "/usr/include/CUnit/Basic.h"
#include "/usr/include/CUnit/CUnit.h"
#include "strCatMalloc.h"

//Tout les tests possible ont été fait

//test avec 2 chaine NULL
void test_strCatMalloc_2chaineNULL() {
    char * str1 = NULL;
    char * str2 = NULL;

    char * result = strCatMalloc(str1, str2);
    
    //le resultat doit être NULL
    CU_ASSERT_EQUAL(result, NULL);
    free(result);
}

//test avec une chaine NULL en 1er parametre et une chaine valide en second
void test_strCatMalloc_chaineNULLGauche() {
    char * str1 = NULL;
    char * str2 = "test";

    char * result = strCatMalloc(str1, str2);
    
    //le resultat doit être NULL
    CU_ASSERT_EQUAL(result, NULL);
    free(result);
}

//test avec une chaine valide en 1er parametre et une chaine NULL en second
void test_strCatMalloc_chaineNULLDroite() {
    char * str1 = "Test";
    char * str2 = NULL ;

    char * result = strCatMalloc(str1, str2);

    //le resultat doit être NULL
    CU_ASSERT_EQUAL(result, NULL);
    free(result);
}

//test avec 2 chaines valides, le resultat doit être "result"
void test_strCatMalloc_chaineOk() {
    char * str1 = "res";
    char * str2 = "ult";

    char * result = strCatMalloc(str1, str2);

    //le resultat doit être "result"
    int test = strcmp(result, "result");
    free(result);

    CU_ASSERT_EQUAL(test, 0);
}

int main(){
    if (CUE_SUCCESS != CU_initialize_registry())
       return CU_get_error();

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("strCatMallocTestSuite", 0, 0);
    if (pSuite == NULL){
       CU_cleanup_registry();
       return CU_get_error();
    }
    if ((CU_add_test(pSuite, "test avec 2 chaine NULL", test_strCatMalloc_2chaineNULL) == NULL) ||
        (CU_add_test(pSuite, "test avec une chaine null en 1er parametre", test_strCatMalloc_chaineNULLGauche) == NULL) ||
        (CU_add_test(pSuite, "test avec une chaine null en 2e parametre", test_strCatMalloc_chaineNULLDroite) == NULL) ||
        (CU_add_test(pSuite, "test avec 2 chaines ok", test_strCatMalloc_chaineOk) == NULL))
    {
       CU_cleanup_registry();
       return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}