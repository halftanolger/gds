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

#include "gdpl.h"
#include "main.h"

float rund_av(float v) {return ((int)(v * 100 + .5) / 100.0);}


int main (int argc, char *argv[])
{               
    int i;
    char *inputfilnavn;
    char *outputfilnavn;
    const char *navn;
    const char *ver;
    int inputfilok;
    int outputfilok;
    FILE *innputfp;
    FILE *outputfp;

    inputfilok = 0;
    outputfilok = 0;

    gdpl_log_stream = stderr ;
    gdpl_log_level = LOG_ERR;

    for ( i = 0; i < argc; i++ ) {
        if ( strcmp ( argv[i], "-l" ) == 0 ) {
            if ( i+1 < argc ) {
                if ( strcmp ( argv[i+1], "debug" ) == 0 ) {
                    gdpl_log_level = LOG_DBG;
                } else if ( strcmp ( argv[i+1], "error" ) == 0 ) {
                    gdpl_log_level = LOG_ERR;
                } else {
                	printf ( "-l kan enten være 'debug' eller 'error'" );
                	return 1;
                } 
            }
        }
    }

    navn = GDPL_kontroller_gdplib_navn();
    ver =  GDPL_kontroller_gdplib_versjon();
    LOG ( LOG_DBG, "%s %s", navn, ver );

    if ( argc == 2 ) {
        if ( strcmp ( argv[1], "-v" ) == 0 ) {
            fprintf ( stdout, "Gubberenn Dataprogram, %s %s\n", navn, ver );
            return 0;
        }
    }

    if ( argc == 2 ) {
        if ( strcmp ( argv[1], "-h" ) == 0 ) {
            fprintf ( stdout, "Gubberenn Dataprogram, %s %s\n", navn, ver );
            fprintf ( stdout, "%s\n", info);
            return 0;
        }
    }

    /* Vi må ha ei inputfil og ei outputfil for å komme videre. */

    for ( i = 0; i < argc; i++ ) {
        if ( strcmp ( argv[i], "-i" ) == 0 ) {
            if ( i+1 < argc ) {
                inputfilnavn = argv[i+1];
                errno = 0;
                innputfp = fopen ( inputfilnavn, "r" );
                if ( innputfp == NULL ) {
                    LOG(LOG_ERR, "I/O error '%s'' occured while opening file %s", strerror(errno), inputfilnavn);
                    return 1;
                }
                LOG ( LOG_DBG, "Inputfil ok");
                inputfilok = 1;
            } else {
                LOG ( LOG_ERR, "Oppgi et input-filnavn.");
                return 1;
            }
        }
    }

    for ( i = 0; i < argc; i++ ) {
        if ( strcmp ( argv[i], "-o" ) == 0 ) {
            if ( i+1 < argc ) {
                outputfilnavn = argv[i+1];
                errno = 0;
                outputfp = fopen ( outputfilnavn, "w" );
                if ( outputfp == NULL ) {
                    LOG ( LOG_ERR, "I/O error '%s'' occured while opening file %s", strerror(errno), outputfilnavn );
                    return 1;
                }
                LOG ( LOG_DBG, "Outputfil ok");
                outputfilok = 1;
            } else {
                LOG ( LOG_ERR, "Oppgi et output-filnavn.");
                return 1;
            }
        }
    }

    if ( inputfilok == 0 || outputfilok == 0 ) {
        printf("\n\ngds mangler input, bruk:\n\n\tgds -i <inputfilnavn> -o <outputfilnavn> [-l error|debug] [-v] [-h]\n\nhvor\n\t-i navn på inputfil\n\t-o navn på outputfil\n\t-l loggnivå, enten 'debug' eller 'error'. 'error' er default valgt.\n\n");
    	return 1;
    } 

    return filversjon(inputfilnavn, outputfilnavn);
}



int filversjon(char *inputfil, char *outputfil)
{   
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

    printf("%s %s\n", GDPL_kontroller_gdplib_navn(),GDPL_kontroller_gdplib_versjon());

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

        char *hfnavn = items[1];
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

    errno = 0;
    fp = fopen(outputfil,"w");
    if (fp == 0) {
        fprintf (stderr, "gdp: Klarte ikke å åpne output-fila %s; %s\n",
                 outputfil, strerror (errno));
        exit (EXIT_FAILURE);
    }

    fprintf(fp,"Plassering;Startnummer;Herre-navn;Dame-navn;Start-tid;Slutt-tid;Oppgave-poeng;Beregnet middel-tid;Beregnet anvendt-tid;Beregnet avveket-tid;Beregnet tids-poeng;Beregnet total-poeng;\n");

    struct GDPL_tid middel_tid;
    if (GDPL_par_beregn_middel_tid(&middel_tid)>0)
        return FEILKODE_FEIL;

    printf("\n\n == DEASE look alike RESULTATLISTE == \n\n");

    int i;
    for (i=1; i<=antall_par; i++) {
        GDPL_par_data_node *data = 0;
        if( GDPL_par_hent_i_rekke(i, &data)>0) {
            fclose(fp);
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


        fprintf(fp,"%d;%d;%s %s;%s %s;%02d:%02d:%02d;%02d:%02d:%02d;%02.02f;%02d:%02d:%02d;%02d:%02d:%02d;%02d:%02d:%02d;%02.02f;%02.02f;\n",
                i,
                data->start_nr,
                hperson->fnavn, hperson->enavn,
                dperson->fnavn, dperson->enavn,
                data->start_tid.timer,data->start_tid.minutt,data->start_tid.sekund,
                data->maal_tid.timer,data->maal_tid.minutt,data->maal_tid.sekund,
                data->oppgave_poeng,
                middel_tid.timer,middel_tid.minutt,middel_tid.sekund,
                data->anvendt_tid.timer,data->anvendt_tid.minutt,data->anvendt_tid.sekund,
                avveket_tid.timer,avveket_tid.minutt,avveket_tid.sekund,
                rund_av(60.0 - data->tids_poeng),
                rund_av((60.0 - data->tids_poeng) + data->oppgave_poeng));


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

    printf("\n\n == TheEnd == \n\n");

    fclose(fp);
    return 0;
}
