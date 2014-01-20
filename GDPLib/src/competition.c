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
**   Denne fila definerer alle funksjonene som er relatert til 
**   konkurranseobjektet.
**
** GDPL_competition_create_empty_node() 
**   Oppretter en ny tom GDPL_competition_data_node, og returnerer denne.
**
**
**
******************************************************************************/

#include <stdlib.h>
#include "gdpl.h"

struct GDPL_competition_data_node *GDPL_controller_competition_list_root_ptr;

struct GDPL_competition_data_node *GDPL_controller_competition_list_selected_ptr;


/* 
 * Funksjon 
 *  struct competition_data_node* GDPL_competition_create_empty_node()
 *  
 * Beskrivelse
 *  Oppretter en ny konkurranse data node, initialiserer denne til null 
 *  og returnerer den.
 *   
 * Parametre  
 *  Ingen
 * 
 * Returnerer 
 *  0 - indikerer feil.
 *  alt annet - peker til ny tom node.
 *  
 * Eksempel på bruk
 *  struct competition_data_node *ptr = 0;
 *  ptr = GDPL_competition_create_empty_node();
 *  if (ptr == 0) 'handle failure'
 */ 

struct GDPL_competition_data_node* GDPL_competition_create_empty_node() 
{

	const char* signature = "GDPL_competition_create_empty_node()";

    GDPL_util_log(DEBUG, signature, "Enter method.");

	struct GDPL_competition_data_node *new_node;
  	new_node =  (struct GDPL_competition_data_node*)
    			malloc (sizeof (struct GDPL_competition_data_node));

	if (new_node == 0) {
    		GDPL_util_log(ERROR, signature, 
			GDPL_controller_error_codes[ERROR_CAN_NOT_ALLOCATE_MEMORY]);
		return 0;		
	}

	new_node->id = 0;
	new_node->year = 0;
    new_node->person_list_root_ptr = 0;
	new_node->couple_list_root_ptr = 0;
  	new_node->next = 0;

    GDPL_util_log(DEBUG, signature, "Exit method.");
  	return new_node;

}


