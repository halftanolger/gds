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
#include "konkurranse.h"
#include "log.h"
#include "modell.h"
#include "kontroller.h"

int GDPL_konkurranse_opprett_node(GDPL_konkurranse_data_node **new_node)
{
  const char* signatur = "GDPL_konkurranse_opprett_node(GDPL_konkurranse_data_node**)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

  if ((*new_node) != 0) {
      int feilkode = FEILKODE_KAN_IKKE_ALLOKERE_MINNE_S;
      GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      return feilkode;
  }
  
  *new_node = (GDPL_konkurranse_data_node*)
               malloc (sizeof (GDPL_konkurranse_data_node));

  if (*new_node == 0) {
    int feilkode = FEILKODE_KAN_IKKE_ALLOKERE_MINNE;
    GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);    
    return feilkode;
  }

  (*new_node)->id = 0;
  (*new_node)->aar = 0;
  (*new_node)->person_liste_root_ptr = 0;
  (*new_node)->par_liste_root_ptr = 0;
  (*new_node)->neste = 0;

  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

  return 0;
}

int GDPL_konkurranse_legg_til(GDPL_konkurranse_data_node *data)
{
  const char* signatur = "GDPL_konkurranse_legg_til(GDPL_konkurranse_data_node,GDPL_konkurranse_data_node*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
  
  /* Root skal være satt nå. */

  GDPL_konkurranse_data_node *root = gdpl_modell_konkurranseliste_root_ptr;

  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "root == 0");
    return FEILKODE_FEIL;  
  }

  /* Litt inputparameter-sjekking. */

  if (data->id == 0) {
    GDPL_log(GDPL_ERROR, signatur, "data.id == 0");
    return FEILKODE_FEIL;  
  }
  
  if (data->aar == 0) {
    GDPL_log(GDPL_ERROR, signatur, "data.aar == 0");
    return FEILKODE_FEIL;  
  }
  
  if (data->person_liste_root_ptr == 0) {
    GDPL_log(GDPL_ERROR, signatur, "data.person_liste_root_ptr == 0");
    return FEILKODE_FEIL;
  }

  if (data->par_liste_root_ptr == 0) {
    GDPL_log(GDPL_ERROR, signatur, "data.par_liste_root_ptr == 0");
    return FEILKODE_FEIL;
  }

  /* Sjekk om ny id finnes fra før i lista. */
  int id_eksisterer = 0;
  GDPL_konkurranse_data_node *runner = root;
  
  while (runner->neste != 0) {
    if (data->id == runner->id) {
      id_eksisterer = 1;
      break;
    }
    runner = runner->neste;
  }
  if (data->id == runner->id) {
    id_eksisterer = 1;
  }
  
  if (id_eksisterer != 0) {
    int feilkode = FEILKODE_ID_EKSISTERER;
    GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    return feilkode;
  }
  
  /* Legg inn noden sist i lista. */
    
  if (root->neste == 0) {
    root->neste = data;
    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
  }

  runner = root;
  while (runner->neste != 0) {
    runner = runner->neste;
  }  
  runner->neste = data;
  
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

int GDPL_konkurranse_fjern_fra(GDPL_konkurranse_data_node* data)
{
  const char* signatur = "GDPL_konkurranse_fjern_fra(GDPL_konkurranse_data_node,GDPL_konkurranse_data_node*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
  
  GDPL_konkurranse_data_node *root = gdpl_modell_konkurranseliste_root_ptr;

  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_root_ptr er null");
    return FEILKODE_FEIL;
  }

  if (data->id == 0) {
    GDPL_log(GDPL_ERROR, signatur, "data.id == 0");
    return FEILKODE_FEIL;  
  }
  
  /* Sjekk om id finnes i lista. */
  int id_eksisterer = 0;
  GDPL_konkurranse_data_node *runner = root;
  
  while (runner->neste != 0) {
    if (data->id == runner->id) {
      id_eksisterer = 1;
      break;
    }
    runner = runner->neste;
  }
  if (data->id == runner->id) {
    id_eksisterer = 1;
  }
  
  if (id_eksisterer != 1) {
    int feilkode = FEILKODE_ID_EKSISTERER_IKKE;
    GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    return feilkode;
  }
  
  GDPL_konkurranse_data_node *runner2 = root;
  
  while (runner2->neste != runner) {
    runner2 = runner2->neste;
  }
    
  /* Fjern noden fra lista. */

  runner2->neste = runner->neste;
  
  free(runner);
  
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

  return 0;

}

int GDPL_konkurranse_sett_valgt_konkurranse(int id)
{
    const char* signatur = "GDPL_konkurranse_sett_valgt_konkurranse(int id)";
    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon. id=%d",id);

    GDPL_konkurranse_data_node *data = 0;

    if (GDPL_konkurranse_hent(id, &data)>0) {
        return FEILKODE_FEIL;
    }

    gdpl_modell_konkurranseliste_valgt_ptr = data;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon. ok");

    return 0;
}

int GDPL_konkurranse_hent(int id, GDPL_konkurranse_data_node **data)
{
  const char* signatur = "GDPL_konkurranse_hent(int,GDPL_konkurranse_data_node,GDPL_konkurranse_data_node*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
  
  GDPL_konkurranse_data_node *root = gdpl_modell_konkurranseliste_root_ptr;

  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "root == 0");
    return FEILKODE_FEIL;  
  }

  /* Litt inputparameter-sjekking. */

  if (*data != 0) {
    GDPL_log(GDPL_ERROR, signatur, "data != 0");
    return FEILKODE_FEIL;  
  }
  
  /* Sjekk om id finnes i lista. */

  int id_eksisterer = 0;
  GDPL_konkurranse_data_node *runner = root;
  
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

int GDPL_konkurranse_antall_i_liste(int *antall)
{
  const char* signatur = "GDPL_konkurranse_antall_i_liste(int *antall,GDPL_konkurranse_data_node*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
  
  GDPL_konkurranse_data_node *root = gdpl_modell_konkurranseliste_root_ptr;

  if (root == 0) {
    GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_root_ptr er null");
    return FEILKODE_FEIL;  
  }

  if (antall == 0) {
    GDPL_log(GDPL_ERROR, signatur, "antall må peke på et heltall, antall er null");    
    return FEILKODE_FEIL;  
  }
    
  int teller = 0;    
  while (root->neste != 0) {
    teller++;
    root = root->neste;
  }
  
  *antall = teller;
  
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

  return 0;

}
