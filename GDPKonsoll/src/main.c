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


#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h> /* setlocale, LC_ALL */

#include "gdpl.h"
#include "main.h"
#include "util.h"

float rund_av(float v) {return ((int)(v * 100 + .5) / 100.0);}

int main ( int argc, char *argv[] )
{          

    int returverdi;
    gubb_input_args i;
    
    /* Bruk default lokale, bør fikse øæå-problematikk. */
    setlocale(LC_ALL,"");
    
    /* Pase inputargumenter til gubb-programmet. */
    returverdi = gubb_util_parse_args ( argc, argv, &i );
    
    if ( returverdi == 1 ) {
        printf( "%s", bruk_info );
        return 1;
    }

    if ( i.hjelp_flagg == 1 ) {
        printf( "%s", hjelp_info );
        return 0;
    }

    if ( i.bruksanvisning_flagg == 1 ) {
        printf( "%s", bruksanvisning_info );
        return 0;
    }
    
    if ( i.versjon_flagg == 1 ) {
        printf( "%s %s\n", GDPL_kontroller_gdplib_navn(), GDPL_kontroller_gdplib_versjon() );
        return 0;
    }

    /* todo: sjekk om inputfil eksisterer, og kan leses fra */
    if ( i.input_flagg == 1 ) {
        printf("inputfil %s\n",i.input_argument);
    }
    
    /* todo: sjekk om outputfil kan skrives til */
    if ( i.output_flagg == 1 ) {
        printf("outputfil %s\n",i.output_argument);
    }
    
    /* todo: sjekk om rapportfil kan skrives til */
    if ( i.rapportfil_flagg == 1 ) {
        printf("rapportfil %s\n",i.rapportfil_argument);
    }

    /* todo: inputargumenter skal nå være på plass. Do the job! */

    return 0;
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
        fprintf (stderr, "gdp: Klarte ikke å åpne input-fila %s; %s\n",
                 inputfil, strerror (errno));
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

        printf("\n\n===================================================================\n\n");
        printf("               STARTNR :%.3d\n\n",data->start_nr);
        printf("  PLASS :%.2d    %s %s\n",i,dperson->fnavn, dperson->enavn);
        printf("               %s %s\n\n",hperson->fnavn, hperson->enavn);
        printf("                 START TID : %02d:%02d:%02d         STARTPOENG :    60,00\n",data->start_tid.timer,data->start_tid.minutt,data->start_tid.sekund);
        printf("                 SLUTT TID : %02d:%02d:%02d        - TIDSPOENG :    %02.02f\n",data->maal_tid.timer,data->maal_tid.minutt,data->maal_tid.sekund,data->tids_poeng);
        printf("               ANVENDT TID : %02d:%02d:%02d        = LØPSPOENG :    %02.02f\n",data->anvendt_tid.timer,data->anvendt_tid.minutt,data->anvendt_tid.sekund, rund_av(60.0 - data->tids_poeng));
        printf("              GJ.SNITT TID : %02d:%02d:%02d       + OPPG.POENG :    %02.02f\n",middel_tid.timer,middel_tid.minutt,middel_tid.sekund,data->oppgave_poeng);
        printf("                             --------       = SLUTTPOENG :    %02.02f\n",rund_av((60.0 - data->tids_poeng) + data->oppgave_poeng));
        printf("                                                           ========\n");

    }

    //myfile.close();


    return 0;
}
