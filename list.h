/*
 List - Dynamic typed list for C with Ref garbage collector.
 BSD license.
 by Sven Nilsen, 2012
 http://www.cutoutpro.com
 
 Version: 0.000
 Angular degrees version notation
 http://isprogrammingeasy.blogspot.no/2012/08/angular-degrees-versioning-notation.html
 
 */
/*
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 */

/*
 
 The point with List is to provide an efficient way of working with
 general data and Ref garbage collected types.
 One thing that is made easier is converting to and from strings.
 
 item	General data type.
		NULL, char, int, float, double, unsigned long long, ref, string
 
 list	General list.
		Ref garbage collected.
 
 A list can contain lists.
 References are incremented with the Ref garbage collector.
 String are copied.
 
 */

#ifndef LIST_GUARD
#define LIST_GUARD

/*

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 #include "ref.h"
 #include "list.h"
 
 */

enum item_type
{
	ITEM_TYPE_NULL = 0,
	ITEM_TYPE_CHAR = 1,
	ITEM_TYPE_INT = 2,
	ITEM_TYPE_FLOAT = 3,
	ITEM_TYPE_DOUBLE = 4,
	ITEM_TYPE_UNSIGNED_LONG_LONG = 5,
	ITEM_TYPE_REF = 6,
	ITEM_TYPE_STRING = 7,
};

typedef struct
{
	ref ref;
} reference;

// Joins many common types, including Ref garbage collected items.
typedef struct {
	int type;
	union {
		char char_value;
		int int_value;
		float float_value;
		double double_value;
		unsigned long long ull_value;
		reference* ref_value;
		char *string_value;
	} value;
} item;

// Contains a list of items.
typedef struct {
	ref ref;
	int cap;
	int len;
	item *ptr;
} list;

item item_NULL();
item item_FromChar(char ch);
item item_FromInt(int val);
item item_FromFloat(float val);
item item_FromDouble(double val);
item item_FromUnsignedLongLong(unsigned long long val);
item item_FromRef(ref *val);
item item_FromString(char *val);
item item_ToChar(item it);
item item_ToInt(item it);
item item_ToFloat(item it);
item item_ToDouble(item it);
item item_ToUnsignedLongLong(item it);
item item_ToRef(item it);
void item_Overwrite(item *a, item *b);

list *list_New(int capacity);
void list_CheckCapacity(list *li, int count);
void list_Add(list *li, item it);
void list_Set(list *li, int index, item it);
item item_ToString(item it);
list *list_Copy(list *li);

#endif