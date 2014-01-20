/* 
** This file is part of the GDPLib project.
** 
** Copyright (C) Halftan Sætherskar (halftan@saetherskar.no)
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
******************************************************************************
**
** Beskrivelse
**   Denne fila beskriver ulike generelle hjelpefunksjoner.
**
** void GDPL_util_log(enum GDPL_log_type type, const char* signature, char* message, ...) 
**
**
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>     
#include "gdpl.h"

/* 
 * Funksjon
 *  void GDPL_util_log(enum GDPL_log_type type, const char* signature, char* message, ...)
 *  
 * Beskrivelse
 *  Skriv ei melding til stdout, med tidsangivelse. Den globale variabelen 
 *  GDPL_log_level fungerer som en nivåregulator mht hva som skal logges. 
 * 
 * Parametre  
 *  type - DEBUG, INFO, WARNING, ERROR
 *  signature - navn på funksjon
 *  message - loggmeldinga
 *  ... - variablet sett med argumenter til loggmeldinga, ala printf()
 * 
 * Returnerer
 *  ingen ting.
 * 
 * Eksempel på bruk
 *  GDPL_util_log(DEBUG, "main()", "Hello %s", "world"); 
 * 
 */ 
void GDPL_util_log(enum GDPL_log_type type, const char* signature, char* message, ...) {

	time_t rawtime;
	struct tm * timeinfo; 	
	va_list ap;
	char *p, *sval;
	char type_str[16] = "";
	int ival;
	double dval;

	if (GDPL_log_level > type)
		return;

	switch (type) {
	case DEBUG:
		strcpy(type_str,"DEBUG");
		break;
	case INFO:
		strcpy(type_str,"INFO");
		break;
	case WARNING:
		strcpy(type_str,"WARNING");
		break;
	case ERROR:
		strcpy(type_str,"ERROR");
		break;
	default:
		strcpy(type_str,"");
	}

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	char time_str[32];
	sprintf(time_str,"%d:%d:%d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

	printf("%s %s %s: ",type_str, time_str, signature);

	va_start(ap, message);
	for (p = message; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		switch (*++p) {
		case 'd':
			ival = va_arg(ap, int);
			printf("%d", ival);
			break;
		case 'f':
			dval = va_arg(ap, double);
			printf("%f", dval);
			break;
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++)
				putchar(*sval);
			break;
		default:
			putchar(*p);
			break;
		}
	}
	
	printf("\n");

}

