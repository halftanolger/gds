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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gdpl.h"

extern GDPL_konkurranse_data_node *gdpl_kontroller_konkurranseliste_root_ptr;
extern GDPL_konkurranse_data_node *gdpl_kontroller_konkurranseliste_valgt_ptr;
extern char gdpl_kontroller_datafilnavn[GDPL_MAX_FILNAVN_LENGDE];


/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Liste root ptr ikke satt. 
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_par_legg_til_a() 
{
  const char* signatur = "GDPL_test_par_legg_til_a()";

  GDPL_par_data_node node;
  GDPL_par_data_node *root = 0;
  
  root =  0;
  node.id = 1;
  //node.fnavn = strdup("Ola");
  //node.enavn = strdup("Nordmann");
  
  /* root er null, dette skal feile. */
  if (GDPL_par_legg_til(node,root) == 0) {
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 

  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Ny node mangler id.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_par_legg_til_b() 
{
  const char* signatur = "GDPL_test_par_legg_til_b()";
  
  /* Sett opp en valgt konkurranse. */
  
  GDPL_konkurranse_data_node *root_konkurranse = 0; 
  GDPL_konkurranse_data_node *valgt_konkurranse = 0;
  GDPL_konkurranse_data_node node_konkurranse;
  
  node_konkurranse.id = 123;
  node_konkurranse.aar = 2014;
  
  /* Opprett root-konkurranse-node */ 
  if (GDPL_konkurranse_opprett_node(&root_konkurranse) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en konkurranse!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;  
  }
 
  /* Opprett en konkurranse i konkurranse-lista */ 
  if (GDPL_konkurranse_legg_til(node_konkurranse, root_konkurranse) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en konkurranse!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;  
  }
    
  /* Hent ut en konkurranse, og initier 'valgt_konkurranse' med denne. */	
  if (GDPL_konkurranse_hent(123, &valgt_konkurranse, root_konkurranse) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å hente en konkurranse!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;    
  }
  
  /* Opprett en root-par-node i den valgte konkurransen. */
  GDPL_par_data_node *root;
  if (GDPL_par_opprett_node(&root) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette et root par!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;  
  }
      
  /* Alt er nå satt opp, kjør test. */
  
  GDPL_par_data_node node;
  node.id = 0;
  //node.fnavn = strdup("Ola");
  //node.enavn = strdup("Nordmann");
  
  if (GDPL_par_legg_til(node,root) == 0) {  
	free(root);
    free(valgt_konkurranse);
    free(root_konkurranse);
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 
  
  free(root);  
  free(valgt_konkurranse);
  free(root_konkurranse);
    
  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}



