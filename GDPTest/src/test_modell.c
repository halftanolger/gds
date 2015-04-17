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

#include <unistd.h> /* access, getcwd */
#include <stdio.h>  /* rename */
#include <errno.h>
#include "log.h"
#include "modell.h"

void gdp_test_modell_flytt_fil_privat ( const char *filnavn, char *bak ) {

    /* Om fila 'filnavn' eksisterer, så flytt denne til ei fil som
     * heter 'filnavn.bak<n>', hvor n er et heltall mellom 1 og  max */

    const char* signatur = "gdp_test_modell_flytt_fil_privat()";
    GDPL_log ( GDPL_DEBUG, signatur, "Start funksjon." );

    int r;
    int max;
    int teller;
    r = 0;
    max = 100;
    teller = 0;

    r = access ( filnavn, F_OK );
    if ( r == 0) {
        GDPL_log ( GDPL_DEBUG, signatur, "%s eksisterer, prøv å lag ei 'bak'-fil av denne.", filnavn );
        errno = 0;
        for ( teller = 1; teller < max; teller++ ) {
            sprintf( bak, "%s.bak%d", filnavn, teller );
            if ( rename ( filnavn, bak )  == 0 )
                break;
        }
        if (teller == max) {
            GDPL_log ( GDPL_ERROR, signatur, "rename feilet. %s ", strerror ( errno ) );
            exit ( 1 );
        }
        GDPL_log ( GDPL_DEBUG, signatur, "%s er omdøpt til %s", filnavn, bak );
    } else {
        GDPL_log ( GDPL_DEBUG, signatur, "%s eksisterer ikke.", filnavn );
    }

    GDPL_log ( GDPL_DEBUG, signatur, "Slutt funksjon." );
}


int gdp_test_modell_opprett_datafil_a() {

    /* Testcase. Om man oppgir en null-peker som filnavn,
     * skal GDPLib opprette ei default datafil, kalt gdp.dat
     * Ved hjelp av GDPL_modell_les_data() tar denne funksjonen å
     * tester at fila faktisk blir opprettet. */

    const char* signatur = "gdp_test_modell_opprett_datafil_a()";
    GDPL_log ( GDPL_DEBUG, signatur, "Start funksjon." );

    int r;
    int feil;
    char *n;
    char cwd[1024];
    char bak[1024];

    r = 0;
    feil = 0;
    n = NULL;
    strcpy ( bak, "nop" );

    /* Ta vare på ei eventuell eksisterende datafil. */
    gdp_test_modell_flytt_fil_privat ( "gdp.dat", bak );

    if ( getcwd ( cwd, sizeof ( cwd ) ) == NULL ) {
        GDPL_log ( GDPL_ERROR, signatur, "getcwd ( cwd, sizeof ( cwd ) ) == NULL " );
        exit ( 1 );
    }

    if ( GDPL_modell_angi_filnavn ( n ) > 0 ) {
        GDPL_log ( GDPL_ERROR, signatur, "GDPL_modell_angi_filnavn(n) > 0, I'm out of here." );
        exit ( 1 );
    }

    if ( GDPL_modell_les_data ( ) > 0 ) { /* Dette vil opprette datafila. */
        GDPL_log ( GDPL_ERROR, signatur, "GDPL_modell_les_data() > 0, I'm out of here." );
        exit ( 1 );
    }

    r = access ( "gdp.dat", F_OK );

    if ( r == 0 ) {
       GDPL_log ( GDPL_DEBUG, signatur, "gdp.dat ble opprettet her %s", cwd );
       feil = 0;

       /* Rydd opp, i.e fjern den fila vi opprettet */
       errno = 0;
       r = remove( "gdp.dat" );
       if ( r != 0 ) {
           GDPL_log ( GDPL_ERROR, signatur, "remove feilet. %s ", strerror ( errno ) );
          exit ( 1 );
       }

       /* Flytt tilbake ei eventuell bak-fil. */
       if ( strcmp ( bak, "nop" ) != 0 ) {
           errno = 0;
           r = rename ( bak, "gdp.dat" );
           if ( r != 0 ) {
               GDPL_log ( GDPL_ERROR, signatur, "rename feilet. %s ", strerror ( errno ) );
              exit ( 1 );
           }
       }
    } else {
       GDPL_log ( GDPL_ERROR, signatur, "gdp.dat eksisterer ikke" );
       feil = 1;
    }

    if ( feil == 1) {
        GDPL_log ( GDPL_ERROR, signatur, "Testen feilet!" );
    }
    GDPL_log ( GDPL_DEBUG, signatur, "Slutt funksjon. feil=%d", feil );
    return feil;
}

