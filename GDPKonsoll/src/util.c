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

#include <stdlib.h> /* exit atoi  */
#include <stdio.h>  /* fprintf    */
#include <string.h> /* strcmp     */

#include "util.h"

int gubb_util_parse_args(int argc, char *argv[], gubb_input_args *data) 
{
    int i;
    int heltall;
    int bruk_flagg;
    char heltallstr[64];

    i = 0;
    heltall = 0;
    bruk_flagg = 1;
    data->logg_flagg = 0;
    data->hjelp_flagg = 0;
    data->input_flagg = 0;
    data->klient_flagg = 0;
    data->server_flagg = 0;
    data->output_flagg = 0;
    data->rapport_flagg = 0;
    data->versjon_flagg = 0;
    data->rapportfil_flagg = 0;
    data->bruksanvisning_flagg = 0;
    strcpy( heltallstr, "nop" );
    strcpy( data->logg_argument, "nop" );
    strcpy( data->input_argument, "nop" );
    strcpy( data->klient_argument, "nop" );
    strcpy( data->server_argument, "nop" );
    strcpy( data->output_argument, "nop" );
    strcpy( data->rapport_argument, "nop" );
    strcpy( data->rapportfil_argument, "nop" );

    for ( i = 1; i < argc; i++ ) {

        if ( strcmp ( argv[i], "-h" ) == 0 ||
             strcmp ( argv[i], "--hjelp" ) == 0 ) {
            data->hjelp_flagg = 1;
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
        
        if ( ( strcmp ( argv[i], "-l") == 0 ||
               strcmp ( argv[i], "--logg") == 0 ) ) {
            if ( argc > ( i + 1 ) ) {
                i++;
                if ( strlen ( argv[i] ) < 64 ) {
                    data->logg_flagg = 1;
                    strcpy ( data->logg_argument, argv[i] );
                    if ( strcmp ( data->logg_argument, "INF" ) != 0 &&
                         strcmp ( data->logg_argument, "DBG" ) != 0 ) {
                        fprintf ( stderr, "Argumentet til -l [ --logg ] kan kun være INF eller DGB, for hhv 'info' eller 'debug'.\n" );
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

    if ( data->input_flagg == 1 && data->output_flagg == 1 ) {
        bruk_flagg = 0;
    }
    
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

    return bruk_flagg;
    
}
