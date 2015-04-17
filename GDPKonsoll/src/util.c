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

#include <errno.h>
#include <stdlib.h> /* exit atoi  */
#include <stdio.h>  /* fprintf    */
#include <string.h> /* strcmp     */

#include "util.h"
#include "log.h"
#include "rapport.h"



/* 'Bruk' -tekst, skal vises når korrekt input args mangler. */

const char* gubb_util_bruk_info = "BRUK: gubb [-i ARG] [-o ARG] [-l ARG] [-m ARG] [-r ARG] [-f ARG] [-k ARG] [-s ARG] [-hvbt] ";

/* Hjelpetekst, -h [ --hjelp ] */

const char* gubb_util_hjelp_info = "\n"
        "Dette er et hjelpemiddel ifm arrangering av det tradisjonelle Gubberennet. Gubberennet er et skirenn som arrangeres hver skjærtorsdag i Bjørkebakken.\n"
        "\n"
        "BRUK: gubb [-i ARG] [-o ARG] [-l ARG] [-m ARG] [-r ARG] [-f ARG] [-k ARG] [-s ARG] [-hvbt] \n"
        "\n"
        "\n"
        "-- Beskrivelse av opsjoner --\n"
        "\n"
        "Programmet tar inn en rekke argumenter. De to viktigste er 'inputfile' og 'outputfile'. "
        "Disse angis med hhv -i 'filnavn' og -o 'filnavn'. Videre kan man bruke opsjonen -r for å "
        "produsere en del ulike rapporter som skal være 100% lik de rapportene som Jan-Erik's DataEase "
        "program ville ha produsert, gitt den samme 'input'. Om man i tillegg bruker -f 'filnavn', "
        "kan man få en slik rapport lagret til ei tekstfil.\n"
        "\n"
        "\n"
        "Opsjoner og argumenter:\n"
        "\n"
        " -h [ --hjelp ]              Skriver en hjelpetekst ut på konsollet\n"
        " -b [ --bruksanvisning ]     Skriver ei enkel bruksanvisning ut på konsollet\n"
        " -v [ --versjon ]            Skriver ut versjonsnummeret til GDPLib\n"
        " -l [ --logg ] nivå          Loggnivå, hvor nivå er INF eller DBG\n"
        " -m [ --loggfil ] filnavn    Navn på loggfil, stdout er default\n"
        " -i [ --inputfil ] filnavn   Input csv-datafil\n"
        " -o [ --outputfil ] filnavn  Output csv-datafil\n"
        " -r [ --rapport ] type       Rapport, type er START, RES1 og RES2\n"
        " -f [ --rapportfil ] filnavn Output rapport-datafil\n"
        " -k [ --klient ] portnr      Start gubb-klient, default port er 1234\n"
        " -s [ --klient ] portnr      Start gubb-server, default port er 1234\n"
        " -t [ --test ]               Kjør en rekke enhetstester.\n"
        "\n"
        "\n"
        "Eksempler på bruk:\n"
        "\n"
        " gubb.exe -i c:/tmp/input.cvs -o c:/tmp/output.cvs \n"
        "\n"
        "    Produserer ei outputfil basert på ei inputfil. Inputfila produseres typiskt i Excel, i.e. man registrerer inn data vha "
        "et regneark, f.eks Microsoft Excel eller LibreOffice Calc. Videre vil man typisk åpne outputfila i et tilsvarende "
        "programm.\n"
        "\n"
        " gubb.exe -i c:/tmp/input.cvs -o c:/tmp/output.cvs -r START -f c:/tmp/start.txt\n"
        "\n"
        "    Samme som eksempelet ovenfor, bare med den forskjelle at nå blir det også produsert en rapport som lagres i ei "
        "tekst fil kalt 'c:/tmp/start.txt'. Rapporten som opprettes er av typen 'START', i.e. ei startliste.\n"
        "\n"
        "\n"
        "For mer info, se ulike hjelpefiler som følger med programmet.\n"
        "\n"
        "halftan@online.no\n"
        "\n";

