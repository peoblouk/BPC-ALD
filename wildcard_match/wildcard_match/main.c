/** \file main.c
 *  \brief Hlavní program (testbench) pro manuální testování spravnosti implementace funkce wildcard_match
 *  \author Petyovský
 *  \version 2024
 *  $Id: main.c 2663 2024-03-25 10:50:21Z petyovsky $
 */

#include <stdio.h>
#include <string.h>
#include "wildcard_match.h"

/** \defgroup MainProgram 2. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

#define STRINGS_MAXSIZE 1024		///< Maximální velikost vstupního řetězce

int main(int argc, char *argv[])
	{
	printf("Insert wildcard string: ");
	char wildcard_str[STRINGS_MAXSIZE] = { '\0', };
	fgets(wildcard_str, STRINGS_MAXSIZE, stdin);
	wildcard_str[strcspn(wildcard_str, "\r\n")] = '\0';	// string complementary span: https://en.cppreference.com/w/c/string/byte/strcspn

	char str[STRINGS_MAXSIZE] = { '\0', };
	do
		{
		printf("Insert string: ");
		fgets(str, STRINGS_MAXSIZE, stdin);
		str[strcspn(str, "\r\n")] = '\0';
		if(strpbrk(str, "?*"))							// string pointer break: https://en.cppreference.com/w/c/string/byte/strpbrk
			puts("Warning: input string contains wildcard character(s).\n");

		if(wildcard_match(wildcard_str, str))
			puts("Strings match passed.\n");
		else
			puts("Strings match failed.\n");
		}
	while(strlen(str));

	return 0;
	}

/** \} MainProgram */
