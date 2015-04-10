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

#ifndef RAPPORT_H
#define RAPPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> /* FILE */
#include "par.h"

/* Enum benyttes som indeks i en funksjonstabell. */

enum gubb_rapport_type_enum { GRT_START, /* 0 */
                              GRT_RES1,  /* 1 */
                              GRT_RES2   /* 2 */
                            };

typedef enum gubb_rapport_type_enum gubb_rapport_type;

/*! \fn int gubb_rapport_print_record ( GDPL_par_data_node *p, gubb_rapport_type type, FILE *stream )
    \brief Printer ut en record til stream ihht valgt print-type.
    \param GDPL_par_data_node par-recorden som skal printes
    \param gubb_rapport_type måten den skal printes på, ie hvilke data skal være med.
    \param FILE hvilken stream, e.g. stdout, stderr, ei fil etc.

    Funksjonen printer kun ut èn record. Det betyr at sortering er ikke et tema her.
*/
int gubb_rapport_print_record ( GDPL_par_data_node *p, gubb_rapport_type type, FILE *stream );

#ifdef __cplusplus
}
#endif

#endif // RAPPORT_H