/* Bruksanvisningstekst, -b [ --bruksanvisning ] */

const char* gubb_util_bruksanvisning_info = "\n"
        "\n"
        "Dette programmet utfører aktuelle utregninger ifm Gubberennet, basert på ei csv-inputfil. "
        "Inputfila må være ei semikolon-delt-datafil. Ei slik fil kan du opprette i de fleste "
        "regneark-programmene, for eksempel i Microsoft Office Excel.\n"
        "\n"
        "Nedenfor er et eksempel på ei inputfil som viser hvilke kolonner som må være med, samt hvordan disse er formatert."
        "\n"
        "\n"
        "Eksempel på innhold i ei inputfil:\n"
        "\n"
        "Startnummer;Herre-fornavn;Herre-etternavn;Dame-fornavn;Dame-etternavn;Start-tid;Slutt-tid;Oppgave-poeng\n"
        "100;Ola1;Nordmann1;Kari1;Nordkvinne1;12:00:00;14:32:00;20,0\n"
        "101;Ola2;Nordmann2;Kari2;Nordkvinne2;12:01:00;14:00:00;21,0\n"
        "102;Ola3;Nordmann3;Kari3;Nordkvinne3;12:02:00;14:36:00;22,0\n"
        "103;Ola4;Nordmann4;Kari4;Nordkvinne4;12:03:00;14:38:00;23,0\n"
        "104;Ola5;Nordmann5;Kari5;Nordkvinne5;12:04:00;14:50:00;24,0\n"
        "105;Ola6;Nordmann6;Kari6;Nordkvinne6;12:05:00;14:42:00;25,0\n"
        "\n"
        "Ut fra denne inputfila oppretter programmet ei output-fil hvor disse verdiene er beregnet:\n"
        "\n"
        "   - beregnet anvendt tid pr par\n"
        "   - beregnet gjennomsnittstid, aka idealtiden\n"
        "   - beregnet avvik mellom anvendt tid og idealtid, pr. par\n"
        "   - beregnet tidspoeng basert på beregnet avvik\n"
        "   - beregnet sum poeng; tidspoeng + oppgavepoeng\n"
        "\n"
        "\n"
        "Eksempel på output:\n"
        "\n"
        "Plassering;Startnummer;Herre-navn;Dame-navn;Start-tid;Slutt-tid;Oppgave-poeng;Beregnet middel-tid;Beregnet anvendt-tid;Beregnet avveket-tid;Beregnet tids-poeng;Beregnet total-poeng;\n"
        "1;102;Ola3 Nordmann3;Kari3 Nordkvinne3;12:2:0;14:36:0;22;2:33:32;2:34:0;0:0:28;60;82;\n"
        "2;103;Ola4 Nordmann4;Kari4 Nordkvinne4;12:3:0;14:38:0;23;2:33:32;2:35:0;0:1:28;59;82;\n"
        "3;105;Ola6 Nordmann6;Kari6 Nordkvinne6;12:5:0;14:42:0;25;2:33:32;2:37:0;0:3:28;57;82;\n"
        "4;106;Ola7 Nordmann7;Kari7 Nordkvinne7;12:6:0;14:44:0;26;2:33:32;2:38:0;0:4:28;56;82;\n"
        "5;107;Ola8 Nordmann8;Kari8 Nordkvinne8;12:7:0;14:46:0;27;2:33:32;2:39:0;0:5:28;55;82;\n"
        "\n"
        "Eksempel på bruk av programmet:\n"
        "\n"
        "  gubb.exe -i mininputfil.csv -o output_mininputfil.cvs \n"
        "\n"
        "Outputfila kan du for eksempel åpne i Microsoft Office Excel, for videre behandling. (Ps. Du angir navnet til outputfila, men \n"
        "programmet tar selv å oppretter denne fila.) \n"
        "\n"
        "-TheEnd-\n"
        "\n"
        "\n";






