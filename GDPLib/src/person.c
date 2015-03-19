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
#include <string.h>
#include "person.h"
#include "kontroller.h"
#include "konkurranse.h"
#include "log.h"


int GDPL_person_opprett_node(GDPL_person_data_node **new_node)
{
  const char* signatur = "GDPL_person_opprett_node(GDPL_person_data_node**)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
    
  if ((*new_node) != 0) {
      int feilkode = FEILKODE_KAN_IKKE_ALLOKERE_MINNE_S;
      GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      return feilkode;
  }

  *new_node = (GDPL_person_data_node*)
               malloc (sizeof (GDPL_person_data_node));

  if (*new_node == 0) {
    int feilkode = FEILKODE_KAN_IKKE_ALLOKERE_MINNE;
    GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return feilkode;
  }

  (*new_node)->id = 0;
  strcpy((*new_node)->fnavn,"nop");
  strcpy((*new_node)->enavn,"nop");
  (*new_node)->neste = 0;

  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

  return 0;
}


int GDPL_person_legg_til(GDPL_person_data_node *data)
{
  const char* signatur = "GDPL_person_legg_til(GDPL_person_data_node,GDPL_person_data_node*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
    	
  GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
  if (kroot == 0) {
      GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
      return FEILKODE_FEIL;
  }

  GDPL_person_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr;
  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr er null");
    return FEILKODE_FEIL;
  }

  if (root->neste == 0) {
    root->neste = data;
    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon");
    return 0;
  }

  int teller = 0;  
  while (root->neste != 0) {
    root = root->neste;
    teller++;
  }  
  root->neste = data;
  
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon");

  return 0;

}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_person_fjern_fra(GDPL_person_data_node,
 *                                   GDPL_person_data_node*)
 *  
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Fjern en person-node fra person-lista.
 *   
 * Parametre  
 *  GDPL_person_data_node  - noden som skal fjernes fra lista.
 *  GDPL_person_data_node* - peker til root-node i personlista.
 * 
 * Returnerer 
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *  
 * Eksempel på bruk
 *  GDPL_person_data_node node;
 *  GDPL_person_data_node *root;
 *  int feilnr;
 *  feilnr = GDPL_person_fjern_fra(node,root);
 *  if (feilnr != 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_person_fjern_fra(GDPL_person_data_node data)
{
  const char* signatur = "GDPL_person_fjern_fra(GDPL_person_data_node,GDPL_person_data_node*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
  
  GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
  if (kroot == 0) {
      GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
      return FEILKODE_FEIL;
  }

  GDPL_person_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr;
  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr er null");
    return FEILKODE_FEIL;
  }


  /* Litt inputparameter-sjekking. */
  
  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "root == 0");
    return FEILKODE_FEIL;  
  }

  if (data.id == 0) {
    GDPL_log(GDPL_ERROR, signatur, "data.id == 0");
    return FEILKODE_FEIL;  
  }
  
  /* Sjekk om id finnes i lista. */
  int id_eksisterer = 0;
  GDPL_person_data_node *runner = root;
  
  while (runner->neste != 0) {
    if (data.id == runner->id) {
      id_eksisterer = 1;
      break;
    }
    runner = runner->neste;
  }
  if (data.id == runner->id) {
    id_eksisterer = 1;
  }
  
  if (id_eksisterer != 1) {
    int feilkode = FEILKODE_ID_EKSISTERER_IKKE;
    GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    return feilkode;
  }
  
  GDPL_person_data_node *runner2 = root;
  
  while (runner2->neste != runner) {
    runner2 = runner2->neste;
  }
    
  /* Fjern noden fra lista. */

  runner2->neste = runner->neste;
  
  free(runner->fnavn);
  free(runner->enavn);
  free(runner);
  
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_person_hent(int id, GDPL_person_data_node*,
 *                         GDPL_person_data_node*)
 *  
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Hent en person-node i person-lista, gitt en id.
 *   
 * Parametre  
 *  int                     - id til den noden som skal hentes.
 *  GDPL_person_data_node*  - noden som hentes fra lista.
 *  GDPL_person_data_node*  - peker til root-node i personlista.
 * 
 * Returnerer 
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *  
 * Eksempel på bruk
 *  int id;
 *  GDPL_person_data_node *node;
 *  GDPL_person_data_node *root;
 *  int feilnr;
 *  feilnr = GDPL_person_hent(id,node,root);
 *  if (feilnr != 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_person_hent(int id, GDPL_person_data_node **data)
{
  const char* signatur = "GDPL_person_hent(int,GDPL_person_data_node,GDPL_person_data_node*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
  
  GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
  if (kroot == 0) {
      GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
      return FEILKODE_FEIL;
  }

  GDPL_person_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr;
  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr er null");
    return FEILKODE_FEIL;
  }

  /* Litt inputparameter-sjekking. */
  
  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "root -peker er null");
    return FEILKODE_FEIL;  
  }

  if ((*data) != 0) {
    GDPL_log(GDPL_ERROR, signatur, "data -peker må være null.");
    return FEILKODE_FEIL;  
  }
  
  /* Sjekk om id finnes i lista. */
  int id_eksisterer = 0;
  GDPL_person_data_node *runner = root;
  
  while (runner->neste != 0) {
    if (id == runner->id) {
      id_eksisterer = 1;
      break;
    }
    runner = runner->neste;
  }
  if (id == runner->id) {
    id_eksisterer = 1;
  }
  
  if (id_eksisterer != 1) {
    int feilkode = FEILKODE_ID_EKSISTERER_IKKE;
    GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    return feilkode;
  }
    
  *data = runner;
  
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_person_antall_i_liste(int *antall, GDPL_person_data_node*)
 *  
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Beregn antall noder i person-lista.
 *   
 * Parametre  
 *  int* - peker til en heltallsvariabel som svaret skal legges i.
 *  GDPL_person_data_node* - peker til root-node i personlista.
 * 
 * Returnerer 
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *  
 * Eksempel på bruk
 *  int antall;
 *  GDPL_person_data_node *root;
 *  int feilnr;
 *  feilnr = GDPL_person_antall_i_liste(&antall,root);
 *  if (feilnr != 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_person_antall_i_liste(int *antall)
{
  const char* signatur = "GDPL_person_antall_i_liste(int *antall,GDPL_person_data_node*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
  
  GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
  if (kroot == 0) {
      GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
      return FEILKODE_FEIL;
  }

  GDPL_person_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr;
  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr er null");
    return FEILKODE_FEIL;
  }

  /* Litt inputparameter-sjekking. */
  
  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "root == 0");
    return FEILKODE_FEIL;  
  }

  if (antall == 0) {
    GDPL_log(GDPL_ERROR, signatur, "antall == 0");
    return FEILKODE_FEIL;  
  }
    
  int teller = 0;
  GDPL_person_data_node *runner = root;
	
  while (runner->neste != 0) {
    teller++;
    runner = runner->neste;
  }
  
  if (root->neste != 0) {
	teller--; /* Ikke tell med root-noden. */
  }
  
  *antall = teller;
  
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}







int GDPL_person_finn_neste_ledige_id(int *id)
{
  const char* signatur = "GDPL_person_finn_neste_ledige_id(..)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

  GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
  if (kroot == 0) {
      GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
      return FEILKODE_FEIL;
  }

  GDPL_person_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr;
  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->person_liste_root_ptr er null");
    return FEILKODE_FEIL;
  }

  if (id == 0) {
    GDPL_log(GDPL_ERROR, signatur, "id må peke på et heltall, id peker på null");
    return FEILKODE_FEIL;
  }

  /* Finn neste unike id mht person-lista. */

  int tmp_id = 1;
  do {
      int i = root->id;
      if (tmp_id <= i) {
        tmp_id = i + 1;
      }
      root = root->neste;
  } while (root != 0);
  *id = tmp_id;

  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon. (id=%d)",*id);
  return 0;
}

