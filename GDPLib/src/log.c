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
*/

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#include <time.h>
#include <assert.h>
#include "log.h"

/* Variabler */

GDPL_log_type gdpl_log_nivaa = -1;

FILE *gdpl_log_stream ;


/* Funksjoner */

void GDPL_log(GDPL_log_type type, const char* signatur, const char* melding, ...)
{
    time_t rawtime;
    struct tm * timeinfo;
    va_list ap;
    char *p, *sval;
    char type_str[16] = "";
    int ival;
    double dval;

    if (gdpl_log_nivaa > type)
        return;

    switch (type) {
    case GDPL_DEBUG:
        strcpy(type_str,"DEBUG");
        break;
    case GDPL_INFO:
        strcpy(type_str,"INFO");
        break;
    case GDPL_WARNING:
        strcpy(type_str,"WARNING");
        break;
    case GDPL_ERROR:
        strcpy(type_str,"ERROR");
        break;
    default:
        strcpy(type_str,"");
    }

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char time_str[32];
    sprintf(time_str,"%d:%d:%d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

    fprintf(gdpl_log_stream,"%s %s %s: ",type_str, time_str, signatur);

    va_start(ap, melding);
    for (p = (char*)melding; *p; p++) {
        if (*p != '%') {
            fputc(*p,gdpl_log_stream);
            continue;
        }
        switch (*++p) {
        case 'd':
            ival = va_arg(ap, int);
            fprintf(gdpl_log_stream,"%d", ival);
            break;
        case 'f':
            dval = va_arg(ap, double);
            fprintf(gdpl_log_stream,"%f", dval);
            break;
        case 's':
            for (sval = va_arg(ap, char *); *sval; sval++)
                putchar(*sval);
            break;
        default:
            fputc(*p,gdpl_log_stream);
            break;
        }
    }

    fprintf(gdpl_log_stream, "\n");
    fflush(gdpl_log_stream);
}

int GDPL_log_init(GDPL_log_type nivaa, FILE * stream)
{
    const char* signatur = "GDPL_init";

    /* Sett loggnivå og loggdestinasjon ihht inputparametre. */
    gdpl_log_nivaa = nivaa;
    gdpl_log_stream = stream;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}