int gubb_util_init_log ( gubb_input_args *data ) {

    GDPL_log_type log_level;

    if ( strcmp( data->logg_argument, "DBG" ) == 0 ) {
        log_level = GDPL_DEBUG;
    } else if ( strcmp( data->logg_argument, "INF" ) == 0 ) {
        log_level = GDPL_INFO;
    } else if ( strcmp( data->logg_argument, "WRN" ) == 0 ) {
        log_level = GDPL_WARNING;
    } else {
        log_level = GDPL_ERROR; /* Error er default loggnivå */
    }

    data->loggfil_stream = stdout; /* Stdout er default loggmedium */

    if ( data->loggfil_flagg == 1 ) {
        if ( strcmp( data->loggfil_argument, "stderr" ) == 0 ) {
            data->loggfil_stream = stderr;
        } else {
            errno = 0;
            data->loggfil_stream = fopen( data->loggfil_argument, "w" );
            if ( data->loggfil_stream  == 0 ) {
                fprintf ( stderr, "%s %s\n", data->logg_argument, strerror ( errno ) );
                exit ( EXIT_FAILURE );
            }
        }
    }

    return GDPL_log_init ( log_level, data->loggfil_stream );

}


void gubb_util_init_rapport ( gubb_input_args *data ) {

    if ( data->rapport_flagg == 1 ) {

        /* Finn hvor rapporten skal skrives */

        if (data->rapportfil_flagg == 0 ) {
            strcpy ( data->rapportfil_argument, "stdout" );
        }

        if ( strcmp ( data->rapportfil_argument, "stdout" ) == 0 ) {
            data->rapportfil_stream = stdout;
        } else if ( strcmp ( data->rapportfil_argument, "stderr" ) == 0 ) {
            data->rapportfil_stream = stderr;
        } else {
            errno = 0;
            data->rapportfil_stream = fopen( data->rapportfil_argument, "w" );
            if ( data->rapportfil_stream == 0 ) {
                fprintf ( stderr, "%s %s\n", data->rapportfil_argument, strerror ( errno ) );
                exit ( EXIT_FAILURE );
            }
        }

        /* Finn ut hvilken type rapport som skal opprettes */

        if ( strcmp (data->rapport_argument, "START" ) == 0 ) {
            data->rapport_type = GRT_START;
        } else if ( strcmp ( data->rapport_argument, "RES1" ) == 0 ) {
            data->rapport_type = GRT_RES1;
        } else if ( strcmp ( data->rapport_argument, "RES2" ) == 0 ) {
            data->rapport_type = GRT_RES2;
        } else {
            data->rapport_type = -1;
        }

    }

}


void gubb_util_nullstill_input_args ( gubb_input_args *data ) {

    /* -h [ --hjelp ] */
    data->hjelp_flagg = 0;

    /* -t [ --test ] */
    data->test_flagg = 0;

    /* -b [ --bruksanvisning ] */
    data->bruksanvisning_flagg = 0;

    /* -v [ --versjon ] */
    data->versjon_flagg = 0;

    /* -l [ --logg ] argument */
    data->logg_flagg = 0;
    strcpy( data->logg_argument, "nop" );

    /* -m [ --loggfil ] argument */
    data->loggfil_flagg = 0;
    strcpy( data->loggfil_argument, "nop" );
    data->loggfil_stream = NULL;

    /* -i [ --inputfil] argument */
    data->input_flagg = 0;
    strcpy( data->input_argument, "nop" );
    data->input_stream = NULL;

    /* -o [ --outputfil] argument */
    data->output_flagg = 0;
    strcpy( data->output_argument, "nop" );
    data->output_stream = NULL;

    /* -r [ --rapport] argument */
    data->rapport_flagg = 0;
    strcpy( data->rapport_argument, "nop" );
    data->rapport_type = 0;

    /* -f [ --rapportfil] argument */
    data->rapportfil_flagg = 0;
    strcpy( data->rapportfil_argument, "nop" );
    data->rapportfil_stream = NULL;

    /* -k [ --klient] argument */
    data->klient_flagg = 0;
    strcpy( data->klient_argument, "nop" );

    /* -s [ --server] argument */
    data->server_flagg = 0;
    strcpy( data->server_argument, "nop" );

}

