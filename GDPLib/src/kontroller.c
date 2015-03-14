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
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#include <time.h>  
#include <assert.h>  
#include "gdpl.h"
#include "kontroller.h"
#include "konkurranse.h"


const char* gdpl_kontroller_gdplib_navn = "GDPLib";

const char* gdpl_kontroller_gdplib_versjon = "1.0";

const char *gdpl_kontroller_feilkoder[] = {
    "",
    "FEILKODE 01: Filnavnet er for langt.",
    "FEILKODE 02: Filnavnet er for kort.",
    "FEILKODE 03: Filnavnet er ikke definert.",
    "FEILKODE 04: Kan ikke opprette datafil.",
    "FEILKODE 05: Kan ikke lese fra datafil.",
    "FEILKODE 06: Kan ikke skrive til datafil.",
    "FEILKODE 07: Kan ikke allokere minne.",
    "FEILKODE 08: Id eksisterer allerede.",
    "FEILKODE 09: Id eksisterer ikke.",
    "FEILKODE 10: Generell feil.",
    "FEILKODE 11: Oppgitt verdi for max antall par, er for lavt.",
    "FEILKODE 12: Oppgitt verdi for max antall par, er for høyt.",
    "FEILKODE 13: Oppgitt verdi for max antall par, er ikke angitt."
};

int gdpl_kontroller_max_antall_par = -1;



/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  const char* GDPL_kontroller_gdplib_navn()
 *
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *   Returnerer navnet på biblioteket, f.eks 'GDPLib'.
 *
 * Parametre
 *   nop
 *
 * Returnerer
 *   En streng som representerer navnet på biblioteket.
 *
 * Eksempel på bruk
 *   const char* navn = GDPL_kontroller_gdplib_navn();
 * ----------------------------------------------------------------------------
 */
const char* GDPL_kontroller_gdplib_navn()
{
    return gdpl_kontroller_gdplib_navn;
}


/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  const char* GDPL_kontroller_gdplib_versjon()
 *
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *   Returnerer versjonen til biblioteket, f.eks '1.0'.
 *
 * Parametre
 *   nop
 *
 * Returnerer
 *   En streng som representerer versjonen til biblioteket.
 *
 * Eksempel på bruk
 *   const char* ver = GDPL_kontroller_gdplib_versjon();
 * ----------------------------------------------------------------------------
 */
const char* GDPL_kontroller_gdplib_versjon()
{
    return gdpl_kontroller_gdplib_versjon;
}



/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_kontroller_hent_par_nummer(int *par_nummer, int type)
 *
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *   Trekk et tilfeldig valgt par-nummer, blant de som er
 *   ledige i settet [1, gdpl_max_antall_par]. Om gdpl_max_antall_par == -1
 *   skal det returneres ei feilmelding. Man må angi som et parameter til
 *   funksjonen, om man ønsker å trekke et par-nummer for ei dame, eller en
 *   herre.
 *
 * Parametre
 *   par_nummer - peker til int hvor det trukne par-nummeret skal plasseres.
 *   type - 0 herrer, 1 damer.
 *
 * Returnerer
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *
 * Eksempel på bruk
 *
 * ----------------------------------------------------------------------------
 */
int GDPL_kontroller_hent_par_nummer(int *par_id, int type) {

    const char* signatur = "GDPL_kontroller_hent_par_nummer(int*, int)";

    /* Først litt sjekking. */
    
    assert (gdpl_kontroller_max_antall_par != -1);
    assert (type == 0 || type == 1);
    assert (par_id != 0);

    /* Trekk et tilfeldig par-nummer, blant de som er ledige,
     for den angitte typen; damer eller herrer. */

    int tabell[GDPL_MAX_ANTALL_PAR];

    int index = 0;
    for (index = 0; index < GDPL_MAX_ANTALL_PAR; index++) {
        tabell[index] = -1;
    }

    GDPL_konkurranse_data_node *vkp = gdpl_modell_konkurranseliste_valgt_ptr;
    GDPL_par_data_node *runner = (GDPL_par_data_node *)vkp->par_liste_root_ptr;

    assert (runner != 0);
    assert (runner->neste != 0);

    runner = runner->neste;

    /* Finn hvilke par som er ledig, gitt typen. */
    index = 0;
    do {
        GDPL_log(GDPL_DEBUG, signatur, "id=%d herre_person_id=%d dame_person_id=%d start_nr=%d",
                 runner->id, runner->herre_person_id,	runner->dame_person_id,	runner->start_nr);

        if (type == 0 && runner->herre_person_id == 0) { /* Ledig herre plass. */
            tabell[index++] = runner->id;
        } else if (type == 1 && runner->dame_person_id == 0) { /* Ledig dame plass. */
            tabell[index++] = runner->id;
        }

        runner = runner->neste;
    } while (runner != 0);

    /* Ok, nå har vi oversikt over ledige par. */
    index = 0;
    while (index < GDPL_MAX_ANTALL_PAR) {

        if (tabell[index] == -1)
            break;

        GDPL_log(GDPL_DEBUG, signatur, "ledig par id=%d (for type = %s)",
                 tabell[index], type==0?"herre":"dame");
        index++;
    }

    /* Velg et tilfeldig par i settet. */
    int seed;
    seed = time(NULL);
    srand(seed);
    *par_id = tabell[rand() % index];

    GDPL_log(GDPL_DEBUG, signatur, "Par-node med id=%d er ledig for %s -registrering.",*par_id, type == 0?"herre":"dame");
    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}


/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_kontroller_angi_max_antall_par(int antall)
 *
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *
 * Parametre
 *
 * Returnerer
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *
 * Eksempel på bruk
 *
 * ----------------------------------------------------------------------------
 */
int GDPL_kontroller_angi_max_antall_par(int antall)
{
    const char* signatur = "GDPL_kontroller_angi_max_antall_par(int)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (antall < 1) {
        int feilkode = FEILKODE_MAX_ANTALL_PAR_FOR_LAVT;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;
    }

    if (antall > GDPL_MAX_ANTALL_PAR) {
        int feilkode = FEILKODE_MAX_ANTALL_PAR_FOR_STORT;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;
    }

    gdpl_kontroller_max_antall_par = antall;

    GDPL_log(GDPL_DEBUG, signatur, "Max antall par er nå satt til '%d'", gdpl_kontroller_max_antall_par);
    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_kontroller_hent_max_antall_par(int *antall)
 *
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *
 * Parametre
 *
 * Returnerer
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *
 * Eksempel på bruk
 *
 * ----------------------------------------------------------------------------
 */
int GDPL_kontroller_hent_max_antall_par(int *antall)
{
    const char* signatur = "GDPL_kontroller_hent_max_antall_par(int)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (antall == 0) {
        int feilkode = FEILKODE_FEIL;
        GDPL_log(GDPL_DEBUG, signatur, "antall == 0, i.e. pekeren er ikke initiert!");
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;
    }

    *antall = gdpl_kontroller_max_antall_par;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}
















