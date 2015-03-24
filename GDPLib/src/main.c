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


#include <stdio.h>  /* printf */
#include <string.h> /* strcmp */

#include "gdpl.h"
#include "main.h"

int main (int argc, char *argv[])
{           
    if (argc > 1) {
        if (strcmp(argv[1],"-v")==0) {
            print_intro();
            return 0;
        }
        if (strcmp(argv[1],"-h")==0) {
            print_intro();
            printf("%s",info);
            return 0;
        }

        if (strcmp(argv[1],"-i")==0 && argc >= 3) {

            int loglevel = 0;
            if (argc == 5 && strcmp(argv[3],"-l")==0 && strcmp(argv[4],"debug")==0) {
                loglevel = 1;
            }

            char *filnavn = argv[2];
            int r = filversjon(filnavn,loglevel);
            return r;
        }        
    }    
    print_intro();
    printf("Feil: mangler input.\n\nEksempel på bruk:\n\n");
    printf(" %s -i inputfil.cvs\n",argv[0]);
    printf(" %s -v \n",argv[0]);
    printf(" %s -h \n",argv[0]);
    return 0;
}

void print_intro()
{
    const char *navn = GDPL_kontroller_gdplib_navn();
    const char *ver =  GDPL_kontroller_gdplib_versjon();
    printf("Gubberenn Dataprogram, %s %s\n", navn, ver);
}

int filversjon(char* inputfil, int loglevel)
{
    const char* signatur = "filversjon";
    if (loglevel == 0)
        GDPL_log_init(GDPL_ERROR, stdout);
    else
        GDPL_log_init(GDPL_DEBUG, stdout);

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

    FILE *file = fopen(inputfil,"r");

    if (file == 0) {
        GDPL_log(GDPL_ERROR,"Klarer ikke å åpne fila %s", inputfil);
        return FEILKODE_FEIL;
    }


    printf("TODO: les inn data fra csv.fil");

    /* Første linje er bare kolonnenavn. */

    /*
    std::vector<std::string> v = getNextLineAndSplitIntoTokens(file);

    bool loop = true;
    do {

        v = getNextLineAndSplitIntoTokens(file);
        if (v.size() == 0) {
            loop = false;
        } else {

            int antall = v.size();

            // Antall kolonner skal være 8 /

            if (antall != 8) {
                GDPL_log(GDPL_ERROR, signatur, "Feil antall kolonner i cvs-input-fil. Er %d, skal være 8.",antall);
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

            std::string oppgave_poeng_str = v.at(7);
            int oppgave_poeng = atoi(oppgave_poeng_str.c_str());

            / Legg til herre-person /

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

            / Legg til dame-person /

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

            / Opprett par /

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

    std::string fn(filnavn);
    fn += "_output.csv";

    std::ofstream myfile;
    myfile.open(fn);

    if (!myfile.is_open()) {
        GDPL_log(GDPL_ERROR,"Klarer ikke aa aapne fila %s", fn.c_str());
        return FEILKODE_FEIL;
    }

    myfile << "Plassering" << ";"
           << "Startnummer" << ";"
           << "Herre-navn" << ";"
           << "Dame-navn" << ";"
           << "Start-tid" << ";"
           << "Slutt-tid" << ";"
           << "Oppgave-poeng" << ";"
           << "Beregnet middel-tid" << ";"
           << "Beregnet anvendt-tid" << ";"
           << "Beregnet avveket-tid" << ";"
           << "Beregnet tids-poeng" << ";"
           << "Beregnet total-poeng" << ";\n";

    struct GDPL_tid middel_tid;
    if (GDPL_par_beregn_middel_tid(&middel_tid)>0)
        return FEILKODE_FEIL;

    for (int i=1; i<=antall_par; i++) {
        GDPL_par_data_node *data = 0;
        if( GDPL_par_hent_i_rekke(i, &data)>0) {
            myfile.close();
            return 1;
        }

        GDPL_person_data_node *hperson = 0;
        if (GDPL_person_hent(data->herre_person_id, &hperson)>0)
            return 1;

        GDPL_person_data_node *dperson = 0;
        if (GDPL_person_hent(data->dame_person_id, &dperson)>0)
            return 1;

        struct GDPL_tid avveket_tid;
        if (GDPL_par_beregn_avvik(&avveket_tid, middel_tid, data->anvendt_tid)>0)
            return 1;

        myfile << i << ";"
               << data->start_nr << ";"
               << hperson->fnavn << " "
               << hperson->enavn << ";"
               << dperson->fnavn << " "
               << dperson->enavn << ";"
               << data->start_tid.timer << ":" << data->start_tid.minutt << ":" << data->start_tid.sekund << ";"
               << data->maal_tid.timer << ":" << data->maal_tid.minutt << ":" << data->maal_tid.sekund << ";"
               << data->oppgave_poeng << ";"
               << middel_tid.timer << ":" << middel_tid.minutt << ":" << middel_tid.sekund << ";"
               << data->anvendt_tid.timer << ":" << data->anvendt_tid.minutt << ":" << data->anvendt_tid.sekund << ";"
               << avveket_tid.timer << ":" << avveket_tid.minutt << ":" << avveket_tid.sekund << ";"
               << data->tids_poeng << ";"
               << (data->tids_poeng + data->oppgave_poeng) << ";\n";

    }

    myfile.close();

    */
    return 0;
}
