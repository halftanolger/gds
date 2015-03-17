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

    GDPL_modell_dump();

    /* Precondition:  Vi skal ha en konkurranse med id = 1 */

    if (GDPL_konkurranse_sett_valgt_konkurranse(1) > 0) {
        return FEILKODE_FEIL;
    }


    /* Precondition:  Vi skal ha et par id = 1 */

    GDPL_par_data_node* parA = 0;
    if (GDPL_par_hent(1, &parA) > 0) {
        return FEILKODE_FEIL;
    }

    /* Precondition:  Vi skal ha et par id = 2 */

    GDPL_par_data_node* parB = 0;
    if (GDPL_par_hent(2, &parB) > 0) {
        return FEILKODE_FEIL;
    }

    strcpy(parA->start_tid,"12:00:00");
    strcpy(parB->start_tid,"12:01:00");

    strcpy(parA->maal_tid,"14:10:00");
    strcpy(parB->maal_tid,"14:25:30");

    parA->oppgave_poeng = 20;
    parB->oppgave_poeng = 30;

    GDPL_modell_dump();

    GDPL_log(GDPL_DEBUG, signatur, "ok så langt");

    /*
    if (GDPL_modell_skriv_data() > 0) {
        return FEILKODE_FEIL;
    }*/




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

        GDPL_konkurranse_data_node *k = 0;
        if (GDPL_konkurranse_opprett_node(&k)>0)
            return FEILKODE_FEIL;

        GDPL_person_data_node* person = 0;
        if (GDPL_person_opprett_node(&person)>0)
            return FEILKODE_FEIL;

        GDPL_par_data_node* par = 0;
        if (GDPL_par_opprett_node(&par)>0)
            return FEILKODE_FEIL;

        k->id = teller;
        k->aar = 2009 + teller;
        k->person_liste_root_ptr = person;
        k->par_liste_root_ptr = par;

        if (GDPL_konkurranse_legg_til(k) > 0)
            return FEILKODE_FEIL;        
    }

    GDPL_modell_dump();

    if (GDPL_konkurranse_sett_valgt_konkurranse(1)>0)
        return FEILKODE_FEIL;

    GDPL_person_data_node *person = 0;
    if(GDPL_person_opprett_node(&person)>0)
        return FEILKODE_FEIL;

    int ny_id = 0;
    if(GDPL_person_finn_neste_ledige_id(&ny_id)>0)
        return FEILKODE_FEIL;

    person->id = ny_id;
    strcpy(person->fnavn,"Ola1");
    strcpy(person->enavn,"Nordmann1");
    if (GDPL_person_legg_til(person) > 0)
        return FEILKODE_FEIL;

    person = 0;
    GDPL_person_opprett_node(&person);
    ny_id = 0;
    if(GDPL_person_finn_neste_ledige_id(&ny_id)>0)
        return FEILKODE_FEIL;

    person->id = ny_id;
    strcpy(person->fnavn,"Kari1");
    strcpy(person->enavn,"Nordkvinne1");
    if (GDPL_person_legg_til(person) > 0)
        return FEILKODE_FEIL;

    person = 0;
    GDPL_person_opprett_node(&person);
    ny_id = 0;
    if(GDPL_person_finn_neste_ledige_id(&ny_id)>0)
        return FEILKODE_FEIL;

    person->id = ny_id;
    strcpy(person->fnavn,"Ola2");
    strcpy(person->enavn,"Nordmann2");
    if (GDPL_person_legg_til(person) > 0)
        return FEILKODE_FEIL;

    person = 0;
    GDPL_person_opprett_node(&person);
    ny_id = 0;
    if(GDPL_person_finn_neste_ledige_id(&ny_id)>0)
        return FEILKODE_FEIL;

    person->id = ny_id;
    strcpy(person->fnavn,"Kari2");
    strcpy(person->enavn,"Nordkvinne2");
    if (GDPL_person_legg_til(person) > 0)
        return FEILKODE_FEIL;




    GDPL_person_data_node *ptrPerson = 0;
    if (GDPL_person_hent(1, &ptrPerson)>0)
        return FEILKODE_FEIL;


    if (strcmp(ptrPerson->fnavn,"Ola1") != 0) {
        GDPL_log(GDPL_ERROR, signatur, "strcmp(ptrPerson->fnavn,Ola1) != 0");
        return FEILKODE_FEIL;
    }


    /* Opprett noen par */


    GDPL_par_data_node *par = 0;
    GDPL_par_opprett_node(&par);
    ny_id = 0;
    if(GDPL_par_finn_neste_ledige_id(&ny_id)>0) {
        return FEILKODE_FEIL;
    }
    par->id = ny_id;
    par->herre_person_id = 1;
    par->dame_person_id = 4;
    par->start_nr=100;
    if (GDPL_par_legg_til(par) > 0) {
        return FEILKODE_FEIL;
    }
    par = 0;
    GDPL_par_opprett_node(&par);
    ny_id = 0;
    if(GDPL_par_finn_neste_ledige_id(&ny_id)>0) {
        return FEILKODE_FEIL;
    }
    par->id = ny_id;
    par->herre_person_id = 2;
    par->dame_person_id = 3;
    par->start_nr=101;
    if (GDPL_par_legg_til(par) > 0) {
        return FEILKODE_FEIL;
    }

    GDPL_modell_dump();

    if (GDPL_modell_skriv_data() > 0)
        return FEILKODE_FEIL;


    int antall = 0;
    GDPL_konkurranse_antall_i_liste(&antall);
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
    if (GDPL_konkurranse_antall_i_liste(&antall) > 0)
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




