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
#include "gdpl.h"

extern const char *gdpl_kontroller_feilkoder[]; 

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_par_opprett_node(GDPL_par_data_node**)
 *  
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Oppretter en ny person-data-node og  initialiserer denne.  
 *   
 * Parametre  
 *  Peker til peker til node
 * 
 * Returnerer 
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *  
 * Eksempel på bruk
 *  GDPL_par_data_node *ptr;
 *  int fk = GDPL_par_opprett_node(&ptr);
 *  if (fk != 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_par_opprett_node(GDPL_par_data_node **new_node)
{
  const char* signatur = "GDPL_par_opprett_node(GDPL_par_data_node**)";

  GDPL_log(DEBUG, signatur, "Start funksjon.");
  
  *new_node = (GDPL_par_data_node*)
               malloc (sizeof (GDPL_par_data_node));

  if (*new_node == 0) {
    int feilkode = FEILKODE_KAN_IKKE_ALLOKERE_MINNE;
    GDPL_log(ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return feilkode;
  }

  (*new_node)->id = 0;
  (*new_node)->herre_person_id = 0;
  (*new_node)->dame_person_id = 0;
  (*new_node)->start_nr = 0;
  (*new_node)->oppgave_poeng = 0;
  strcpy((*new_node)->start_tid,"00:00:00");  
  strcpy((*new_node)->maal_tid,"00:00:00");  
      
  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_par_legg_til(GDPL_par_data_node, GDPL_par_data_node*)
 *  
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Legg til en par-node i par-lista.
 *   
 * Parametre  
 *  GDPL_par_data_node  - noden som skal legges til lista.
 *  GDPL_par_data_node* - peker til root-node i parlista.
 * 
 * Returnerer 
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *  
 * Eksempel på bruk
 *  GDPL_par_data_node node;
 *  GDPL_par_data_node *root;
 *  int feilnr;
 *  feilnr = GDPL_par_legg_til(node,root);
 *  if (feilnr != 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_par_legg_til(GDPL_par_data_node data, GDPL_par_data_node *root)
{
  const char* signatur = "GDPL_par_legg_til(GDPL_par_data_node,GDPL_par_data_node*)";

  GDPL_log(DEBUG, signatur, "Start funksjon.");
  
  /* Litt inputparameter-sjekking. */
  
  if (root == 0) {
    GDPL_log(DEBUG, signatur, "root == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }

  if (data.id == 0) {
    GDPL_log(DEBUG, signatur, "data.id == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
  
/*  
  if (data.fnavn == 0) {
    GDPL_log(DEBUG, signatur, "data.fnavn == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }

  if (strlen(data.fnavn) > GDPL_MAX_PERSONNAVN_LENGDE) {
    GDPL_log(DEBUG, signatur, "data.fnavn for langt, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
  
  if (strlen(data.fnavn) < GDPL_MIN_PERSONNAVN_LENGDE) {
    GDPL_log(DEBUG, signatur, "data.fnavn for kort, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
    
  if (data.enavn == 0) {
    GDPL_log(DEBUG, signatur, "data.enavn == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
  
  if (strlen(data.enavn) > GDPL_MAX_PERSONNAVN_LENGDE) {
    GDPL_log(DEBUG, signatur, "data.lnavn for langt, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
  
  if (strlen(data.enavn) < GDPL_MIN_PERSONNAVN_LENGDE) {
    GDPL_log(DEBUG, signatur, "data.lnavn for kort, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
	
*/	
	
  /* Sjekk om ny id finnes fra før i lista. */
  int id_eksisterer = 0;
  GDPL_par_data_node *runner = root;
        
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
  
  if (id_eksisterer != 0) {
    int feilkode = FEILKODE_ID_EKSISTERER;
    GDPL_log(DEBUG, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");  
    return feilkode;
  }
    
  /* Legg inn noden sist i lista. */
  
  GDPL_par_data_node *new_node;
  
  int feilkode = GDPL_par_opprett_node(&new_node);
  if (feilkode != 0) {
    GDPL_log(ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);  
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");  
    return feilkode;  
  }
    
  new_node->id = data.id;  
  new_node->herre_person_id = data.herre_person_id;
  new_node->dame_person_id = data.dame_person_id;      
  new_node->start_nr = data.start_nr;
  new_node->oppgave_poeng = data.oppgave_poeng;
  strcpy(new_node->start_tid,data.start_tid);
  strcpy(new_node->maal_tid,data.maal_tid);        	  	  	  	 	 
	  
  if (root->neste == 0) {
    root->neste = new_node;
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return 0;
  }

  runner = root;
  while (runner->neste != 0) {
    runner = runner->neste;
  }  
  runner->neste = new_node;
  
  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_par_fjern_fra(GDPL_par_data_node,
 *                                   GDPL_par_data_node*)
 *  
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Fjern en par-node fra par-lista.
 *   
 * Parametre  
 *  GDPL_par_data_node  - noden som skal fjernes fra lista.
 *  GDPL_par_data_node* - peker til root-node i parlista.
 * 
 * Returnerer 
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *  
 * Eksempel på bruk
 *  GDPL_par_data_node node;
 *  GDPL_par_data_node *root;
 *  int feilnr;
 *  feilnr = GDPL_par_fjern_fra(node,root);
 *  if (feilnr != 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_par_fjern_fra(GDPL_par_data_node data, GDPL_par_data_node *root)
{
  const char* signatur = "GDPL_par_fjern_fra(GDPL_par_data_node,GDPL_par_data_node*)";

  GDPL_log(DEBUG, signatur, "Start funksjon.");
  
  /* Litt inputparameter-sjekking. */
  
  if (root == 0) {
    GDPL_log(DEBUG, signatur, "root == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }

  if (data.id == 0) {
    GDPL_log(DEBUG, signatur, "data.id == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
  
  /* Sjekk om id finnes i lista. */
  int id_eksisterer = 0;
  GDPL_par_data_node *runner = root;
  
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
    GDPL_log(DEBUG, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");  
    return feilkode;
  }
  
  GDPL_par_data_node *runner2 = root;
  
  while (runner2->neste != runner) {
    runner2 = runner2->neste;
  }
    
  /* Fjern noden fra lista. */

  runner2->neste = runner->neste;
  
  free(runner);
  
  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_par_hent(int id, GDPL_par_data_node*,
 *                         GDPL_par_data_node*)
 *  
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Hent en par-node i par-lista, gitt en id.
 *   
 * Parametre  
 *  int                  - id til den noden som skal hentes.
 *  GDPL_par_data_node*  - noden som hentes fra lista.
 *  GDPL_par_data_node*  - peker til root-node i parlista.
 * 
 * Returnerer 
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *  
 * Eksempel på bruk
 *  int id;
 *  GDPL_par_data_node *node;
 *  GDPL_par_data_node *root;
 *  int feilnr;
 *  feilnr = GDPL_par_hent(id,node,root);
 *  if (feilnr != 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_par_hent(int id, GDPL_par_data_node **data, GDPL_par_data_node *root)
{
  const char* signatur = "GDPL_par_hent(int,GDPL_par_data_node,GDPL_par_data_node*)";

  GDPL_log(DEBUG, signatur, "Start funksjon.");
  
  /* Litt inputparameter-sjekking. */
  
  if (root == 0) {
    GDPL_log(DEBUG, signatur, "root == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }

  if (*data != 0) {
    GDPL_log(DEBUG, signatur, "data != 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
  
  /* Sjekk om id finnes i lista. */
  int id_eksisterer = 0;
  GDPL_par_data_node *runner = root;
  
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
    GDPL_log(DEBUG, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");  
    return feilkode;
  }
    
  *data = runner;
  
  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_par_antall_i_liste(int *antall, GDPL_par_data_node*)
 *  
 * ----------------------------------------------------------------------------
 *
 * Beskrivelse
 *  Beregn antall noder i par-lista.
 *   
 * Parametre  
 *  int* - peker til en heltallsvariabel som svaret skal legges i.
 *  GDPL_par_data_node* - peker til root-node i parlista.
 * 
 * Returnerer 
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 *  
 * Eksempel på bruk
 *  int antall;
 *  GDPL_par_data_node *root;
 *  int feilnr;
 *  feilnr = GDPL_par_antall_i_liste(&antall,root);
 *  if (feilnr != 0) 'handle failure'
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_par_antall_i_liste(int *antall, GDPL_par_data_node *root)
{
  const char* signatur = "GDPL_par_antall_i_liste(int *antall,GDPL_par_data_node*)";

  GDPL_log(DEBUG, signatur, "Start funksjon.");
  
  /* Litt inputparameter-sjekking. */
  
  if (root == 0) {
    GDPL_log(DEBUG, signatur, "root == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }

  if (antall == 0) {
    GDPL_log(DEBUG, signatur, "antall == 0, I'm out of here ...");
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return FEILKODE_FEIL;  
  }
    
  int teller = 0;
  GDPL_par_data_node *runner = root;
  
  while (runner != 0) {
    teller++;
    runner = runner->neste;
  }
    
  if (root->neste != 0) {
	teller--; /* Ikke tell med root-noden. */
  }	
	
  *antall = teller;
  
  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return 0;
}
