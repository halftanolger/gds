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

#ifndef _GDPL_H_
#define _GDPL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>


//
// 'Diverse'
//
// Definerer ulike globale varibler og funksjoner, slikt som
// datastrukturen til biblioteket og ulike hjelpefunksjoner som logging
// og filhandtering.
//

#include "diverse.h"


#include "kontroller.h"


#include "konkurranse.h"


/* Funksjoner */

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

#ifdef __cplusplus
}
#endif

#endif //GDPL_H