void gubb_util_lukk_eventuelle_aapne_filer ( gubb_input_args *data ) {

    if ( data->loggfil_stream != NULL )
        fclose ( data->loggfil_stream );

    if ( data->input_stream != NULL )
        fclose ( data->input_stream );

    if ( data->output_stream != NULL )
        fclose ( data->output_stream );

    if ( data->rapportfil_stream != NULL )
        fclose ( data->rapportfil_stream );

}

void gubb_util_parse_args(int argc, char *argv[], gubb_input_args *data)
{
    int i;
    int heltall;
    int bruk_flagg;
    char heltallstr[64];

    /* Initier diverse variabler */

    i = 0;
    heltall = 0;
    bruk_flagg = 1;
    strcpy( heltallstr, "nop" );

    gubb_util_nullstill_input_args ( data );

    /* Parse inputargumenter og legg disse i datamodellen */

    for ( i = 1; i < argc; i++ ) {

        if ( strcmp ( argv[i], "-h" ) == 0 ||
             strcmp ( argv[i], "--hjelp" ) == 0 ) {
            data->hjelp_flagg = 1;
            bruk_flagg = 0;
            continue;
        }

        if ( strcmp ( argv[i], "-t" ) == 0 ||
             strcmp ( argv[i], "--test" ) == 0 ) {
            data->test_flagg = 1;
            bruk_flagg = 0;
            continue;
        }

        if ( strcmp ( argv[i], "-b" ) == 0 ||
             strcmp ( argv[i], "--bruksanvisning" ) == 0 ) {
            data->bruksanvisning_flagg = 1;
            bruk_flagg = 0;
            continue;
        }

        if ( strcmp ( argv[i], "-v" ) == 0 ||
             strcmp ( argv[i], "--versjon" ) == 0 ) {
            data->versjon_flagg = 1;
            bruk_flagg = 0;
            continue;
        }

        if ( ( strcmp ( argv[i], "-i") == 0 ||
               strcmp ( argv[i], "--inputfil") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen ( argv[i] ) < 512 ) {
                    data->input_flagg = 1;
                    strcpy ( data->input_argument, argv[i] );
                    continue;
                } else {
                    fprintf ( stderr, "Argumentet til -i [ --inputfil ] er for langt.\n" );
                    exit ( 1 );
                }
            } else {
                fprintf ( stderr, "Argumentet til -i [ --inputfil ] mangler.\n" );
                exit ( 1 );
            }
        }
        
        if ( ( strcmp ( argv[i], "-o") == 0 ||
               strcmp ( argv[i], "--outputfil") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen( argv[i] ) < 512 ) {
                    data->output_flagg = 1;
                    strcpy( data->output_argument, argv[i] );
                    continue;
                } else {
                    fprintf ( stderr, "Argumentet til -o [ --outputfil ] er for langt.\n" );
                    exit ( 1 );
                }
            } else {
                fprintf ( stderr, "Argumentet til -o [ --outputfil ] mangler.\n" );
                exit ( 1 );
            }
        }

        if ( ( strcmp ( argv[i], "-m") == 0 ||
               strcmp ( argv[i], "--loggfil") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen( argv[i] ) < 512 ) {
                    data->loggfil_flagg = 1;
                    strcpy( data->loggfil_argument, argv[i] );
                    continue;
                } else {
                    fprintf ( stderr, "Argumentet til -m [ --loggfil ] er for langt.\n" );
                    exit ( 1 );
                }
            } else {
                fprintf ( stderr, "Argumentet til -m [ --loggfil ] mangler.\n" );
                exit ( 1 );
            }
        }

        if ( ( strcmp ( argv[i], "-l") == 0 ||
               strcmp ( argv[i], "--logg") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen ( argv[i] ) < 64 ) {
                    data->logg_flagg = 1;
                    strcpy ( data->logg_argument, argv[i] );
                    if ( strcmp ( data->logg_argument, "WRN" ) != 0 &&
                         strcmp ( data->logg_argument, "INF" ) != 0 &&
                         strcmp ( data->logg_argument, "DBG" ) != 0 ) {
                        fprintf ( stderr, "Argumentet til -l [ --logg ] kan kun være INF, WRN eller DGB, for hhv 'info', 'warning' eller 'debug'.\n" );
                        exit ( 1 );
                    }
                    continue;
                } else {
                    fprintf ( stderr, "Argumentet til -l [ --logg ] er for langt.\n" );
                    exit ( 1 );
                }
            } else {
                fprintf ( stderr, "Argumentet til -l [ --logg ] mangler.\n" );
                exit ( 1 );
            }
        }

        if ( ( strcmp ( argv[i], "-r") == 0 ||
               strcmp ( argv[i], "--rapport") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen ( argv[i] ) < 64 ) {
                    data->rapport_flagg = 1;
                    strcpy ( data->rapport_argument, argv[i] );
                    if ( strcmp ( data->rapport_argument, "START" ) != 0 &&
                         strcmp ( data->rapport_argument, "RES1" ) != 0 &&
                         strcmp ( data->rapport_argument, "RES2" ) != 0 ) {
                        fprintf ( stderr, "Argumentet til -r [ --rapport ] kan kun være START, RES1 eller RES2, hvor START lager ei startliste, RES1 lager ei enkel resultatliste og RES2 lager ei komplett resultatliste.\n" );
                        exit ( 1 );
                    }
                    continue;
                } else {
                    fprintf ( stderr, "Argumentet til -r [ --rapport ] er for langt.\n" );
                    exit ( 1 );
                }
            } else {
                fprintf ( stderr, "Argumentet til -r [ --rapport ] mangler.\n" );
                exit ( 1 );
            }
        }

        if ( ( strcmp ( argv[i], "-f") == 0 ||
               strcmp ( argv[i], "--rapportfil") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen( argv[i] ) < 512 ) {
                    data->rapportfil_flagg = 1;
                    strcpy( data->rapportfil_argument, argv[i] );
                    continue;
                } else {
                    fprintf ( stderr, "Argumentet til -f [ --rapportfil ] er for langt.\n" );
                    exit ( 1 );
                }
            } else {
                fprintf ( stderr, "Argumentet til -f [ --rapportfil ] mangler.\n" );
                exit ( 1 );
            }
        }

        if ( ( strcmp ( argv[i], "-k") == 0 ||
               strcmp ( argv[i], "--klient") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen ( argv[i] ) < 64 ) {
                    data->klient_flagg = 1;
                    strcpy ( data->klient_argument, argv[i] );
                    heltall = atoi ( data->klient_argument );
                    sprintf ( heltallstr, "%d", heltall );
                    if ( strcmp ( heltallstr, data->klient_argument ) != 0 ) {
                        fprintf ( stderr, "Argumentet til -k [ --klient ] (i.e portnummeret) må være et positivt heltall.\n" );
                        exit ( 1 );
                    }
                    bruk_flagg = 0;
                    continue;
                } else {
                    fprintf ( stderr, "Argumentet til -k [ --klient ] er for langt.\n" );
                    exit ( 1 );
                }
            } else {
                fprintf ( stderr, "Argumentet til -k [ --klient ] mangler.\n" );
                exit ( 1 );
            }
        }

        if ( ( strcmp ( argv[i], "-s") == 0 ||
               strcmp ( argv[i], "--server") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen ( argv[i] ) < 64 ) {
                    data->server_flagg = 1;
                    strcpy ( data->server_argument, argv[i] );
                    heltall = atoi ( data->server_argument );
                    sprintf ( heltallstr, "%d", heltall );
                    if ( strcmp ( heltallstr, data->server_argument ) != 0 ) {
                        fprintf ( stderr, "Argumentet til -s [ --server ] (i.e portnummeret) må være et positivt heltall.\n" );
                        exit ( 1 );
                    }
                    bruk_flagg = 0;
                    continue;
                } else {
                    fprintf ( stderr, "Argumentet til -s [ --server ] er for langt.\n" );
                    exit ( 1 );
                }
            } else {
                fprintf ( stderr, "Argumentet til -s [ --server ] mangler.\n" );
                exit ( 1 );
            }
        }

        fprintf ( stderr, "Ukjent argument %s (argument nummer %d).\n", argv[i], i );
        exit ( 1 );
        
    }

    if ( data->hjelp_flagg == 1 ) {
        fprintf ( stdout, "%s\n", gubb_util_hjelp_info );
        exit ( 0 );
    }

    if ( data->bruksanvisning_flagg == 1 ) {
        fprintf ( stdout, "%s\n", gubb_util_bruksanvisning_info );
        exit ( 0 );
    }

    if ( data->versjon_flagg == 1 ) {
        fprintf ( stdout, "%s %s\n", GDPL_kontroller_gdplib_navn(), GDPL_kontroller_gdplib_versjon() );
        exit ( 0 );
    }

    if ( data->input_flagg == 1 && data->output_flagg == 1 ) {
        bruk_flagg = 0;
    }
    
    if ( bruk_flagg == 1 ) {
        printf( "%s\n", gubb_util_bruk_info );
        exit ( 1 );
    }

    /* Litt semantikk sjekking */

    if ( data->rapportfil_flagg == 1 && data->rapport_flagg == 0 ) {
        fprintf ( stderr, "Inkonsistens. Argumentet -r [ --rapport ] mangler, mens -f [ --rapportfil ] er angitt.\n" );
        exit ( 1 );
    }

    if ( data->klient_flagg == 1 && data->server_flagg == 1 ) {
        fprintf ( stderr, "Inkonsistens. Argumentet -k [ --klient ] og -s [ --server ] er begge to angitt.\n" );
        exit ( 1 );
    }

    if ( ( data->input_flagg == 1 ||
           data->output_flagg == 1 ||
           data->rapport_flagg == 1 ||
           data->rapportfil_flagg == 1 ) &&
         ( data->klient_flagg == 1 || data->server_flagg == 1 ) ) {
        fprintf ( stderr, "Inkonsistens. Argumentet -k [ --klient ] eller -s [ --server ] kan ikke benyttes i lag med [iorf].\n" );
        exit ( 1 );
    }

    /* Diverse initiering */

    if ( gubb_util_init_log( data ) > 0 ) {
        fprintf ( stderr, "Initiering av loggfunksjon feilet.\n" );
        exit ( 1 );
    }

    gubb_util_init_rapport ( data );

    if ( data->input_flagg == 1 ) {
        errno = 0;
        data->input_stream = fopen( data->input_argument, "r" );
        if ( data->input_stream == 0 ) {
            fprintf ( stderr, "%s %s\n", data->input_argument, strerror ( errno ) );
            exit ( 1 );
        }
    }

    if ( data->output_flagg == 1 ) {
        errno = 0;
        data->output_stream = fopen( data->output_argument, "w" );
        if ( data->output_stream == 0 ) {
            fprintf ( stderr, "%s %s\n", data->output_argument, strerror ( errno ) );
            exit ( 1 );
        }
    }

    if ( data->test_flagg == 1 ) {
        fprintf ( stdout, "GDPTest utført. Antall feil %d\n", gdp_test_run() );
    }

}


float gubb_util_rund_av(float v) {
    return ((int)(v * 100 + .5) / 100.0);
}
