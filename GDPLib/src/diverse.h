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

#ifndef DIVERSE_H
#define DIVERSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

// Defines

#define FEILKODE_DATAFILNAVN_FOR_LANGT       1
#define FEILKODE_DATAFILNAVN_FOR_KORT        2
#define FEILKODE_DATAFILNAVN_IKKE_DEFINERT   3
#define FEILKODE_KAN_IKKE_OPPRETTE_DATAFIL   4
#define FEILKODE_KAN_LESE_FRA_DATAFIL        5
#define FEILKODE_KAN_SKRIVE_TIL_DATAFIL      6
#define FEILKODE_KAN_IKKE_ALLOKERE_MINNE     7
#define FEILKODE_ID_EKSISTERER               8
#define FEILKODE_ID_EKSISTERER_IKKE          9
#define FEILKODE_FEIL                        10
#define FEILKODE_MAX_ANTALL_PAR_FOR_LAVT     11
#define FEILKODE_MAX_ANTALL_PAR_FOR_STORT    12
#define FEILKODE_MAX_ANTALL_IKKE_ANGITT      13

#define GDPL_MAX_PERSONNAVN_LENGDE           128
#define GDPL_MIN_PERSONNAVN_LENGDE           1
#define GDPL_MAX_FILNAVN_LENGDE              516
#define GDPL_MIN_FILNAVN_LENGDE              8
#define GDPL_MAX_TID_LENGDE                  16
#define GDPL_MAX_ANTALL_PAR                  200

// Enums

enum GDPL_log_type_enum {GDPL_DEBUG, GDPL_INFO, GDPL_WARNING, GDPL_ERROR};



// Typedefs

typedef enum GDPL_log_type_enum GDPL_log_type;




// Variabler

extern GDPL_log_type gdpl_log_nivaa;

extern FILE * gdpl_log_stream;


// Funksjoner

void GDPL_log(GDPL_log_type, const char*, const char*, ...);

int GDPL_init_log(GDPL_log_type nivaa, FILE * stream);



#ifdef __cplusplus
}
#endif

#endif // DIVERSE_H

