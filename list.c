#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ref.h"
#include "list.h"

item item_NULL()
{
	return (item){.type = ITEM_TYPE_NULL};
}

item item_FromChar(char ch)
{
	return (item){.type = ITEM_TYPE_CHAR, .value.char_value = ch};
}

item item_FromInt(int val)
{
	return (item){.type = ITEM_TYPE_INT, .value.int_value = val};
}

item item_FromFloat(float val)
{
	return (item){.type = ITEM_TYPE_FLOAT, .value.float_value = val};
}

item item_FromDouble(double val)
{
	return (item){.type = ITEM_TYPE_DOUBLE, .value.double_value = val};
}

item item_FromUnsignedLongLong(unsigned long long val)
{
	return (item){.type = ITEM_TYPE_UNSIGNED_LONG_LONG, .value.ull_value = val};
}

item item_FromRef(ref *val)
{
	return (item){.type = ITEM_TYPE_REF, .value.ref_value = (reference*)val};
}

item item_CopyString(char *val)
{
	int length = strlen(val)+1;
	char *copy = malloc(sizeof(char)*length);
	memcpy(copy, val, length);
	return (item){.type = ITEM_TYPE_STRING, .value.string_value = copy};
}

item item_FromString(char *val)
{
	return (item){.type = ITEM_TYPE_STRING, .value.string_value = val};
}

item item_ToChar(item it)
{
	switch (it.type) {
		case ITEM_TYPE_CHAR: return it;
		case ITEM_TYPE_INT: return item_FromChar(it.value.int_value);
		case ITEM_TYPE_FLOAT: return item_FromChar(it.value.float_value);
		case ITEM_TYPE_DOUBLE: return item_FromChar(it.value.double_value);
		case ITEM_TYPE_UNSIGNED_LONG_LONG:
			return item_FromChar(it.value.ull_value);
		case ITEM_TYPE_STRING: {
			char ch;
			if (sscanf(it.value.string_value, "%c", &ch)) {
				return item_FromChar(ch);
			} else {
				return item_NULL();
			}
		}
		default: return item_NULL();
	}
}

item item_ToInt(item it)
{
	switch (it.type) {
		case ITEM_TYPE_CHAR: return item_FromInt(it.value.char_value);
		case ITEM_TYPE_INT: return it;
		case ITEM_TYPE_FLOAT: return item_FromInt(it.value.float_value);
		case ITEM_TYPE_DOUBLE: return item_FromInt(it.value.double_value);
		case ITEM_TYPE_UNSIGNED_LONG_LONG:
			return item_FromChar(it.value.ull_value);
		case ITEM_TYPE_STRING: {
			int val;
			if (sscanf(it.value.string_value, "%i", &val)) {
				return item_FromInt(val);
			} else {
				return item_NULL();
			}
		}
		default: return item_NULL();
	}
}

item item_ToFloat(item it)
{
	switch (it.type) {
		case ITEM_TYPE_CHAR: return item_FromFloat(it.value.char_value);
		case ITEM_TYPE_INT: return item_FromFloat(it.value.int_value);
		case ITEM_TYPE_FLOAT: return it;
		case ITEM_TYPE_DOUBLE: return item_FromFloat(it.value.double_value);
		case ITEM_TYPE_UNSIGNED_LONG_LONG:
			return item_FromFloat(it.value.ull_value);
		case ITEM_TYPE_STRING: {
			float val;
			if (sscanf(it.value.string_value, "%g", &val)) {
				return item_FromFloat(val);
			} else {
				return item_NULL();
			}
		}
		default: return item_NULL();
	}
}

item item_ToDouble(item it)
{
	switch (it.type) {
		case ITEM_TYPE_CHAR: return item_FromDouble(it.value.char_value);
		case ITEM_TYPE_INT: return item_FromDouble(it.value.int_value);
		case ITEM_TYPE_FLOAT: return item_FromDouble(it.value.float_value);
		case ITEM_TYPE_DOUBLE: return it;
		case ITEM_TYPE_UNSIGNED_LONG_LONG:
			return item_FromDouble(it.value.ull_value);
		case ITEM_TYPE_STRING: {
			double val;
			if (sscanf(it.value.string_value, "%lg", &val)) {
				return item_FromDouble(val);
			} else {
				return item_NULL();
			}
		}
		default: return item_NULL();
	}
}

