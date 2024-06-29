#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>

#include "eLine.h"


void test_create_eLine_normal(void)
{
	eLine *next = create_eLine("Hello world!", 32, 2, NULL, NULL);
	eLine *previous = create_eLine("Hello world!", 32, 0, NULL, NULL);
	eLine *eline = create_eLine("Hello world!", 32, 1, previous, next);

	CU_ASSERT_PTR_NOT_NULL(next);
	CU_ASSERT_PTR_NOT_NULL(previous);
	CU_ASSERT_PTR_NOT_NULL(eline);
	if(next != NULL && previous != NULL && eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "Hello world!");
		CU_ASSERT_EQUAL(eline->length, 12);
		CU_ASSERT_EQUAL(eline->alloc_size, 32);
		CU_ASSERT_EQUAL(eline->pos, 1);
		CU_ASSERT_EQUAL(eline->previous, previous);
		CU_ASSERT_EQUAL(eline->next, next);

		if(previous->string != NULL)
			free(previous->string);
		if(next->string != NULL)
			free(next->string);
		if(eline->string != NULL)
			free(eline->string);
		free(next);
		free(previous);
		free(eline);
	}
}

void test_create_eLine_size_lower_than_length(void)
{
	eLine *eline = create_eLine("Hello world!", 3, 0, NULL, NULL);

	CU_ASSERT_PTR_NOT_NULL(eline);
	
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "Hel");
		CU_ASSERT_EQUAL(eline->length, 3);
		CU_ASSERT_EQUAL(eline->alloc_size, 4);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}

void test_delete_eLine_normal(void)
{
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);

	delete_eLine(&eline);

	CU_ASSERT_PTR_NULL(eline);
}

void test_insert_string_eLine_normal(void)
{
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);
	int result = insert_string_eLine(eline, "the big ", 32, 6);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "Hello the big world!");
		
		CU_ASSERT_EQUAL(eline->length, 20);
		CU_ASSERT_EQUAL(eline->alloc_size, 32);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}

void test_insert_string_eLine_limit1(void)
{
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);
	int result = insert_string_eLine(eline, "$$$$", 32, 0);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "$$$$Hello world!");
		
		CU_ASSERT_EQUAL(eline->length, 16);
		CU_ASSERT_EQUAL(eline->alloc_size, 32);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}

void test_insert_string_eLine_limit2(void)
{
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);
	int result = insert_string_eLine(eline, "$$$", 32, 0);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "$$$Hello world!");
		
		CU_ASSERT_EQUAL(eline->length, 15);
		CU_ASSERT_EQUAL(eline->alloc_size, 16);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}

void test_insert_string_eLine_limit3(void)
{
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);
	int result = insert_string_eLine(eline, "$$$$", 32, 12);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "Hello world!$$$$");
		
		CU_ASSERT_EQUAL(eline->length, 16);
		CU_ASSERT_EQUAL(eline->alloc_size, 32);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}

void test_insert_string_eLine_bad_pos(void)
{
	
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);
	int result = insert_string_eLine(eline, "BAD GUY", 32, 1000);

	CU_ASSERT_EQUAL(result, -1);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "Hello world!");
		
		CU_ASSERT_EQUAL(eline->length, 12);
		CU_ASSERT_EQUAL(eline->alloc_size, 16);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}


void test_insert_char_eLine_normal(void)
{
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);
	int result = insert_char_eLine(eline, 'q', 6);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "Hello qworld!");
		
		CU_ASSERT_EQUAL(eline->length, 13);
		CU_ASSERT_EQUAL(eline->alloc_size, 16);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}


void test_insert_char_eLine_limit1(void)
{
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);
	int result = insert_char_eLine(eline, '$', 0);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "$Hello world!");
		
		CU_ASSERT_EQUAL(eline->length, 13);
		CU_ASSERT_EQUAL(eline->alloc_size, 16);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}


void test_insert_char_eLine_limit2(void)
{
	eLine *eline = create_eLine("Hello world!", 32, 0, NULL, NULL);
	int result = insert_char_eLine(eline, '$', eline->length);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "Hello world!$");
		
		CU_ASSERT_EQUAL(eline->length, 13);
		CU_ASSERT_EQUAL(eline->alloc_size, 16);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}


void test_insert_char_eLine_limit3(void)
{
	eLine *eline = create_eLine("aaaaaaaaaaaaaaa", 15, 0, NULL, NULL);
	int result = insert_char_eLine(eline, '$', eline->length);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "aaaaaaaaaaaaaaa$");
		
		CU_ASSERT_EQUAL(eline->length, 16);
		CU_ASSERT_EQUAL(eline->alloc_size, 32);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}


void test_insert_char_eLine_limit4(void)
{
	eLine *eline = create_eLine("aaaaaaaaaaaaaa", 14, 0, NULL, NULL);
	int result = insert_char_eLine(eline, '$', eline->length);

	CU_ASSERT_EQUAL(result, 0);

	CU_ASSERT_PTR_NOT_NULL(eline);
	if(eline != NULL)
	{
		CU_ASSERT_PTR_NOT_NULL(eline->string);
		if(eline->string != NULL)
			CU_ASSERT_STRING_EQUAL(eline->string, "aaaaaaaaaaaaaa$");
		
		CU_ASSERT_EQUAL(eline->length, 15);
		CU_ASSERT_EQUAL(eline->alloc_size, 16);
		CU_ASSERT_EQUAL(eline->pos, 0);
		CU_ASSERT_PTR_NULL(eline->previous);
		CU_ASSERT_PTR_NULL(eline->next);

		if(eline->string != NULL)
			free(eline->string);
		free(eline);
	}
}


void add_tests_eLine(CU_pSuite pSuite)
{
	CU_add_test(pSuite, "create_eLine() with normal line", test_create_eLine_size_lower_than_length);
	CU_add_test(pSuite, "create_eLine() with size lower than leght", test_create_eLine_size_lower_than_length);
	CU_add_test(pSuite, "delete_eLine() with normal line", test_delete_eLine_normal);
	CU_add_test(pSuite, "insert_string_eLine() with normal insertion", test_insert_string_eLine_normal);
	CU_add_test(pSuite, "insert_string_eLine() with bad position insertion", test_insert_string_eLine_bad_pos);
	CU_add_test(pSuite, "insert_string_eLine() with limit test1", test_insert_string_eLine_limit1);
	CU_add_test(pSuite, "insert_string_eLine() with limit test2", test_insert_string_eLine_limit2);
	CU_add_test(pSuite, "insert_string_eLine() with limit test3", test_insert_string_eLine_limit3);
	CU_add_test(pSuite, "insert_char_eLine() with normal inserttion", test_insert_char_eLine_normal);
	CU_add_test(pSuite, "insert_char_eLine() with limit test1", test_insert_char_eLine_limit1);
	CU_add_test(pSuite, "insert_char_eLine() with limit test2", test_insert_char_eLine_limit2);
	CU_add_test(pSuite, "insert_char_eLine() with limit test3", test_insert_char_eLine_limit3);
	CU_add_test(pSuite, "insert_char_eLine() with limit test4", test_insert_char_eLine_limit4);
}