int gdp_test_modell_opprett_datafil_b() {

    /* Testcase. Om man oppgir en streng som filnavn,
     * skal GDPLib opprette ei datafil ved det oppgitte navnet.
     * Ved hjelp av GDPL_modell_les_data() tar denne funksjonen å
     * tester at fila faktisk blir opprettet. */

    const char* signatur = "gdp_test_modell_opprett_datafil_b()";
    GDPL_log ( GDPL_DEBUG, signatur, "Start funksjon." );

    int r;
    int feil;
    char *n;
    char cwd[1024];
    char bak[1024];
    char fn[1024];

    r = 0;
    feil = 0;
    n = NULL;
    strcpy ( bak, "nop" );
    strcpy ( fn, "mingubbdatafil.dat" );

    /* Ta vare på ei eventuell eksisterende datafil som tilfeldigvis
       har dette filnavnet.*/
    gdp_test_modell_flytt_fil_privat ( fn, bak );

    if ( getcwd ( cwd, sizeof ( cwd ) ) == NULL ) {
        GDPL_log ( GDPL_ERROR, signatur, "getcwd ( cwd, sizeof ( cwd ) ) == NULL " );
        exit ( 1 );
    }

    if ( GDPL_modell_angi_filnavn ( fn ) > 0 ) {
        GDPL_log ( GDPL_ERROR, signatur, "GDPL_modell_angi_filnavn(fn) > 0, I'm out of here." );
        exit ( 1 );
    }

    if ( GDPL_modell_les_data ( ) > 0 ) { /* Dette vil opprette datafila. */
        GDPL_log ( GDPL_ERROR, signatur, "GDPL_modell_les_data() > 0, I'm out of here." );
        exit ( 1 );
    }

    r = access ( fn, F_OK );

    if ( r == 0 ) {
       GDPL_log ( GDPL_DEBUG, signatur, "%s ble opprettet her %s", fn, cwd );
       feil = 0;

       /* Rydd opp, i.e fjern den fila vi opprettet */
       errno = 0;
       r = remove( fn );
       if ( r != 0 ) {
           GDPL_log ( GDPL_ERROR, signatur, "remove feilet. %s ", strerror ( errno ) );
           exit ( 1 );
       }

       /* Flytt tilbake ei eventuell bak-fil. */
       if ( strcmp ( bak, "nop" ) != 0 ) {
           errno = 0;
           r = rename ( bak, fn );
           if ( r != 0 ) {
               GDPL_log ( GDPL_ERROR, signatur, "rename feilet. %s ", strerror ( errno ) );
               exit ( 1 );
           }
       }
    } else {
       GDPL_log ( GDPL_ERROR, signatur, "%s eksisterer ikke", fn );
       feil = 1;
    }

    if ( feil == 1) {
        GDPL_log ( GDPL_ERROR, signatur, "Testen feilet!" );
    }
    GDPL_log ( GDPL_DEBUG, signatur, "Slutt funksjon. feil=%d", feil );
    return feil;
}


int gdp_test_modell_opprett_datafil_c() {

    /* Om man oppgir et filnavn, så kan ikke dette være
     * lengre enn GDPL_MAX_FILNAVN_LENGDE */

    const char* signatur = "gdp_test_modell_opprett_datafil_c()";
    GDPL_log ( GDPL_DEBUG, signatur, "Start funksjon." );

    int feil;
    char fn[GDPL_MAX_FILNAVN_LENGDE + 8];

    feil = 0;

    memset ( fn, 'a', GDPL_MAX_FILNAVN_LENGDE + 2 );
    fn[GDPL_MAX_FILNAVN_LENGDE + 3] = '\0';

    if ( GDPL_modell_angi_filnavn ( fn ) ==  FEILKODE_DATAFILNAVN_FOR_LANGT ) {
        GDPL_log ( GDPL_DEBUG, signatur, "ok, GDPL_modell_angi_filnavn () skal feile her." );
        feil = 0;
    } else {
        feil = 1;
    }

    strcpy ( fn, "etokfilnavn.dat" );
    if ( strlen( fn ) < GDPL_MAX_FILNAVN_LENGDE ) {
        if ( GDPL_modell_angi_filnavn ( fn ) > 0 ) {
            GDPL_log ( GDPL_DEBUG, signatur, "ikke ok, GDPL_modell_angi_filnavn () skal ikke feile her." );
            feil = 1;
        } else {
            feil = 0;
        }
    }

    if ( feil == 1) {
        GDPL_log ( GDPL_ERROR, signatur, "Testen feilet!" );
    }
    GDPL_log ( GDPL_DEBUG, signatur, "Slutt funksjon. feil=%d", feil );
    return feil;
}


int gdp_test_modell_opprett_datafil_d() {

    /* Om man oppgir et filnavn, så kan ikke dette være
     * kortere enn GDPL_MIN_FILNAVN_LENGDE */

    const char* signatur = "gdp_test_modell_opprett_datafil_d()";
    GDPL_log ( GDPL_DEBUG, signatur, "Start funksjon." );

    int feil;
    char fn[GDPL_MAX_FILNAVN_LENGDE];

    feil = 0;

    if ( GDPL_MIN_FILNAVN_LENGDE > GDPL_MAX_FILNAVN_LENGDE ) {
        GDPL_log ( GDPL_ERROR, signatur, "GDPL_MIN_FILNAVN_LENGDE > GDPL_MAX_FILNAVN_LENGDE" );
        exit ( 1 );
    }

    memset ( fn, 'a', GDPL_MIN_FILNAVN_LENGDE - 1 );
    memset ( fn, '\0', GDPL_MIN_FILNAVN_LENGDE );

    if ( GDPL_modell_angi_filnavn ( fn ) == FEILKODE_DATAFILNAVN_FOR_KORT ) {
        GDPL_log ( GDPL_DEBUG, signatur, "ok, GDPL_modell_angi_filnavn () skal feile her." );
        feil = 0;
    } else {
        feil = 1;
    }

    if ( feil == 1) {
        GDPL_log ( GDPL_ERROR, signatur, "Testen feilet!" );
    }
    GDPL_log ( GDPL_DEBUG, signatur, "Slutt funksjon. feil=%d", feil );
    return feil;
}
