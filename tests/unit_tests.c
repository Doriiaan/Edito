#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>


extern void add_tests_eFile(CU_pSuite pSuite);
extern void add_tests_eLine(CU_pSuite pSuite);


int main(void) 
{
	/* Registry init */
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();


    /* Suite creation */
    CU_pSuite eFile_pSuite = CU_add_suite("eFile_pSuite", NULL, NULL);
    CU_pSuite eLine_pSuite = CU_add_suite("eLine_pSuite", NULL, NULL);
    if (eFile_pSuite == NULL || eLine_pSuite == NULL) 
	{
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* Add tests to Suite */
	add_tests_eFile(eFile_pSuite);
	add_tests_eLine(eLine_pSuite);


    /* Basic execution */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();


    /* Registry cleanup */
    CU_cleanup_registry();

    return CU_get_error();
}

