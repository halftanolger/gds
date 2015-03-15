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
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "test_modell.h"
#include "diverse.h"
#include "modell.h"


int GDPL_test_modell_alle_funksjoner()
{
    const char* signatur = "GDPL_test_modell_alle_funksjoner()";
    int feilkode = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    /* Klargjør for testtilfellet. */

    int tilfeldig_tall, seed;
    char tilfeldig_filnavn[128];
    seed = time(NULL);
    srand(seed);
    tilfeldig_tall = rand();
    sprintf(tilfeldig_filnavn,"%d_tempfil.dat",tilfeldig_tall);
    GDPL_log(GDPL_DEBUG, signatur, "Opprettet temoprært filnavn: %s", tilfeldig_filnavn);

    debug hva som skjer her ...

    GDPL_modell_angi_filnavn(tilfeldig_filnavn);
    GDPL_modell_les_data();

    assert ( gdpl_modell_konkurranseliste_root_ptr != 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->neste == 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->person_liste_root_ptr != 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->person_liste_root_ptr->neste == 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->par_liste_root_ptr != 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->par_liste_root_ptr->neste == 0 );


    GDPL_konkurranse_data_node *k;
    GDPL_konkurranse_opprett_node(&k);
    k->id = 1;
    k->aar = 2015;
    GDPL_konkurranse_legg_til(k,gdpl_modell_konkurranseliste_root_ptr);


    GDPL_modell_skriv_data();


    FILE* file = fopen(tilfeldig_filnavn,"rb");
    GDPL_modell_privat_les_inn_fra_eksisterende_fil(file);







    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon, feilkode=%d", feilkode);
    return feilkode;
}


int GDPL_test_modell_angi_filnavn(const char *filnavn)
{
    const char* signatur = "GDPL_test_modell_angi_filnavn()";
    int feilkode = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");


    GDPL_log(GDPL_DEBUG, signatur, "TODO: alt ..");
    feilkode = 1;


    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon, feilkode=%d", feilkode);
    return feilkode;
}


int GDPL_test_modell_les_data()
{
    const char* signatur = "GDPL_test_modell_les_data()";
    int feilkode = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    GDPL_log(GDPL_DEBUG, signatur, "TODO: alt ..");
    feilkode = 1;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon, feilkode=%d", feilkode);
    return feilkode;
}


int GDPL_test_modell_skriv_data()
{
    const char* signatur = "GDPL_test_modell_skriv_data()";
    int feilkode = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    GDPL_log(GDPL_DEBUG, signatur, "TODO: alt ..");
    feilkode = 1;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon, feilkode=%d", feilkode);
    return feilkode;
}


