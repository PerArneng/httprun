/* --
 * Copyright (C) 2009 Per Arneng
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Boston, MA  02110-1301, USA.
 * --
 */

#include "testutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <execinfo.h>

void run_test( const char* test_name, void (*test_case)(void)) {
	printf("%s -->", test_name);
	(*test_case)();
	printf("<-- finished\n");
}

int string_compare(const char* expected, const char* actual) {
	int comparrison = strcmp(expected,actual);

	if(comparrison != 0) {
		printf("strings are not equal: expected: '%s' actual: '%s'\n", expected, actual);
		dump_string(expected);
		dump_string(actual);
		return 0;
	} else {
		return 1;
	}
}

void print_backtrace() {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace (array, 10);
    strings = backtrace_symbols (array, size);

    printf ("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++)
       printf ("%s\n", strings[i]);

    free (strings);

}

void dump_string(const char* str) {

	printf("str: '%s' strlen:%d sizeof:%d --> ",str,
		   (int)strlen(str), (int)sizeof(str));

	int size = strlen(str);
	for(int i=0;i<size;i++) {
		printf("[%d:%d:%c]",i,str[i],str[i]);
	}
	printf("\n");

}

