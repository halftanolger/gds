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

extern int GDPL_test_kontroller_angi_filnavn_a();
extern int GDPL_test_kontroller_angi_filnavn_b();
extern int GDPL_test_kontroller_angi_filnavn_c();
extern int GDPL_test_kontroller_angi_filnavn_d();

extern int GDPL_test_kontroller_les_fra_fil_a();
extern int GDPL_test_kontroller_les_fra_fil_b();

extern int GDPL_test_konkurranse_legg_til_a();
extern int GDPL_test_konkurranse_legg_til_b();
extern int GDPL_test_konkurranse_legg_til_c();
extern int GDPL_test_konkurranse_legg_til_d();

extern int GDPL_test_konkurranse_fjern_fra_a();
extern int GDPL_test_konkurranse_fjern_fra_b();
extern int GDPL_test_konkurranse_fjern_fra_c();
extern int GDPL_test_konkurranse_fjern_fra_d();

extern int GDPL_test_person_legg_til_a();
extern int GDPL_test_person_legg_til_b();
extern int GDPL_test_person_legg_til_c();
extern int GDPL_test_person_legg_til_d();
extern int GDPL_test_person_legg_til_e();


/* 
 * Intern hjelpefunksjon. 
 */
int GDPL_run_test(int (*function_ptr)()) { return (*function_ptr)(); }

/* ----------------------------------------------------------------------------
 * Funksjon
 *  int GDPL_test() 
 * ---------------------------------------------------------------------------- 
 * Beskrivelse 
 *  Kjører 'test-settet' til GDPLib. 
 * 
 * Parametre
 *  Ingen
 *
 * Returnerer
 *  0 - ok
 *  alt annet - antall tester som feilet.
 * 
 * Eksempel på bruk
 *  int r;
 *  r = GDPL_test();
 *  if (r > 0) 'find out why'
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test() 
{
  const char* signatur = "GDPL_test()";

  int r;
  int antall_tester;
  int antall_tester_ok; 
  int antall_tester_feil;

  GDPL_log(DEBUG, signatur, "Start funksjon.");
  
 /* 
  * Dette er en tabell med pekere til testfunksjoner. Enhver testfunksjon skal
  * 'stå på egne ben'. Det skal ikke være innbyrdes avhengighet mellom dem.
  * Tilsammen definerer de test-settet til GDPLib.
  */
  int (*function_ptr_array[])() = { 
    
    GDPL_test_kontroller_angi_filnavn_a,   /* 1  Filnavn ikke angitt, i.e. null peker. */
    GDPL_test_kontroller_angi_filnavn_b,   /* 2  Filnavn for kort. */
    GDPL_test_kontroller_angi_filnavn_c,   /* 3  Filnavn for langt. */
    GDPL_test_kontroller_angi_filnavn_d,   /* 4  Filnavn ok. */

    GDPL_test_kontroller_les_fra_fil_a,    /* 5  Fil eksisterer ikke. */
    GDPL_test_kontroller_les_fra_fil_b,    /* 6  Fil eksisterer. */

    GDPL_test_konkurranse_legg_til_a,      /* 7  Liste root ptr ikke satt. */
    GDPL_test_konkurranse_legg_til_b,      /* 8  Ny node mangler id. */
    GDPL_test_konkurranse_legg_til_c,      /* 9  Ny node mangler aar. */
    GDPL_test_konkurranse_legg_til_d,      /* 10 Ny node har id som finnes i lista. */

    GDPL_test_konkurranse_fjern_fra_a,     /* 11 Liste root ptr ikke satt. */
    GDPL_test_konkurranse_fjern_fra_b,     /* 12 Node mangler id. */	
    GDPL_test_konkurranse_fjern_fra_c,     /* 13 Node har id som ikke finnes i lista. */
    GDPL_test_konkurranse_fjern_fra_d,     /* 14 Fjern node lagt inn av GDPL_test_konkurranse_legg_til_d*/

    GDPL_test_person_legg_til_a,           /* 15 Liste root ptr ikke satt. */
    GDPL_test_person_legg_til_b,           /* 16 Ny node mangler id. */
    GDPL_test_person_legg_til_c,           /* 17 Ny node mangler fornavn. */
    GDPL_test_person_legg_til_d,           /* 18 Ny node mangler etternavn. */	
    GDPL_test_person_legg_til_e            /* 19 Ny node har id som finnes i lista. */
	
	
  };

  antall_tester = 19;
  antall_tester_ok = 0; 
  antall_tester_feil = 0;
  
  for (r = 0; r < antall_tester; r++) {
    if (GDPL_run_test( function_ptr_array[r] ) == 0) 
      antall_tester_ok++;
    else
      antall_tester_feil++;
  }
  
  GDPL_log(INFO, signatur, "GDPLib testsett består av %d tester. [%d ok, %d ikke ok]", 
                             antall_tester, antall_tester_ok, antall_tester_feil);

  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return antall_tester_feil;
}


