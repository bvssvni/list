#if 0
#!/bin/bash
clear
gcc -o test-list *.c -Wall -O3
if [ "$?" = "0" ]; then
	time ./test-list
fi
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ref.h"
#include "list.h"

/*
 
 I want to create an item type that can be any native C type.
 This is to be able to support dynamic data types.
 
 I want it to support garbage collected types.
 I need a list to contain items that expands by itself.
 
 */


void test_ToChar_1(void)
{
	item a = item_FromChar('a');
	item b = item_ToChar(a);
	assert(b.type == ITEM_TYPE_CHAR);
	assert(b.value.char_value == 'a');
}

void test_ToChar_2(void)
{
	item a = item_FromInt('a');
	item b = item_ToChar(a);
	assert(b.type == ITEM_TYPE_CHAR);
	assert(b.value.char_value == 'a');
}

void test_ToChar_3(void)
{
	item a = item_FromFloat(4.5f);
	item b = item_ToChar(a);
	assert(b.type == ITEM_TYPE_CHAR);
	assert(b.value.char_value == 4);
}

void test_NULL_1(void)
{
	item a = item_NULL();
	assert(a.type == 0);
	assert(a.value.char_value == 0);
	assert(a.value.int_value == 0);
	assert(a.value.float_value == 0);
	assert(a.value.double_value == 0);
	assert(a.value.ull_value == 0ULL);
	assert(a.value.string_value == NULL);
	assert(a.value.ref_value == NULL);
}

void test_list_1(void)
{
	list *li = list_New(10);

	assert(li->cap == 10);
	assert(li->len == 0);
	
	gcSet(li, NULL);
}

void test_list_2(void)
{
	list *li = list_New(0);
	
	item a = item_FromDouble(20.5);
	list_Add(li, a);
	
	assert(li->len == 1);
	assert(li->cap == 1);
	assert(li->ptr[0].type == ITEM_TYPE_DOUBLE);
	assert(li->ptr[0].value.double_value == 20.5);
	
	item b = item_FromChar('b');
	list_Add(li, b);
	
	assert(li->len == 2);
	assert(li->cap == 2);
	assert(li->ptr[1].type == ITEM_TYPE_CHAR);
	assert(li->ptr[1].value.char_value == 'b');
	
	item c = item_FromString("hello");
	list_Add(li, c);
	
	assert(li->len == 3);
	
	list *sublist = list_New(0);
	list_Add(li, item_FromRef((ref*)sublist));
	gcSet(sublist, NULL);
	
	assert(li->len == 4);
	assert(li->ptr[3].type == ITEM_TYPE_REF);
	assert(li->ptr[3].value.ref_value->ref.keep == 0);
	
	gcSet(li, NULL);
}

void test_list_3(void)
{
	list *li = list_New(0);
	
	gcSet(li, NULL);
}

void test_list_4(void)
{
	list *a = list_New(0);
	list *b = list_New(0);
	
	assert(b->ref.keep == 0);
	
	item it = item_FromRef((ref*)b);
	
	assert(b->ref.keep == 0);
	
	list_Add(a, it);
	
	assert(b->ref.keep == 1);
	
	gcSet(a, NULL);
	
	assert(b->ref.keep == 0);
	
	gcSet(b, NULL);
}

void test_FromString(void)
{
	item a = item_FromString("hello");
	a = item_NULL();
}

int main(int argc, char *argv[])
{
	int i;
	int end = 1 << 0;
	
	for (i = 0; i < end; i++) {
		test_NULL_1();
		
		test_ToChar_1();
		test_ToChar_2();
		test_ToChar_3();
		
		test_list_1();
		test_list_2();
		test_list_3();
		test_list_4();
		
		test_FromString();
	}
	printf("%i unit tests succeeded!\n", i);
	
	return 0;
}

