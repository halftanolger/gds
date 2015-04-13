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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h> /* setlocale, LC_ALL */

#include "gdpl.h"
#include "main.h"
#include "util.h"
#include "rapport.h"

int main ( int argc, char *argv[] )
{          
    int returverdi;
    gubb_input_args i;

    /* Bruk default lokale, bør fikse øæå-problematikk. */
    setlocale(LC_ALL,"");
    
    /* Pase inputargumenter til gubb-programmet. */
    gubb_util_parse_args ( argc, argv, &i );

    /* Do the job */
    returverdi = 0;
    if ( i.input_flagg == 1 && i.output_flagg == 1 ) {
        returverdi = gubb ( &i );
    }

    /* Rydd opp */
    gubb_util_lukk_eventuelle_aapne_filer ( &i );

    /* TheEnd */
    return returverdi;
}


int gubb(gubb_input_args *input)
{   
    /* Benyttes av log-funksjon */
    char* signatur;
    signatur = "gubb";

    /* Bruk default datafil. */
    if ( GDPL_modell_angi_filnavn ( 0 ) > 0 )
        return 1;

    /* Initier datamodell */
    if ( GDPL_modell_les_data () > 0 )
        return 1;

    /* Om den defaulte datafila inneholder data, skal denne nullstilles */
    int antall = 0;
    if ( GDPL_konkurranse_antall_i_liste(&antall) > 0 )
        return 1;

    if ( antall > 0 ) {
        if ( GDPL_modell_nullstill() > 0 ) {
            return 1;
        }
    }

    /* Her skal vi ha 'blank' modell. */
    GDPL_modell_dump();

    /* Legg til en ny konkurranse. */
    GDPL_konkurranse_data_node *k = 0;
    if ( GDPL_konkurranse_opprett_node ( &k ) > 0 )
        return FEILKODE_FEIL;

    GDPL_person_data_node* person = 0;
    if ( GDPL_person_opprett_node ( &person ) > 0 )
        return FEILKODE_FEIL;

    GDPL_par_data_node* par = 0;
    if ( GDPL_par_opprett_node ( &par ) > 0 )
        return FEILKODE_FEIL;

    k->id = 1;
    k->aar = 2015;
    k->person_liste_root_ptr = person;
    k->par_liste_root_ptr = par;

    if ( GDPL_konkurranse_legg_til ( k ) > 0 )
        return FEILKODE_FEIL;

    /* Velg denne konkurransen */
    if ( GDPL_konkurranse_sett_valgt_konkurranse ( 1 ) > 0 )
        return FEILKODE_FEIL;

    /* Last inn data fra cvs-fil. */
    int index = 0;
    int linjeteller = -1;
    char *pch;
    char *items[8];
    char line[512];
    while ( fgets( line, sizeof ( line ), input->input_stream ) ) {
        index = 0;
        pch = strtok ( line, ";" );
        while( pch != NULL ) {
            items[index++] = pch;
            pch = strtok ( NULL, ";" );
        }

        linjeteller++;
        if ( linjeteller == 0 ) {
            continue;
        }

        int start_nr = atoi ( items[0] );

        char *hfnavn = items[1];
        char *henavn = items[2];
        char *dfnavn = items[3];
        char *denavn = items[4];
        float oppgavepoeng = atof ( items[7] );

        char *st;
        st = strtok( items[5], ":" );
        int start_tid_t = atoi ( st );
        st = strtok( NULL, ":" );
        int start_tid_m = atoi ( st );
        st = strtok( NULL, ":" );
        int start_tid_s = atoi ( st );

        st = strtok( items[6], ":" );
        int maal_tid_t = atoi ( st );
        st = strtok ( NULL, ":" );
        int maal_tid_m = atoi ( st );
        st = strtok ( NULL, ":" );
        int maal_tid_s = atoi ( st );

        GDPL_log ( GDPL_INFO, signatur , "--Inputfil linje %d--", linjeteller );
        GDPL_log ( GDPL_INFO, signatur , "startnr=%d", start_nr );
        GDPL_log ( GDPL_INFO, signatur , "hfnavn=%s", hfnavn );
        GDPL_log ( GDPL_INFO, signatur , "henavn=%s", henavn );
        GDPL_log ( GDPL_INFO, signatur , "dfnavn=%s", dfnavn );
        GDPL_log ( GDPL_INFO, signatur , "denavn=%s", denavn );
        GDPL_log ( GDPL_INFO, signatur , "starttid=%d:%d:%d", start_tid_t, start_tid_m, start_tid_s );
        GDPL_log ( GDPL_INFO, signatur , "måltid=%d:%d:%d", maal_tid_t, maal_tid_m, maal_tid_s );
        GDPL_log ( GDPL_INFO, signatur , "oppgavepoeng=%f", oppgavepoeng );

        /* Legg til herre-person */

        GDPL_person_data_node *person = 0;
        if ( GDPL_person_opprett_node ( &person ) > 0 )
            return 1;

        int ny_id_h = 0;
        if ( GDPL_person_finn_neste_ledige_id ( &ny_id_h ) > 0 )
            return 1;

        person->id = ny_id_h;
        strcpy ( person->fnavn, hfnavn );
        strcpy ( person->enavn, henavn );
        if ( GDPL_person_legg_til ( person ) > 0 )
            return 1;

        /* Legg til dame-person */

        person = 0;
        GDPL_person_opprett_node ( &person );
        int ny_id_d = 0;
        if ( GDPL_person_finn_neste_ledige_id ( &ny_id_d ) > 0 )
            return 1;

        person->id = ny_id_d;
        strcpy ( person->fnavn, dfnavn );
        strcpy ( person->enavn, denavn );
        if ( GDPL_person_legg_til ( person ) > 0 )
            return 1;

        /* Opprett par */

        GDPL_par_data_node *par = 0;
        GDPL_par_opprett_node ( &par );
        int ny_id = 0;
        if ( GDPL_par_finn_neste_ledige_id ( &ny_id ) > 0 )
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

        if ( GDPL_par_legg_til ( par ) > 0 )
            return 1;
    }

    /* Utfør beregningene og opprett output-fil */

    if ( GDPL_par_beregn ( ) > 0 )
        return 1;

    GDPL_modell_dump ( );

    int antall_par = 0;
    if (GDPL_par_antall_i_liste ( &antall_par ) > 0 )
        return 1;


    /* Skriv ut data til output-fil */

    fprintf( input->output_stream, "Plassering;Startnummer;Herre-navn;Dame-navn;Start-tid;Slutt-tid;Oppgave-poeng;Beregnet middel-tid;Beregnet anvendt-tid;Beregnet avveket-tid;Beregnet tids-poeng;Beregnet total-poeng;\n" );

    struct GDPL_tid middel_tid;
    if ( GDPL_par_beregn_middel_tid ( &middel_tid ) > 0 )
        return FEILKODE_FEIL;

    int i;
    for ( i=1; i <= antall_par; i++ ) {

        GDPL_par_data_node *data = 0;
        if( GDPL_par_hent_i_rekke ( i, &data ) > 0 ) {
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

        /* Skriv ut data til output-fil */

        fprintf ( input->output_stream, "%d;%d;%s %s;%s %s;%02d:%02d:%02d;%02d:%02d:%02d;%02.02f;%02d:%02d:%02d;%02d:%02d:%02d;%02d:%02d:%02d;%02.02f;%02.02f;\n",
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
                  gubb_util_rund_av ( 60.0 - data->tids_poeng ),
                  gubb_util_rund_av ( ( 60.0 - data->tids_poeng) + data->oppgave_poeng ) );

        /* DataEase look-a-like rapporter av typen resultat-liste */

        if ( input->rapport_flagg == 1 && ( input->rapport_type == GRT_RES1 ||  input->rapport_type == GRT_RES2 ) ) {

            gubb_rapport_print_record_data d;

            d.plassering = &i;
            d.par = data;
            d.dame = dperson;
            d.herre = hperson;
            d.middel_tid = &middel_tid;

            gubb_rapport_print_record ( &d, input->rapport_type, input->rapportfil_stream );
        }

    }

    /* DataEase look-a-like rapporter av typen start-liste */

    if ( input->rapport_flagg == 1 && input->rapport_type == GRT_START ) {

        if ( GDPL_par_sorter ( START_NR_STIGENDE ) > 0 )
            return 1;

        fprintf ( input->rapportfil_stream, "STARTLISTE FOR GUBBERENN\n" );
        fprintf ( input->rapportfil_stream, "=======================================================\n" );

        for ( i=1; i <= antall_par; i++ ) {

            GDPL_par_data_node *data = 0;
            if( GDPL_par_hent_i_rekke ( i, &data ) > 0 ) {
                return 1;
            }

            GDPL_person_data_node *hperson = 0;
            if (GDPL_person_hent(data->herre_person_id, &hperson)>0)
                return 1;

            GDPL_person_data_node *dperson = 0;
            if (GDPL_person_hent(data->dame_person_id, &dperson)>0)
                return 1;

            gubb_rapport_print_record_data d;

            d.par = data;
            d.dame = dperson;
            d.herre = hperson;

            gubb_rapport_print_record ( &d, input->rapport_type, input->rapportfil_stream );

        }

    }

    return 0;

}

