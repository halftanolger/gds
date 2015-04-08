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

/*! \file util.h
    \brief GDPKonsoll hjelpfil
    
    Inneholder ulike hjelpemetoder med tilhørende datastrukturer.
*/    


#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif


/*! \struct gubb_input_args_struct
    \brief Dataholder for inputargumenter.   
    
    Benyttes som dataholder for inputargumenter til konsollprogrammet.
    
*/

struct gubb_input_args_struct {

    /* -h [ --hjelp ] */    
    int hjelp_flagg;

    /* -b [ --bruksanvisning ] */    
    int bruksanvisning_flagg;
    
    /* -v [ --versjon ] */
    int versjon_flagg;
    
    /* -l [ --logg ] argument */
    int logg_flagg;
    char logg_argument[64];
    
    /* -i [ --inputfil] argument */
    int input_flagg;
    char input_argument[512];
    
    /* -o [ --outputfil] argument */
    int output_flagg;
    char output_argument[512];

    /* -r [ --rapport] argument */
    int rapport_flagg;
    char rapport_argument[64];

    /* -f [ --rapportfil] argument */
    int rapportfil_flagg;
    char rapportfil_argument[512];

    /* -k [ --klient] argument */
    int klient_flagg;
    char klient_argument[64];

    /* -s [ --server] argument */
    int server_flagg;
    char server_argument[64];
    
}; 

typedef struct gubb_input_args_struct gubb_input_args;

/*! \fn int gubb_util_parse_args(int argc, char *argv[], gubb_input_args *data)
    \brief Parser inputargumenter gitt til konsollprogrammet, og plasserer dem i en intern datastruktur.
    \param argc Antall argumenter
    \param argv Selve argumentene som kommer inn.
    \param data Struktur hvor de parsede verdiene blir lagret.
*/
int gubb_util_parse_args(int argc, char *argv[], gubb_input_args *data);

#ifdef __cplusplus
}
#endif

#endif // UTIL_H