item item_ToUnsignedLongLong(item it)
{
	switch (it.type) {
		case ITEM_TYPE_CHAR:
			return item_FromUnsignedLongLong(it.value.char_value);
		case ITEM_TYPE_INT:
			return item_FromUnsignedLongLong(it.value.int_value);
		case ITEM_TYPE_FLOAT:
			return item_FromUnsignedLongLong(it.value.float_value);
		case ITEM_TYPE_DOUBLE:
			return item_FromUnsignedLongLong(it.value.double_value);
		case ITEM_TYPE_UNSIGNED_LONG_LONG: return it;
		case ITEM_TYPE_STRING: {
			unsigned long long val;
			if (sscanf(it.value.string_value, "%llu", &val)) {
				return item_FromUnsignedLongLong(val);
			} else {
				return item_NULL();
			}
		}
		default: return item_NULL();
	}
}

item item_ToRef(item it)
{
	switch (it.type) {
		case ITEM_TYPE_REF: return it;
		default: return item_NULL();
	}
}

item item_ToString(item it)
{
	switch (it.type) {
		case ITEM_TYPE_CHAR: {
			char buf[] = {it.value.char_value, '\0'};
			return item_FromString(buf);
		}
		case ITEM_TYPE_INT: {
			char buf[1024];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%i", it.value.int_value);
			return item_FromString(buf);
		}
		case ITEM_TYPE_FLOAT: {
			char buf[1024];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%g", it.value.float_value);
			return item_FromString(buf);
		}
		case ITEM_TYPE_DOUBLE: {
			char buf[1024];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%lg", it.value.double_value);
			return item_FromString(buf);
		}
		case ITEM_TYPE_UNSIGNED_LONG_LONG: {
			char buf[1024];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%llu", it.value.ull_value);
			return item_FromString(buf);
		}
		case ITEM_TYPE_STRING: return it;
		default: return item_NULL();
	}
}

void list_Delete(void *ptr)
{
	list *li = ptr;
	int i;
	for (i = 0; i < li->len; i++) {
		item it = li->ptr[i];
		if (it.type == ITEM_TYPE_REF) {
			gcSet(it.value.ref_value, NULL);
		} else if (it.type == ITEM_TYPE_STRING) {
			if (it.value.string_value != NULL) free(it.value.string_value);
		}
		it = item_NULL();
	}
	
	if (li->ptr != NULL) free(li->ptr);
}

// Expands the list capacity if necessary.
void list_CheckCapacity(list *li, int count)
{
	int newsize = li->len + count;
	if (newsize <= li->cap) return;
	
	int c = li->cap == 0 ? 1 : li->cap;
	while (c < newsize) c <<= 1;
	
	item *items = malloc(sizeof(item)*c);
	memset(items, 0, sizeof(item)*c);
	memcpy(items, li->ptr, sizeof(item)*li->len);
	if (li->ptr != NULL) free(li->ptr);
	li->ptr = items;
	li->cap += count;
}

list *list_New(int capacity)
{
	gcInit(list, li, .ref.destructor = list_Delete);
	list_CheckCapacity(li, capacity);
	return li;
}

// Overwrites data but makes sure to increase reference counter or
// copy string.
void item_Overwrite(item *a, item *b)
{
	switch (a->type) {
		case ITEM_TYPE_REF: {
			gcSet(a->value.ref_value, NULL);
			break;
		}
		case ITEM_TYPE_STRING: {
			if (a->value.string_value != NULL) free(a->value.string_value);
			break;
		}
	}
	*a = item_NULL();
	
	switch (b->type) {
		case ITEM_TYPE_REF: {
			gcSet(a->value.ref_value, b->value.ref_value);
			a->type = ITEM_TYPE_REF;
			break;
		}
		case ITEM_TYPE_STRING: {
			*a = item_CopyString(b->value.string_value);
			a->type = ITEM_TYPE_STRING;
			break;
		}
		default: *a = *b;
	}
	 
}

void list_Add(list *li, item it)
{
	list_CheckCapacity(li, 1);
	item_Overwrite(&li->ptr[li->len], &it);
	li->len++;
}

void list_Set(list *li, int index, item it)
{
	list_CheckCapacity(li, index+1);
	item_Overwrite(li->ptr + index, &it);
	li->len = index < li->len ? li->len : index+1;
}


list *list_Copy(list *li)
{
	list *newlist = list_New(li->len);
	int i;
	for (i = 0; i < li->len; i++) {
		list_Add(newlist, li->ptr[i]);
	}
	return newlist;
}

