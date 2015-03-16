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
#include "konkurranse.h"
#include "person.h"
#include "par.h"



int GDPL_test_modell_privat_alle_funksjoner_eksisterende_data()
{
    const char* signatur = "GDPL_test_modell_privat_alle_funksjoner_eksisterende_data()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    assert ( gdpl_modell_konkurranseliste_root_ptr != 0 );

    /* Precondition er at vi skal ha en node med id lik 1 her ...*/

    int id = 1;
    GDPL_konkurranse_data_node *data = 0;

    if (GDPL_konkurranse_hent(id, &data, gdpl_modell_konkurranseliste_root_ptr)>0) {
        return FEILKODE_FEIL;
    }

    GDPL_log(GDPL_DEBUG, signatur, "data->id=%d",data->id);
    GDPL_log(GDPL_DEBUG, signatur, "data->aar=%d",data->aar);


    gdpl_modell_konkurranseliste_valgt_ptr = data;

    GDPL_person_data_node *person;
    GDPL_person_opprett_node(&person);

    int ny_id = 0;
    GDPL_person_finn_neste_ledige_id(&ny_id,data->par_liste_root_ptr);

    person->id = ny_id;
    person->fnavn = strdup("Ola");  <- her må jeg bruke realloc() stuff ...
    person->enavn = strdup("Nordmann");

    if (GDPL_person_legg_til(person, data->par_liste_root_ptr) > 0) {
        return FEILKODE_FEIL;
    }


    if (GDPL_modell_skriv_data() > 0)
        return FEILKODE_FEIL;


    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}

int GDPL_test_modell_privat_alle_funksjoner_ny_data()
{
    const char* signatur = "GDPL_test_modell_privat_alle_funksjoner_ny_data()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    /* Opprett en ny node. */

    /* Etter at vi har lest inn data så skal vi i alle fall ha grunnstrukturen
     * på plass. */

    assert ( gdpl_modell_konkurranseliste_root_ptr != 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->neste == 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->person_liste_root_ptr != 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->person_liste_root_ptr->neste == 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->par_liste_root_ptr != 0 );
    assert ( gdpl_modell_konkurranseliste_root_ptr->par_liste_root_ptr->neste == 0 );

    int teller = 1;
    for (teller=1; teller<2; teller++) {

        GDPL_konkurranse_data_node *k;
        GDPL_konkurranse_opprett_node(&k);

        GDPL_person_data_node* person;
        GDPL_person_opprett_node(&person);

        GDPL_par_data_node* par;
        GDPL_par_opprett_node(&par);

        k->id = teller;
        k->aar = 2009 + teller;
        k->person_liste_root_ptr = person;
        k->par_liste_root_ptr = par;

        if (GDPL_konkurranse_legg_til(k,gdpl_modell_konkurranseliste_root_ptr) > 0) {
            return FEILKODE_FEIL;
        }
    }

    if (GDPL_modell_skriv_data() > 0)
        return FEILKODE_FEIL;


    int antall = 0;
    GDPL_konkurranse_antall_i_liste(&antall,gdpl_modell_konkurranseliste_root_ptr);
    GDPL_log(GDPL_DEBUG, signatur, "antall konkurranser = %d",antall);

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon");
    return 0;

}


int GDPL_test_modell_alle_funksjoner()
{
    const char* signatur = "GDPL_test_modell_alle_funksjoner()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    /* Angi default filnavn. */

    if (GDPL_modell_angi_filnavn(0) > 0)
        return FEILKODE_FEIL;

    /* Les inn data. */

    if (GDPL_modell_les_data() > 0)
        return FEILKODE_FEIL;

    /* Her kan vi har to situasjoner; enten ei tom datafil, eller ei
     * datafil som inneholder data. */

    int antall = 0;
    if (GDPL_konkurranse_antall_i_liste(&antall,gdpl_modell_konkurranseliste_root_ptr) > 0)
        return FEILKODE_FEIL;

    if (antall > 0) {
        if (GDPL_test_modell_privat_alle_funksjoner_eksisterende_data()>0) {
            return FEILKODE_FEIL;
        }
    } else {
        if (GDPL_test_modell_privat_alle_funksjoner_ny_data()>0) {
            return FEILKODE_FEIL;
        }
    }


    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon");
    return 0;
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


