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

#include <QtWidgets>
#include <QDebug>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "gdpl.h"



std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str,line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while(std::getline(lineStream,cell,';'))
    {
        result.push_back(cell);
    }
    return result;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

int main(int argc, char **argv)
{  
    const char* signatur = "main()";

    GDPL_init_log(GDPL_INFO, stdout);

    const char *navn = GDPL_kontroller_gdplib_navn();
    const char *ver =  GDPL_kontroller_gdplib_versjon();
    GDPL_log(GDPL_INFO, signatur, "%s %s", navn, ver);

    if (GDPL_modell_angi_filnavn(0) > 0)
        return 1;

    if (GDPL_modell_les_data() > 0)
        return 1;

    int antall = 0;
    if (GDPL_konkurranse_antall_i_liste(&antall) > 0)
        return 1;

    if (antall > 0) {
        if (GDPL_modell_nullstill()>0) {
            return 1;
        }
    }

    /* Her skal vi ha 'blank' modell. */

    GDPL_modell_dump();

    /* Legg til en ny konkurranse. */

    GDPL_konkurranse_data_node *k = 0;
    if (GDPL_konkurranse_opprett_node(&k)>0)
        return FEILKODE_FEIL;

    GDPL_person_data_node* person = 0;
    if (GDPL_person_opprett_node(&person)>0)
        return FEILKODE_FEIL;

    GDPL_par_data_node* par = 0;
    if (GDPL_par_opprett_node(&par)>0)
        return FEILKODE_FEIL;

    k->id = 1;
    k->aar = 2015;
    k->person_liste_root_ptr = person;
    k->par_liste_root_ptr = par;

    if (GDPL_konkurranse_legg_til(k) > 0)
        return FEILKODE_FEIL;

    /* Velg denne konkurransen */

    if (GDPL_konkurranse_sett_valgt_konkurranse(1)>0)
        return FEILKODE_FEIL;

    /* Last inn data fra cvs-fil. */

    // TODO: bruk arg ...
    std::ifstream file("testinput.csv");

    /* Første linje er bare kolonnenavn. */

    std::vector<std::string> v = getNextLineAndSplitIntoTokens(file);

    bool loop = true;
    do {

        v = getNextLineAndSplitIntoTokens(file);
        if (v.size() == 0) {
            loop = false;
        } else {

            int antall = v.size();

            /* Antall kolonner må være 11 */

            if (antall != 11) {
                GDPL_log(GDPL_ERROR, signatur, "Feil format på cvs-input-fil.");
            }

            std::string  start_nr_str = v.at(0);
            int start_nr = atoi(start_nr_str.c_str());

            std::string hfnavn_str = v.at(1);
            const char *hfnavn = hfnavn_str.c_str();

            std::string henavn_str = v.at(2);
            const char *henavn = henavn_str.c_str();

            std::string dfnavn_str = v.at(3);
            const char *dfnavn = dfnavn_str.c_str();

            std::string denavn_str = v.at(4);
            const char *denavn = denavn_str.c_str();

            std::string start_tid_str = v.at(5);
            std::vector<std::string> start_tid_elementer = split(start_tid_str, ':');
            int start_tid_t = atoi(start_tid_elementer.at(0).c_str());
            int start_tid_m = atoi(start_tid_elementer.at(1).c_str());
            int start_tid_s = atoi(start_tid_elementer.at(2).c_str());

            std::string maal_tid_str = v.at(6);
            std::vector<std::string> maal_tid_elementer = split(maal_tid_str, ':');
            int maal_tid_t = atoi(maal_tid_elementer.at(0).c_str());
            int maal_tid_m = atoi(maal_tid_elementer.at(1).c_str());
            int maal_tid_s = atoi(maal_tid_elementer.at(2).c_str());

            std::string oppgave_poeng_str = v.at(10);
            int oppgave_poeng = atoi(oppgave_poeng_str.c_str());

            /* Legg til herre-person */

            GDPL_person_data_node *person = 0;
            if(GDPL_person_opprett_node(&person)>0)
                return 1;

            int ny_id_h = 0;
            if(GDPL_person_finn_neste_ledige_id(&ny_id_h)>0)
                return 1;

            person->id = ny_id_h;
            strcpy(person->fnavn,hfnavn);
            strcpy(person->enavn,henavn);
            if (GDPL_person_legg_til(person) > 0)
                return 1;

            /* Legg til dame-person */

            person = 0;
            GDPL_person_opprett_node(&person);
            int ny_id_d = 0;
            if(GDPL_person_finn_neste_ledige_id(&ny_id_d)>0)
                return 1;

            person->id = ny_id_d;
            strcpy(person->fnavn,dfnavn);
            strcpy(person->enavn,denavn);
            if (GDPL_person_legg_til(person) > 0)
                return 1;

            /* Opprett par */

            GDPL_par_data_node *par = 0;
            GDPL_par_opprett_node(&par);
            int ny_id = 0;
            if(GDPL_par_finn_neste_ledige_id(&ny_id)>0)
                return 1;

            par->id = ny_id;
            par->herre_person_id = ny_id_h;
            par->dame_person_id = ny_id_d;
            par->start_nr=start_nr;
            par->start_tid.timer = start_tid_t;
            par->start_tid.minutt = start_tid_m;
            par->start_tid.sekund = start_tid_s;
            par->maal_tid.timer = maal_tid_t;
            par->maal_tid.minutt = maal_tid_m;
            par->maal_tid.sekund = maal_tid_s;
            par->oppgave_poeng = oppgave_poeng;

            if (GDPL_par_legg_til(par) > 0)
                return 1;

        }

    } while (loop);

    if (GDPL_par_beregn()>0)
        return 1;

    GDPL_modell_dump();

    int antall_par = 0;
    if (GDPL_par_antall_i_liste(&antall_par)>0)
        return 1;

    for (int i=1; i<=antall_par; i++) {
        GDPL_par_data_node *data = 0;
        if( GDPL_par_hent_i_rekke(i, &data)>0)
            return 1;

        //TODO: skriv til fil.
        std::cout << "id=" << data->id << "poeng=" << (data->tids_poeng + data->oppgave_poeng) << std::endl;


    }

    return 0;


    /*
    QApplication app(argc, argv);
    GDPMainWindows* appWin = new GDPMainWindows();
    appWin->show();
    appWin->setFocus();
    int r = app.exec();
    return r;
    */
}


