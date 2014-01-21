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
#include "gdpl.h"

extern GDPL_konkurranse_data_node *gdpl_kontroller_konkurranseliste_root_ptr;
extern GDPL_konkurranse_data_node *gdpl_kontroller_konkurranseliste_valgt_ptr;
extern char gdpl_kontroller_datafilnavn[GDPL_MAX_FILNAVN_LENGDE];



int GDPL_test_kontroller_angi_filnavn_a();
int GDPL_test_kontroller_angi_filnavn_b();
int GDPL_test_kontroller_angi_filnavn_c();
int GDPL_test_kontroller_angi_filnavn_d();

int GDPL_test_kontroller_les_fra_fil_a();
int GDPL_test_kontroller_les_fra_fil_b();

/* 
 * Intern hjelpefunksjon. 
 */
int GDPL_run_test(int (*function_ptr)()) { return (*function_ptr)(); }

/* ----------------------------------------------------------------------------
 * Funksjon
 *  int GDPL_test() 
 * ---------------------------------------------------------------------------- 
 * Beskrivelse 
 *  Kjører 'test settet' til GDPLib. 
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

 /* 
  * Dette er en tabell med pekere til testfunksjoner. 
  */
  int (*function_ptr_array[])() = { 
    
    GDPL_test_kontroller_angi_filnavn_a,   /* Filnavn ikke angitt, i.e. null peker. */
    GDPL_test_kontroller_angi_filnavn_b,   /* Filnavn for kort. */
    GDPL_test_kontroller_angi_filnavn_c,   /* Filnavn for langt. */
    GDPL_test_kontroller_angi_filnavn_d,   /* Filnavn ok. */
	

    GDPL_test_kontroller_les_fra_fil_a,    /* Fil eksisterer ikke.*/
    GDPL_test_kontroller_les_fra_fil_b     /* Fil eksisterer. */

  };

  antall_tester = 6;
  antall_tester_ok = 0; 
  antall_tester_feil = 0;

  GDPL_log(INFO, signatur, "GDPLib testsettet består av %d tester.", antall_tester);
  for (r = 0; r < antall_tester; r++) {
    if (GDPL_run_test( function_ptr_array[r] ) == 0) 
      antall_tester_ok++;
    else
      antall_tester_feil++;
  }
  GDPL_log(INFO, signatur, "GDPLib testsett er ferdig. [%d ok, %d ikke ok]", antall_tester_ok, antall_tester_feil);

  return antall_tester_feil;
}


/*
 *
 * ===== GDPLib testsett ======================================================
 *
 */


/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 *
 *   Datafila eksisterer ikke på filsystemet. Fila må da opprettes, og 
 *   interne datastrukturer må initieres.
 *
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_kontroller_les_fra_fil_a()
{
  const char* signatur = "GDPL_test_kontroller_les_fra_fil_a()";

  /* Klargjør for testtilfellet. */
  
  int tilfeldig_tall, seed;
  char tilfeldig_filnavn[128];
  seed = time(NULL);
  srand(seed);
  tilfeldig_tall = rand();
  sprintf(tilfeldig_filnavn,"%d_tempfil.dat",tilfeldig_tall);
  GDPL_log(DEBUG, signatur, "Opprettet temoprært filnavn: %s", tilfeldig_filnavn);
 
  GDPL_kontroller_angi_filnavn(tilfeldig_filnavn);
  GDPL_kontroller_les_fra_fil();

  /* Test, fila skal eksistere. */
  
  FILE *fp = fopen(tilfeldig_filnavn,"r");
  if (fp == 0) {
    GDPL_log(INFO, signatur, "Test feilet.");
    return 1;
  }
  fclose(fp);
  fp = 0;
  remove(tilfeldig_filnavn); 

  /* Test, konkurranseliste_root_ptr skal være initiert. */
  
  if (gdpl_kontroller_konkurranseliste_root_ptr == 0) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  }
  if (gdpl_kontroller_konkurranseliste_root_ptr->id != 0) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  }
  if (gdpl_kontroller_konkurranseliste_root_ptr->aar != 0) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  }
  if (gdpl_kontroller_konkurranseliste_root_ptr->person_liste_root_ptr != 0) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  }
  if (gdpl_kontroller_konkurranseliste_root_ptr->par_liste_root_ptr != 0) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  }
  if (gdpl_kontroller_konkurranseliste_root_ptr->neste != 0) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  }

  GDPL_log(INFO, signatur, "Test ok");
  return 0;
}

/* ---------------------------------------------------------------------------- 
 * Testbeskrivelse
 * 
 *   Datafila eksisterer på filsystemet. Fila må lese, og 
 *   interne datastrukturer må initieres ihht innhold i fila.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_kontroller_les_fra_fil_b()
{
  const char* signatur = "GDPL_test_kontroller_les_fra_fil_b()";

  GDPL_log(INFO, signatur, "Test feilet - siden den ikke er implementer ...");
  return 1;
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Filnavn ikke angitt, i.e. null peker.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_kontroller_angi_filnavn_a() 
{
  const char* signatur = "GDPL_test_kontroller_angi_filnavn_a()";

  GDPL_kontroller_angi_filnavn(0);

  if (strcmp("gdp.dat",gdpl_kontroller_datafilnavn) == 0) {
    GDPL_log(INFO, signatur, "Test ok");
    return 0;
  } else {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  }

}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Filnavn for kort.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_kontroller_angi_filnavn_b() 
{
  const char* signatur = "GDPL_test_kontroller_angi_filnavn_b()";

  char for_kort_filnavn[GDPL_MAX_FILNAVN_LENGDE];

  int index;
  if (GDPL_MIN_FILNAVN_LENGDE > 1) {
	index = GDPL_MIN_FILNAVN_LENGDE - 1;
  } else {
	index = 1;
  }
  
  memset(&for_kort_filnavn,'x',GDPL_MAX_FILNAVN_LENGDE);
  for_kort_filnavn[index]='\0';

  int a = GDPL_kontroller_angi_filnavn(for_kort_filnavn);
  int b = FEILKODE_DATAFILNAVN_FOR_KORT;

  if (a != b) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  } 

  GDPL_log(INFO, signatur, "Test ok");
  return 0;	
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Filnavn for langt.
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_kontroller_angi_filnavn_c() 
{
  const char* signatur = "GDPL_test_kontroller_angi_filnavn_c()";

  char for_langt_filnavn[GDPL_MAX_FILNAVN_LENGDE + 1];

  memset(&for_langt_filnavn,'x',GDPL_MAX_FILNAVN_LENGDE + 1);
  for_langt_filnavn[GDPL_MAX_FILNAVN_LENGDE + 1]='\0';

  int a = GDPL_kontroller_angi_filnavn(for_langt_filnavn);
  int b = FEILKODE_DATAFILNAVN_FOR_LANGT;

  if (a != b) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  } 

  GDPL_log(INFO, signatur, "Test ok");
  return 0;	
}

/* ----------------------------------------------------------------------------
 * Testbeskrivelse
 * 
 *  Filnavn ok 
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_test_kontroller_angi_filnavn_d() 
{
  const char* signatur = "GDPL_test_kontroller_angi_filnavn_d()";

  if (GDPL_kontroller_angi_filnavn("mydata.dat") != 0) {
    GDPL_log(INFO, signatur, "Test feilet");
    return 1;
  } 

  GDPL_log(INFO, signatur, "Test ok");
  return 0;
}



