﻿/* 
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


#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    printf("\nFeil: mangler input.\n\nEksempel på bruk:\n\n");
    printf(" %s -i inputfil.cvs\n",argv[0]);
    printf(" %s -i inputfil.cvs -l debug\n",argv[0]);
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
        GDPL_log_init(GDPL_ERROR, stderr);
    else
        GDPL_log_init(GDPL_DEBUG, stderr);

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

    errno = 0;
    FILE *fp = fopen(inputfil,"r");
    if (fp == 0) {
        fprintf (stderr, "%s: Klarte ikke å åpne input-fila %s; %s\n",
                 program_invocation_short_name, inputfil, strerror (errno));
        exit (EXIT_FAILURE);
    }

    int index = 0;
    int linjeteller = -1;
    char *pch;
    char *items[8];
    char line[512];
    while ( fgets( line, sizeof(line), fp ) ) {
        index = 0;
        pch = strtok(line, ";");
        while(pch != NULL) {
            items[index++]=pch;
            pch = strtok(NULL, ";");
        }

        linjeteller++;
        if (linjeteller == 0) {
            continue;
        }

        int start_nr = atoi(items[0]);

        char hfnavn[128];
        strcpy(hfnavn,items[1]);

        //char *hfnavn = items[1];
        char *henavn = items[2];
        char *dfnavn = items[3];
        char *denavn = items[4];
        float oppgavepoeng = atof(items[7]);

        char *st;
        st = strtok(items[5],":");
        int start_tid_t = atoi(st);
        st = strtok(NULL,":");
        int start_tid_m = atoi(st);
        st = strtok(NULL,":");
        int start_tid_s = atoi(st);

        st = strtok(items[6],":");
        int maal_tid_t = atoi(st);
        st = strtok(NULL,":");
        int maal_tid_m = atoi(st);
        st = strtok(NULL,":");
        int maal_tid_s = atoi(st);


        /* TODO: skriv dette ut vha GDPL_log og sscan stuff ... */

        printf("\n-Input linje %d-------------------------\n",linjeteller);
        printf("startnr=%d\n",start_nr);
        printf("hfnavn=%s\n",hfnavn);
        printf("henavn=%s\n",henavn);
        printf("dfnavn=%s\n",dfnavn);
        printf("denavn=%s\n",denavn);
        printf("starttid=%02d:%02d:%02d\n",start_tid_t,start_tid_m,start_tid_s);
        printf("måltid=%02d:%02d:%02d\n",maal_tid_t,maal_tid_m,maal_tid_s);
        printf("oppgavepoeng=%2.2f\n",oppgavepoeng);


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
        par->oppgave_poeng = oppgavepoeng;

        if (GDPL_par_legg_til(par) > 0)
            return 1;

    }

    fclose(fp);


    if (GDPL_par_beregn()>0)
        return 1;

    GDPL_modell_dump();

    int antall_par = 0;
    if (GDPL_par_antall_i_liste(&antall_par)>0)
        return 1;

    /*
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

           */

    struct GDPL_tid middel_tid;
    if (GDPL_par_beregn_middel_tid(&middel_tid)>0)
        return FEILKODE_FEIL;

    int i;
    for (i=1; i<=antall_par; i++) {
        GDPL_par_data_node *data = 0;
        if( GDPL_par_hent_i_rekke(i, &data)>0) {
            //myfile.close();
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

        /*
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
*/

        printf("\n\n-- Resultat %d ------------------------------------------------------\n",i);
        printf("Startnr     : %d\n",data->start_nr);
        printf("Navn        : %s %s, %s %s\n",dperson->fnavn, dperson->enavn, hperson->fnavn, hperson->enavn);
        printf("Starttid    : %02d:%02d:%02d\n",data->start_tid.timer,data->start_tid.minutt,data->start_tid.sekund);
        printf("Måltid      : %02d:%02d:%02d\n",data->maal_tid.timer,data->maal_tid.minutt,data->maal_tid.sekund);
        printf("Anvendt tid : %02d:%02d:%02d\n",data->anvendt_tid.timer,data->anvendt_tid.minutt,data->anvendt_tid.sekund);
        printf("Idealtid    : %02d:%02d:%02d\n",middel_tid.timer,middel_tid.minutt,middel_tid.sekund);
        printf("Tidspoeng   : %2.2f\n", data->tids_poeng);
        printf("Oppgavepoeng: %2.2f\n", data->oppgave_poeng);

        double t = data->tids_poeng + data->oppgave_poeng;
        t = ((int)(t * 100 + .5) / 100.0); /* Rund av til to desimaler. */
        printf("Totalt      : %2.2f\n", t);

    }

    //myfile.close();


    return 0;
}
