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
#include "diverse.h"

GDPL_log_type gdpl_log_nivaa = -1;

FILE * gdpl_log_stream = 0;

/* ----------------------------------------------------------------------------
 * Funksjon
 *  void GDPL_log(GDPL_GDPL_log_type type, const char* signature, char* message, ...)
 * ----------------------------------------------------------------------------
 * Beskrivelse
 *  Skriv ei logg melding med tidsangivelse. To globale variabler styrer hva
 *  som skal logges og hvor det skal logges;
 *
 *  gdpl_log_stream - definerer hvor; stdout, stderr, eller til fil.
 *  gdpl_log_nivaa  - definerer minimumsnivået; DEBUG, INFO, WARNING.
 *
 * Parametre
 *  type - DEBUG, INFO, WARNING, ERROR
 *  signatur - navn på funksjon
 *  melding - loggmeldinga
 *  ... - variablet sett med argumenter til loggmeldinga, ala printf()
 *
 * Returnerer
 *  ingen ting.
 *
 * Eksempel på bruk
 *  GDPL_log(GDPL_DEBUG, "main()", "Hello %s", "world");
 * ----------------------------------------------------------------------------
 */
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

/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_init()
 *
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Initier GDPLib.
 *
 * Parametre
 *  nivaa på logging - DEBUG, INFO, WARNING eller ERROR
 *  stream for logging - stdout, stderr eller en annen filstream.
 *  datafil for data - et filnavn eller en nullpeker for default navn.
 *
 * Return
 *  0 - ok
 *  alt annet  - feil. Den returnerte verdien kan brukes som indeks til
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *
 * Eksempel på bruk
 *  int error_nr;
 *  error_nr = GDPL_initier()
 *  if (error_nr > 0) 'handle error'
 *
 * ----------------------------------------------------------------------------
 */
int GDPL_init_log(GDPL_log_type nivaa, FILE * stream)
{
    const char* signatur = "GDPL_init";

    /* Sett locale til 'norsk bokmål',
     noe som bør fikse øæå -problematikk. */
    setlocale(LC_ALL,"nb_NO.utf8");

    /* Sett loggnivå og loggdestinasjon ihht inputparametre. */
    gdpl_log_nivaa = nivaa;
    gdpl_log_stream = stream;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}


