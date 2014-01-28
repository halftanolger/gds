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

#ifndef _GDPL_H_
#define _GDPL_H_

#include <stdio.h>

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

#define GDPL_MAX_PERSONNAVN_LENGDE           1024
#define GDPL_MIN_PERSONNAVN_LENGDE           1
#define GDPL_MAX_FILNAVN_LENGDE              1024
#define GDPL_MIN_FILNAVN_LENGDE              8

typedef enum GDPL_log_type_enum {DEBUG, INFO, WARNING, ERROR} GDPL_log_type;

typedef struct GDPL_par_data_node_struct {
	int id;
	int herre_person_id;
	int dame_person_id;
	int start_nr;
	char start_tid[16];
	char maal_tid[16];
	double oppgave_poeng;	
	struct GDPL_par_data_node_struct *neste;
} GDPL_par_data_node;

typedef struct GDPL_person_data_node_struct {
	int id;
	char *fnavn;
	char *enavn;
	struct GDPL_person_data_node_struct *neste;
} GDPL_person_data_node;

typedef struct GDPL_konkurranse_data_node_struct {
	int id;
	int aar;
	struct GDPL_person_data_node_struct *person_liste_root_ptr;
	struct GDPL_par_data_node_sctruct *par_liste_root_ptr;
	struct GDPL_konkurranse_data_node_struct *neste;
} GDPL_konkurranse_data_node;

/* Funksjoner */

void GDPL_log(GDPL_log_type, const char*, const char*, ...);
int GDPL_init(GDPL_log_type nivaa, FILE * stream); 
int GDPL_test();

int GDPL_kontroller_angi_filnavn(const char *filnavn);
int GDPL_kontroller_les_fra_fil();
int GDPL_kontroller_skriv_til_fil();

int GDPL_konkurranse_opprett_node(GDPL_konkurranse_data_node **new_node);
int GDPL_konkurranse_legg_til(GDPL_konkurranse_data_node data, GDPL_konkurranse_data_node *root);
int GDPL_konkurranse_fjern_fra(GDPL_konkurranse_data_node data, GDPL_konkurranse_data_node *root);
int GDPL_konkurranse_antall_i_liste(int *antall, GDPL_konkurranse_data_node *root);
int GDPL_konkurranse_hent(int id, GDPL_konkurranse_data_node **data, GDPL_konkurranse_data_node *root);

int GDPL_person_opprett_node(GDPL_person_data_node **new_node);
int GDPL_person_legg_til(GDPL_person_data_node data, GDPL_person_data_node *root);
int GDPL_person_fjern_fra(GDPL_person_data_node data, GDPL_person_data_node *root);
int GDPL_person_antall_i_liste(int *antall, GDPL_person_data_node *root);
int GDPL_person_hent(int id, GDPL_person_data_node **data, GDPL_person_data_node *root);

int GDPL_par_opprett_node(GDPL_par_data_node **new_node);
int GDPL_par_legg_til(GDPL_par_data_node data, GDPL_par_data_node *root);
int GDPL_par_fjern_fra(GDPL_par_data_node data, GDPL_par_data_node *root);
int GDPL_par_antall_i_liste(int *antall, GDPL_par_data_node *root);
int GDPL_par_hent(int id, GDPL_par_data_node **data, GDPL_par_data_node *root);

#endif //GDPL_H
