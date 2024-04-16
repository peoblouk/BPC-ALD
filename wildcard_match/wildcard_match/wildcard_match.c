/** \file wildcard_match.c
 *  \brief Definice funkce wildcard_match() realizující test na shodu řetězců z nichž jeden obsahuje žolíkové znaky
 *  \author Petyovský
 *  \version 2024
 *  $Id: wildcard_match.c 2663 2024-03-25 10:50:21Z petyovsky $
 */
#include "wildcard_match.h"
#include <stdio.h>

bool wildcard_match(const char aWildCardStr[], const char aStr[])
{
	printf("\twildcard_match(\"%s\",\"%s\")\n", aWildCardStr, aStr);

	if (*aWildCardStr == '\0' && *aStr == '\0') // Narazil jsem na konec řetězce
		return true;

	if (*aWildCardStr == *aStr)							   // hvězdička značí dereferenci
		return wildcard_match(aWildCardStr + 1, aStr + 1); // Zavolám rekurzi

	if (*aWildCardStr == '?')
		if (*aStr == '\0')
			return false;
		else
			return wildcard_match(aWildCardStr + 1, aStr + 1); // Zavolám rekurzi

	if (*aWildCardStr == '*')
	{
		bool state = true;
		if (*aStr == '\0')
		{
			state = false;
		}
		else if (*aStr != '\0')
		{
			state = wildcard_match(aWildCardStr, aStr + 1);
		}
		if (state == false)
		{
			state = wildcard_match(aWildCardStr + 1, aStr);
		}
		return state;
	}

	return false;
}
