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


extern int GDPL_test_kontroller_hent_par_nummer_a();

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

extern int GDPL_test_par_legg_til_a();
extern int GDPL_test_par_legg_til_b();


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

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
  
 /* 
  * Dette er en tabell med pekere til testfunksjoner. Enhver testfunksjon skal
  * 'stå på egne ben'. Det skal ikke være innbyrdes avhengighet mellom dem.
  * Tilsammen definerer de test-settet til GDPLib.
  */
  int (*function_ptr_array[])() = { 
    



  //  GDPL_test_par_legg_til_a,               /* Liste root ptr ikke satt. */
  //  GDPL_test_par_legg_til_b                /* Ny node mangler id. */
	
	
  };

  antall_tester = 1;
  antall_tester_ok = 0; 
  antall_tester_feil = 0;
  
  for (r = 0; r < antall_tester; r++) {
    if (GDPL_run_test( function_ptr_array[r] ) == 0) 
      antall_tester_ok++;
    else
      antall_tester_feil++;
  }
  
  GDPL_log(GDPL_INFO, signatur, "GDPLib testsett består av %d tester. [%d ok, %d ikke ok]",
                             antall_tester, antall_tester_ok, antall_tester_feil);

  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return antall_tester_feil;
}


