#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "eFile.h"

#define FILENAME1 "text_files/file1.txt" /* Empty */
#define FILENAME2 "text_files/file2.txt" /* 3 lines, normal file */
#define FILENAME3 "text_files/file3.txt" /* 30000 'a' and 1 '\n' */
#define FILENAME4 "text_files/file4.txt" /* Same as FILENAME2 but readonly */
#define FILENAME5 "text_files/file5.txt" /* No perm */


/* Empty file */
void test_create_eFile_empty(void)
{
	eFile *file = create_eFile(FILENAME1);
	CU_ASSERT_PTR_NOT_NULL(file);

	if(file != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(file->filename);
		if(file->filename != NULL)
			CU_ASSERT_STRING_EQUAL(file->filename, FILENAME1);
		CU_ASSERT_PTR_NOT_NULL(file->first);
		if(file->first)
		{
			CU_ASSERT_PTR_NOT_NULL(file->first->string);
			if(file->first->string != NULL)
				CU_ASSERT_STRING_EQUAL(file->first->string, "");
			CU_ASSERT_EQUAL(file->first->length, 0);
			CU_ASSERT_EQUAL(file->first->pos, 1);
			CU_ASSERT_PTR_NULL(file->first->previous);
			CU_ASSERT_PTR_NULL(file->first->next);

		}
		CU_ASSERT_EQUAL(file->n_elines, 1);
		CU_ASSERT_EQUAL(file->permissions, p_READWRITE);
	}
}


/* Normal file */
void test_create_eFile_normal(void)
{
	eFile *file = create_eFile(FILENAME2);

	CU_ASSERT_PTR_NOT_NULL(file);
	if(file != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(file->filename);
		if(file->filename != NULL)
			CU_ASSERT_STRING_EQUAL(file->filename, FILENAME2);
		CU_ASSERT_EQUAL(file->permissions, p_READWRITE);
		CU_ASSERT_EQUAL(file->n_elines, 3);
		
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
							CU_ASSERT_STRING_EQUAL(current->string, "Hello world!");
				   		CU_ASSERT_EQUAL(current->length, 12);
						CU_ASSERT_EQUAL(current->pos, 1);
						CU_ASSERT_PTR_NULL(current->previous);
						CU_ASSERT_PTR_NOT_NULL(current->next);
						break;
					case 1:
						CU_ASSERT_PTR_NOT_NULL(current->string);
						if(current->string != NULL)
							CU_ASSERT_STRING_EQUAL(current->string, "");
					   	CU_ASSERT_EQUAL(current->length, 0);
						CU_ASSERT_EQUAL(current->pos, 2);
						CU_ASSERT_PTR_NOT_NULL(current->previous);
						CU_ASSERT_PTR_NOT_NULL(current->next);
						break;
					case 2:
						CU_ASSERT_PTR_NOT_NULL(current->string);
						if(current->string != NULL)
							CU_ASSERT_STRING_EQUAL(current->string, "I'm fine!");
					   	CU_ASSERT_EQUAL(current->length, 9);
						CU_ASSERT_EQUAL(current->pos, 3);
						CU_ASSERT_PTR_NOT_NULL(current->previous);
						CU_ASSERT_PTR_NULL(current->next);
						break;
				}
				current = current->next;
			}
		}
	}
}

/* Bifline file */
void test_create_eFile_bigline(void)
{
	eFile *file = create_eFile(FILENAME3);

	CU_ASSERT_PTR_NOT_NULL(file);

	if(file != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(file->filename);
		if(file->filename != NULL)
			CU_ASSERT_STRING_EQUAL(file->filename, FILENAME3);
		CU_ASSERT_EQUAL(file->permissions, p_READWRITE);
		CU_ASSERT_EQUAL(file->n_elines, 1);
		CU_ASSERT_PTR_NOT_NULL(file->first);

		if(file->first != NULL)
		{
			char expected_string[30001]; // 30000 'a' + '\0'
    		memset(expected_string, 'a', 30000);
    		expected_string[30000] = 0;

			CU_ASSERT_PTR_NOT_NULL(file->first->string);
			CU_ASSERT_STRING_EQUAL(file->first->string, expected_string);
		  	CU_ASSERT_EQUAL(file->first->length, 30000);
			CU_ASSERT_EQUAL(file->first->pos, 1);
			CU_ASSERT_PTR_NULL(file->first->previous);
			CU_ASSERT_PTR_NULL(file->first->next);
		}
	}	
}

void test_create_eFile_readonly(void)
{
	eFile *file = create_eFile(FILENAME4);

	CU_ASSERT_PTR_NOT_NULL(file);
	if(file != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(file->filename);
		if(file->filename != NULL)
			CU_ASSERT_STRING_EQUAL(file->filename, FILENAME4);
		CU_ASSERT_EQUAL(file->permissions, p_READONLY);
		CU_ASSERT_EQUAL(file->n_elines, 3);
		
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
							CU_ASSERT_STRING_EQUAL(current->string, "Hello world!");
				   		CU_ASSERT_EQUAL(current->length, 12);
						CU_ASSERT_EQUAL(current->pos, 1);
						CU_ASSERT_PTR_NULL(current->previous);
						CU_ASSERT_PTR_NOT_NULL(current->next);
						break;
					case 1:
						CU_ASSERT_PTR_NOT_NULL(current->string);
						if(current->string != NULL)
							CU_ASSERT_STRING_EQUAL(current->string, "");
					   	CU_ASSERT_EQUAL(current->length, 0);
						CU_ASSERT_EQUAL(current->pos, 2);
						CU_ASSERT_PTR_NOT_NULL(current->previous);
						CU_ASSERT_PTR_NOT_NULL(current->next);
						break;
					case 2:
						CU_ASSERT_PTR_NOT_NULL(current->string);
						if(current->string != NULL)
							CU_ASSERT_STRING_EQUAL(current->string, "I'm fine!");
					   	CU_ASSERT_EQUAL(current->length, 9);
						CU_ASSERT_EQUAL(current->pos, 3);
						CU_ASSERT_PTR_NOT_NULL(current->previous);
						CU_ASSERT_PTR_NULL(current->next);
						break;
				}
				current = current->next;
			}
		}
	}

}

void test_create_eFile_noperm(void)
{
	eFile *file = create_eFile(FILENAME5);
	CU_ASSERT_PTR_NULL(file);
}

void add_tests_eFile(CU_pSuite pSuite)
{
    CU_add_test(pSuite, "create_eFile() with empty file", test_create_eFile_empty);
    CU_add_test(pSuite, "create_eFile() with normal file", test_create_eFile_normal);
    CU_add_test(pSuite, "create_eFile() with bigline file", test_create_eFile_bigline);
    CU_add_test(pSuite, "create_eFile() with readonly file", test_create_eFile_readonly);
    CU_add_test(pSuite, "create_eFile() with no perm file", test_create_eFile_noperm);
}
