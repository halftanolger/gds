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

#include <stdlib.h>
#include "gdpl.h"

extern const char *gdpl_kontroller_feilkoder[]; 

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  GDPL_konkurranse_data_node* GDPL_konkurranse_opprett_node()
 *  
 * ----------------------------------------------------------------------------
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
 *  GDPL_konkurranse_data_node *ptr = 0;
 *  ptr = GDPL_konkurranse_opprett_node();
 *  if (ptr == 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
GDPL_konkurranse_data_node* GDPL_konkurranse_opprett_node()
{
  const char* signatur = "GDPL_konkurranse_opprett_node()";

  GDPL_log(DEBUG, signatur, "Start funksjon.");

  GDPL_konkurranse_data_node *new_node;
  new_node = (GDPL_konkurranse_data_node*)
               malloc (sizeof (GDPL_konkurranse_data_node));

  if (new_node == 0) {

    int feilkode = FEILKODE_KAN_IKKE_ALLOKERE_MINNE;
    GDPL_log(ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return 0; /* Returner en nullpeker. */

  }

  new_node->id = 0;
  new_node->aar = 0;
  new_node->person_liste_root_ptr = 0;
  new_node->par_liste_root_ptr = 0;
  new_node->neste = 0;

  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return new_node;
}


