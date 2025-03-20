#include <stdio.h>
#include <time.h>
#include "/usr/include/CUnit/Basic.h"
#include "/usr/include/CUnit/CUnit.h"
#include "goldGainOffline.h"

/* Test lorsque aucun temps ne s'est écoulé : goldEarned doit être zéro */
void test_goldGainOffline_noElapsed() {
    testGold = 0;
    notifCalled = 0;
    notifGold = 0;
    fakeCurrentTime = 1000; // temps actuel
    time_t lastSaveTime = 1000; // même valeur, donc temps écoulé = 0

    goldGainOffline(lastSaveTime);

    CU_ASSERT_EQUAL(testGold, 0);
    CU_ASSERT_EQUAL(notifCalled, 0);
}

/* Test lorsque du temps s'est écoulé : vérifier l'ajout d'or et la notification */
void test_goldGainOffline_positiveElapsed() {
    testGold = 0;
    notifCalled = 0;
    notifGold = 0;
    fakeCurrentTime = 1100; // temps actuel
    time_t lastSaveTime = 1000; // temps écoulé : 100 secondes
    /* goldEarned = temps écoulé * goldBySec = 100 * testGoldRate
       or ajouté = (goldEarned / 10) = (100 * testGoldRate / 10) = 10 * testGoldRate */

    goldGainOffline(lastSaveTime);

    CU_ASSERT_EQUAL(testGold, 10 * testGoldRate);
    CU_ASSERT_EQUAL(notifCalled, 1);
    CU_ASSERT_EQUAL(notifGold, 10 * testGoldRate);
}

/* Test lorsque le temps est négatif : vérifier qu'aucun or n'est ajouté et aucune notification */
void test_goldGainOffline_negativeElapsed() {
    testGold = 0;
    notifCalled = 0;
    notifGold = 0;
    fakeCurrentTime = 900; // temps actuel inférieur au dernier temps de sauvegarde
    time_t lastSaveTime = 1000; // temps futur relatif

    goldGainOffline(lastSaveTime);

    CU_ASSERT_EQUAL(testGold, 0);
    CU_ASSERT_EQUAL(notifCalled, 0);
}

int main(){
    if (CUE_SUCCESS != CU_initialize_registry())
       return CU_get_error();

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("GoldGainOfflineTestSuite", 0, 0);
    if (pSuite == NULL){
       CU_cleanup_registry();
       return CU_get_error();
    }
    if ((CU_add_test(pSuite, "test sans temps écoulé", test_goldGainOffline_noElapsed) == NULL) ||
        (CU_add_test(pSuite, "test avec temps écoulé positif", test_goldGainOffline_positiveElapsed) == NULL) ||
        (CU_add_test(pSuite, "test avec temps négatif", test_goldGainOffline_negativeElapsed) == NULL))
    {
       CU_cleanup_registry();
       return CU_get_error();
    }

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}