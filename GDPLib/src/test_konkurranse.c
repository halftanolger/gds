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
int GDPL_test_konkurranse_legg_til_a() 
{
  const char* signatur = "GDPL_test_konkurranse_legg_til_a()";

  GDPL_konkurranse_data_node node;
  GDPL_konkurranse_data_node *root;
  
  root =  0;
  node.id = 1;
  node.aar = 2014;
  
  if (GDPL_konkurranse_legg_til(node,root) == 0) {
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
int GDPL_test_konkurranse_legg_til_b() 
{
  const char* signatur = "GDPL_test_konkurranse_legg_til_b()";

  GDPL_konkurranse_data_node node;
  GDPL_konkurranse_data_node *root;
  
  root =  gdpl_kontroller_konkurranseliste_root_ptr;
  node.id = 0; 
  node.aar = 2014;
  
  if (GDPL_konkurranse_legg_til(node,root) == 0) {
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 
    
  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Ny node mangler aar.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_konkurranse_legg_til_c() 
{
  const char* signatur = "GDPL_test_konkurranse_legg_til_c()";

  GDPL_konkurranse_data_node node;
  GDPL_konkurranse_data_node *root;
  
  node.id = 1; 
  node.aar = 0;
  
  if (GDPL_konkurranse_opprett_node(&root) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en konkurranse root peker!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;  
  }
  
  if (GDPL_konkurranse_legg_til(node,root) == 0) {
	free(root);
    GDPL_log(GDPL_DEBUG, signatur, "Uups. Klarte å opprette en ny node som manler aar!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 
    
  free(root);
	
  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Ny node har id som finnes i lista.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_konkurranse_legg_til_d() 
{
  const char* signatur = "GDPL_test_konkurranse_legg_til_d()";

  int antall = 0;     
  GDPL_konkurranse_data_node node;
  GDPL_konkurranse_data_node *root;
      
  node.id = 1; 
  node.aar = 2014;
  
  if (GDPL_konkurranse_opprett_node(&root) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en konkurranse root peker!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;  
  }
	  
  GDPL_konkurranse_antall_i_liste(&antall, root);
  GDPL_log(GDPL_DEBUG, signatur, "antall = %d (skal være 0)", antall);
  
  if (GDPL_konkurranse_legg_til(node,root) != 0) {
	free(root);
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en ny node i konkurranse-lista!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 

  GDPL_konkurranse_antall_i_liste(&antall, root);
  GDPL_log(GDPL_DEBUG, signatur, "antall = %d (skal være 1)", antall);
  
  if (GDPL_konkurranse_legg_til(node,root) == 0) {
    free(root);
    GDPL_log(GDPL_DEBUG, signatur, "Uups. Klarte å opprette en ny node med samme id, i konkurranse-lista!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 
	
  free(root);
	
  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Liste root ptr ikke satt. 
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_konkurranse_fjern_fra_a() 
{
  const char* signatur = "GDPL_test_konkurranse_fjern_fra_a()";

  GDPL_konkurranse_data_node node;
  GDPL_konkurranse_data_node *root;
  
  root = 0;  
  node.id = 1;
  node.aar = 2014;
    
  if (GDPL_konkurranse_fjern_fra(node,root) == 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Uups. Klarte å fjerne node fra liste uten root!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 
    
  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Fjerner node som mangler id.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_konkurranse_fjern_fra_b() 
{
  const char* signatur = "GDPL_test_konkurranse_fjern_fra_b()";

  GDPL_konkurranse_data_node node;
  GDPL_konkurranse_data_node *root;
    
  node.id = 0; 
  node.aar = 2014;
  
  if (GDPL_konkurranse_opprett_node(&root) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en konkurranse root peker!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;  
  }
	      
  if (GDPL_konkurranse_fjern_fra(node,root) == 0) {
    free(root);
    GDPL_log(GDPL_DEBUG, signatur, "Uups. Klarte å fjerne node som manglet id!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 
    
  free(root);
  
  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Fjern node med id som ikke finnes i lista.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_konkurranse_fjern_fra_c() 
{
  const char* signatur = "GDPL_test_konkurranse_fjern_fra_c()";

  int antall = 0;     
  GDPL_konkurranse_data_node node;
  GDPL_konkurranse_data_node *root;
    
  node.id = 2; 
  node.aar = 2014;
  
  if (GDPL_konkurranse_opprett_node(&root) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en konkurranse root peker!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;  
  }
    
  if (GDPL_konkurranse_fjern_fra(node,root) == 0) {
    free(root);
    GDPL_log(GDPL_DEBUG, signatur, "Uups. Klarte å fjerne en node med id som ikke finnes i lista!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 

  free(root);
    	
  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Fjern node som ble lagt inn av GDPL_test_konkurranse_legg_til_d() 
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_konkurranse_fjern_fra_d() 
{
  const char* signatur = "GDPL_test_konkurranse_fjern_fra_d()";

  int antall = 0;     
  GDPL_konkurranse_data_node node;
  GDPL_konkurranse_data_node *root;
    
  node.id = 1; 
  node.aar = 2014;
  
  if (GDPL_konkurranse_opprett_node(&root) != 0) {
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en konkurranse root peker!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;  
  }
  
  if (GDPL_konkurranse_legg_til(node,root) != 0) {
    free(root);
    GDPL_log(GDPL_DEBUG, signatur, "Klarer ikke å opprette en ny node i konkurranse-lista!");
    GDPL_log(GDPL_INFO, signatur, "Test feilet");
    return 1;
  } 
    
  GDPL_konkurranse_antall_i_liste(&antall, root);
  GDPL_log(GDPL_DEBUG, signatur, "antall = %d", antall);
  
  if (GDPL_konkurranse_fjern_fra(node,root) != 0) {
    free(root);
    GDPL_log(GDPL_INFO, signatur, "Test feilet a");
    return 1;
  } 
  
  GDPL_konkurranse_antall_i_liste(&antall, root);
  GDPL_log(GDPL_DEBUG, signatur, "antall = %d", antall);

  free(root);
  
  GDPL_log(GDPL_INFO, signatur, "Test ok");
  return 0;  
}
	


