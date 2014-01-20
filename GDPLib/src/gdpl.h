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
**
** Beskrivelse
**  Denne fila beskriver interfacet til funksjonene i GDPLib
**
**  int GDPL_controller_set_filename(const char *filename) 
**		Angi navn til datafil. Om du angir tallet null, vil det bli
**      opprettet ei datafil med navnet 'gds.dat'.
**
**  int GDPL_controller_read_from_file() 
**		Les inn data fra datafil.
**
**
**
******************************************************************************/

#ifndef _GDPL_H_
#define _GDPL_H_

#define GDPL_MAX_DATA_FILENAME_LENGTH  255

/*
 * Feilkoder. Alle funksjonene, som er ment å bli brukt eksternt, 
 * returnerer en feilkode ulik null, om de feiler. Disse feilkodene
 * kan benyttes til å hente ei feilmelding fra tabellen 
 * GDPL_controller_error_codes[] 
 */
#define ERROR_FILENAME_TO_LONG           1
#define ERROR_FILENAME_TO_SHORT          2
#define ERROR_FILENAME_IS_NOT_SET        3
#define ERROR_CAN_NOT_CREATE_DATAFILE    4
#define ERROR_CAN_NOT_ALLOCATE_MEMORY    5

enum GDPL_log_type {DEBUG, INFO, WARNING, ERROR};

enum GDPL_log_type GDPL_log_level;

struct GDPL_couple_data_node {
	int id;
	int male_id;
	int female_id;
	int start_nr;
	char start_time[16];
	char finish_time[16];
	double assignment_score;	
	struct GDPL_person_data_node *next;
};

struct GDPL_person_data_node {
	int id;
	char fname[64];
	char lname[64];
	struct GDPL_person_data_node *next;
};

struct GDPL_competition_data_node {
	int id;
	int year;
	struct GDPL_person_data_node *person_list_root_ptr;
	struct GDPL_couple_data_node *couple_list_root_ptr;
	struct GDPL_competition_data_node *next;
};

extern char* GDPL_controller_gdplib_name;
extern char* GDPL_controller_gdplib_version;
extern char* GDPL_controller_error_codes[];
extern char GDPL_controller_data_file_name[];
extern struct GDPL_competition_data_node *GDPL_controller_competition_list_root_ptr;
extern struct GDPL_competition_data_node *GDPL_controller_competition_list_selected_ptr;

/* Controller */

int GDPL_controller_set_filename(const char *filename);

int GDPL_controller_save_to_file();

int GDPL_controller_read_from_file();

/* Competition */

struct GDPL_competition_data_node* GDPL_competition_create_empty_node(); 

/* Util */

void GDPL_util_log(enum GDPL_log_type, const char*, char*, ...);

/* Test */

int GDPL_test();

#endif //GDPL_H
