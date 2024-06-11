#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "eFile.h"

#define FILENAME1 "text_files/file1.txt" /* Empty */
#define FILENAME2 "text_files/file2.txt" /* 3 lines, normal file */
#define FILENAME3 "text_files/file3.txt" /* 30000 'a' and 1 '\n' */
#define FILENAME4 "doesnt_exist" /* The file does not exist */
#define FILENAME5 "error*!@ /." /* syntax error in filenam */


void test_open_eFile_empty(eFile *file);
void test_open_eFile_normal(eFile *file);
void test_open_eFile_bigline(eFile *file);



/* Empty file */
void test_open_eFile_empty_readwrite(void)
{
	eFile *file = open_eFile(FILENAME1, "r+");
	test_open_eFile_empty(file);
}

void test_open_eFile_empty_readonly(void)
{
	eFile *file = open_eFile(FILENAME1, "r");
	test_open_eFile_empty(file);
}

void test_open_eFile_empty(eFile *file)
{
	CU_ASSERT_PTR_NOT_NULL(file);

	if(file != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(file->fp);
		CU_ASSERT_PTR_NULL(file->first);
		CU_ASSERT_EQUAL(file->n_elines, 0);
	}
}


/* Normal file */
void test_open_eFile_normal_readonly(void)
{
	eFile *file = open_eFile(FILENAME2, "r");
	test_open_eFile_normal(file);
}

void test_open_eFile_normal_readwrite(void)
{
	eFile *file = open_eFile(FILENAME2, "r+");
	test_open_eFile_normal(file);
}

void test_open_eFile_normal(eFile *file)
{

	CU_ASSERT_PTR_NOT_NULL(file);
	if(file != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(file->fp);
		
		eLine *current = file->first;
		for(int i=0; i<3 ; i++)
		{
			CU_ASSERT_PTR_NOT_NULL(current);
			if(current != NULL)
			{
				switch(i)
				{
					case 0:
						CU_ASSERT_PTR_NOT_NULL(current->string);
						if(current->string != NULL)
							CU_ASSERT_STRING_EQUAL(current->string, "Hello world!\n");
				   		CU_ASSERT_EQUAL(current->length, 13);
						CU_ASSERT_EQUAL(current->pos, 1);
						CU_ASSERT_PTR_NULL(current->previous);
						CU_ASSERT_PTR_NOT_NULL(current->next);
						break;
					case 1:
						CU_ASSERT_PTR_NOT_NULL(current->string);
						if(current->string != NULL)
							CU_ASSERT_STRING_EQUAL(current->string, "\n");
					   	CU_ASSERT_EQUAL(current->length, 1);
						CU_ASSERT_EQUAL(current->pos, 2);
						CU_ASSERT_PTR_NOT_NULL(current->previous);
						CU_ASSERT_PTR_NOT_NULL(current->next);
						break;
					case 2:
						CU_ASSERT_PTR_NOT_NULL(current->string);
						if(current->string != NULL)
							CU_ASSERT_STRING_EQUAL(current->string, "I'm fine!\n");
					   	CU_ASSERT_EQUAL(current->length, 10);
						CU_ASSERT_EQUAL(current->pos, 3);
						CU_ASSERT_PTR_NOT_NULL(current->previous);
						CU_ASSERT_PTR_NULL(current->next);
						break;
				}
				current = current->next;
			}
		}
		CU_ASSERT_EQUAL(file->n_elines, 3);
	}
}


void test_open_eFile_bigline_readonly(void)
{
	eFile *file = open_eFile(FILENAME3, "r");
	test_open_eFile_bigline(file);
}

void test_open_eFile_bigline_readwrite(void)
{
	eFile *file = open_eFile(FILENAME3, "r+");
	test_open_eFile_bigline(file);
}

void test_open_eFile_bigline(eFile *file)
{
	CU_ASSERT_PTR_NOT_NULL(file);

	if(file != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(file->fp);
		CU_ASSERT_PTR_NOT_NULL(file->first);

		if(file->first != NULL)
		{
			char expected_string[30002]; // 30000 'a' + '\n' + '\0'
    		memset(expected_string, 'a', 30000);
    		expected_string[30000] = '\n';
    		expected_string[30001] = '\0';

			CU_ASSERT_PTR_NOT_NULL(file->first->string);
			CU_ASSERT_STRING_EQUAL(file->first->string, expected_string);
		  	CU_ASSERT_EQUAL(file->first->length, 30001);
			CU_ASSERT_EQUAL(file->first->pos, 1);
			CU_ASSERT_PTR_NULL(file->first->previous);
			CU_ASSERT_PTR_NULL(file->first->next);
		}
		CU_ASSERT_EQUAL(file->n_elines, 1);
	}	
}


void add_tests_eFile(CU_pSuite pSuite)
{
    CU_add_test(pSuite, "open_eFile() with empty file (readonly)", test_open_eFile_empty_readonly);
    CU_add_test(pSuite, "open_eFile() with empty file (readwrite)", test_open_eFile_empty_readwrite);
    CU_add_test(pSuite, "open_eFile() with normal file (readonly)", test_open_eFile_normal_readwrite);
    CU_add_test(pSuite, "open_eFile() with normal file (readwrite)", test_open_eFile_normal_readwrite);
    CU_add_test(pSuite, "open_eFile() with bigline file (readonly)", test_open_eFile_bigline_readwrite);
    CU_add_test(pSuite, "open_eFile() with bigline file (readwrite)", test_open_eFile_bigline_readwrite);
}
