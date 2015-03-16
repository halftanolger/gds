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

#ifndef MODELL_H
#define MODELL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "diverse.h"


/* Variabel-deklareringer */

struct GDPL_node {
    void *data;
    struct GDPL_node *neste;
};

struct GDPL_konkurranse_data {
    int id;
    int aar;
    struct GDPL_node *person_liste;
    struct GDPL_node *par_liste;
};




struct GDPL_par_data_node_struct {
    int id;
    int herre_person_id;
    int dame_person_id;
    int start_nr;
    char start_tid[GDPL_MAX_TID_LENGDE];
    char maal_tid[GDPL_MAX_TID_LENGDE];
    int oppgave_poeng;
    struct GDPL_par_data_node_struct *neste;
};

struct GDPL_person_data_node_struct {
    int id;
    char *fnavn;
    char *enavn;
    struct GDPL_person_data_node_struct *neste;
};

typedef struct GDPL_par_data_node_struct GDPL_par_data_node;
typedef struct GDPL_person_data_node_struct GDPL_person_data_node;

struct GDPL_konkurranse_data_node_struct {
    int id;
    int aar;
    GDPL_person_data_node *person_liste_root_ptr;
    GDPL_par_data_node *par_liste_root_ptr;
    struct GDPL_konkurranse_data_node_struct *neste;
};

typedef struct GDPL_konkurranse_data_node_struct  GDPL_konkurranse_data_node;

/* Variabler */

extern char gdpl_modell_datafilnavn[GDPL_MAX_FILNAVN_LENGDE];

extern GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_root_ptr;

extern GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_valgt_ptr;


/* Funksjons-deklareringer */


int GDPL_modell_angi_filnavn(const char *filnavn);

int GDPL_modell_les_data();

int GDPL_modell_skriv_data();


/* 'Private' funksjons-deklareringer */


int GDPL_modell_privat_opprett_ny_fil();

int GDPL_modell_privat_skriv_til_eksisterende_fil();

int GDPL_modell_privat_les_inn_fra_eksisterende_fil();

int GDPL_modell_privat_sjekk_filnavn(const char* filnavn);

#ifdef __cplusplus
}
#endif

#endif // MODELL_H
