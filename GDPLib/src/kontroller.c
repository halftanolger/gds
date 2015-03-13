﻿/* 
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
#include <stdarg.h>
#include <locale.h>
#include <time.h>  
#include <assert.h>  
#include "gdpl.h"


const char* gdpl_kontroller_gdplib_navn = "GDPLib";

const char* gdpl_kontroller_gdplib_versjon = "1.0";

const char *gdpl_kontroller_feilkoder[] = {
  "",
  "FEILKODE 01: Filnavnet er for langt.",
  "FEILKODE 02: Filnavnet er for kort.",
  "FEILKODE 03: Filnavnet er ikke definert.",
  "FEILKODE 04: Kan ikke opprette datafil.",
  "FEILKODE 05: Kan ikke lese fra datafil.",
  "FEILKODE 06: Kan ikke skrive til datafil.",
  "FEILKODE 07: Kan ikke allokere minne.",
  "FEILKODE 08: Id eksisterer allerede.",
  "FEILKODE 09: Id eksisterer ikke.",
  "FEILKODE 10: Generell feil.",
  "FEILKODE 11: Oppgitt verdi for max antall par, er for lavt.",
  "FEILKODE 12: Oppgitt verdi for max antall par, er for høyt.",
  "FEILKODE 13: Oppgitt verdi for max antall par, er ikke angitt."
};


/*
 * Definerer max antall par.
 */
int gdpl_max_antall_par;

/*
 * Denne variabelen holder på datafilnavnet.
 */
char gdpl_kontroller_datafilnavn[GDPL_MAX_FILNAVN_LENGDE];

/*
 * Denne variabelen blir brukt av GDPL_log(GDPL_).
 */
GDPL_log_type gdpl_log_nivaa;

/*
 * Denne variabelen blir brukt av GDPL_log(). 
 */
FILE * gdpl_log_stream; 

/*
 * Peker på root noden i konkurranselista. (Om vi har flere enn ett renn.)
 */
GDPL_konkurranse_data_node *gdpl_kontroller_konkurranseliste_root_ptr;

/*
 * Peker på det valgte rennet, i konkurranselista.
 */
GDPL_konkurranse_data_node *gdpl_kontroller_konkurranseliste_valgt_ptr;



/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_kontroller_hent_par_nummer(int *par_nummer, int type)
 *
 * ----------------------------------------------------------------------------  
 *
 * Beskrivelse
 *   Trekk et tilfeldig valgt par-nummer, blant de som er 
 *   ledige i settet [1, gdpl_max_antall_par]. Om gdpl_max_antall_par == -1 
 *   skal det returneres ei feilmelding. Man må angi som et parameter til 
 *   funksjonen, om man ønsker å trekke et par-nummer for ei dame, eller en 
 *   herre. 
 *
 * Parametre  
 *   par_nummer - peker til int hvor det trukne par-nummeret skal plasseres.
 *   type - 0 herrer, 1 damer. 
 * 
 * Returnerer
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 * 
 * Eksempel på bruk
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_kontroller_hent_par_nummer(int *par_id, int type) {

  const char* signatur = "GDPL_kontroller_hent_par_nummer(int*, int)";

  /* Først litt sjekking. */
    
  assert (gdpl_max_antall_par != -1);  
  assert (type == 0 || type == 1);
  assert (par_id != 0);
  
  /* Trekk et tilfeldig par-nummer, blant de som er ledige, 
     for den angitte typen; damer eller herrer. */ 
	   
  int tabell[GDPL_MAX_ANTALL_PAR];
	
  int index = 0;	
  for (index = 0; index < GDPL_MAX_ANTALL_PAR; index++) {
    tabell[index] = -1;
  }
  	
  GDPL_konkurranse_data_node *vkp = gdpl_kontroller_konkurranseliste_valgt_ptr;	    
  GDPL_par_data_node *runner = (GDPL_par_data_node *)vkp->par_liste_root_ptr;
  
  assert (runner != 0);
  assert (runner->neste != 0);
      
  runner = runner->neste;	
		
  /* Finn hvilke par som er ledig, gitt typen. */ 
  index = 0;  
  do {  	  
    GDPL_log(GDPL_DEBUG, signatur, "id=%d herre_person_id=%d dame_person_id=%d start_nr=%d",
	           runner->id, runner->herre_person_id,	runner->dame_person_id,	runner->start_nr);
		
	if (type == 0 && runner->herre_person_id == 0) { /* Ledig herre plass. */
	  tabell[index++] = runner->id;		
	} else if (type == 1 && runner->dame_person_id == 0) { /* Ledig dame plass. */	  
	  tabell[index++] = runner->id;		
	}
	
    runner = runner->neste;	
  } while (runner != 0);

  /* Ok, nå har vi oversikt over ledige par. */
  index = 0;  
  while (index < GDPL_MAX_ANTALL_PAR) {
	
	if (tabell[index] == -1) 
	  break;
		
    GDPL_log(GDPL_DEBUG, signatur, "ledig par id=%d (for type = %s)",
	           tabell[index], type==0?"herre":"dame");	
	index++;	
  }
	
  /* Velg et tilfeldig par i settet. */	
  int seed;  
  seed = time(NULL);
  srand(seed);  
  *par_id = tabell[rand() % index];
  
  GDPL_log(GDPL_DEBUG, signatur, "Par-node med id=%d er ledig for %s -registrering.",*par_id, type == 0?"herre":"dame");
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}


/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_kontroller_angi_max_antall_par(int antall)
 *
 * ----------------------------------------------------------------------------  
 *
 * Beskrivelse
 * 
 * Parametre  
 * 
 * Returnerer
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 * 
 * Eksempel på bruk
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_kontroller_angi_max_antall_par(int antall)
{
  const char* signatur = "GDPL_kontroller_angi_max_antall_par(int)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

  if (antall < 1) {
      int feilkode = FEILKODE_MAX_ANTALL_PAR_FOR_LAVT;
      GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
      return feilkode;    
  } 

  if (antall > GDPL_MAX_ANTALL_PAR) {
      int feilkode = FEILKODE_MAX_ANTALL_PAR_FOR_STORT;
      GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
      return feilkode;    
  } 

  gdpl_max_antall_par = antall;
      
  GDPL_log(GDPL_DEBUG, signatur, "Max antall par er nå satt til '%d'", gdpl_max_antall_par);
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_kontroller_hent_max_antall_par(int *antall)
 *
 * ----------------------------------------------------------------------------  
 *
 * Beskrivelse
 * 
 * Parametre  
 * 
 * Returnerer
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 * 
 * Eksempel på bruk
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_kontroller_hent_max_antall_par(int *antall)
{
  const char* signatur = "GDPL_kontroller_hent_max_antall_par(int)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

  if (antall == 0) {
    int feilkode = FEILKODE_FEIL;
    GDPL_log(GDPL_DEBUG, signatur, "antall == 0, i.e. pekeren er ikke initiert!");
    GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return feilkode;  
  }
  
  *antall = gdpl_max_antall_par;
      
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}








/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_kontroller_angi_filnavn(const char *filnavn) 
 *
 * ----------------------------------------------------------------------------  
 *
 * Beskrivelse
 *  Angi navnet til den datafila som skal brukes. Om navnet er en nullpeker,
 *  skal datafilnavnet settes til 'gds.dat'.
 * 
 * Parametre  
 *  filename - et filnavn, eller tallet 0 for å indikere et default navn.
 * 
 * Returnerer
 *  0 - ok
 *  alt annet - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 * 
 * Eksempel på bruk
 *  GDPL_kontroller_sett_filnavn("minfil.dat");
 *  GDPL_kontroller_sett_filnavn(0);
 * 
 * ----------------------------------------------------------------------------
 */ 
int GDPL_kontroller_angi_filnavn(const char *filnavn)
{
  const char* signatur = "GDPL_kontroller_angi_filnavn(const char*)";

  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

  if (filnavn == 0) {
  
    /* Angi default datafilnavn. */

    strcpy(gdpl_kontroller_datafilnavn, "gdp.dat");	
    
  } else {
  
    /* Angi oppgitt filnavn, såfremt det ikke er for langt eller kort. */  

    if (strlen(filnavn) > GDPL_MAX_FILNAVN_LENGDE) {
      int feilkode = FEILKODE_DATAFILNAVN_FOR_LANGT;
      GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
      return feilkode;  
    }

    if (strlen(filnavn) < GDPL_MIN_FILNAVN_LENGDE) {  
      int feilkode = FEILKODE_DATAFILNAVN_FOR_KORT;
      GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
      return feilkode;  
    }

    strcpy(gdpl_kontroller_datafilnavn, filnavn);

  }
    
  GDPL_log(GDPL_DEBUG, signatur, "Datafilnavn er nå satt til '%s'", gdpl_kontroller_datafilnavn);
  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_kontroller_les_fra_fil() 
 *
 * ---------------------------------------------------------------------------- 
 *
 * Beskrivelse
 *  Forutsetningen for denne funksjonen, er at et datafilnavn er angitt.
 *  Hvis det eksisterer ei slik fil, så skal vi lese data fra fila 
 *  og initiere interne datastrukturer. Hvis fila ikke eksisterer, skal
 *  fila opprettes og en root node til konkurranselista opprettes.
 *   
 * Parametre
 *  Ingen
 *
 * Return 
 *  0 - ok
 *  alt annet  - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 * 
 * Eksempel på bruk
 *  int error_nr; 
 *  error_nr = GDPL_kontroller_les_fra_fil()
 *  if (error_nr > 0) 'handle error'
 *
 * ---------------------------------------------------------------------------- 
 */ 
int GDPL_kontroller_les_fra_fil()
{
  const char* signatur = "GDPL_kontroller_les_fra_fil()";
  
  GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

  /* Datafilnavn må være initiert. */
 
  if (gdpl_kontroller_datafilnavn == 0) {
  
    int feilkode = FEILKODE_DATAFILNAVN_IKKE_DEFINERT;
    GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return feilkode;

  }

  FILE *file;
  file = fopen(gdpl_kontroller_datafilnavn,"r"); 

  if (file == 0) {

    /* Fila eksisterer ikke, opprett fil og initier grunnleggende datastruktur. */

    GDPL_log(GDPL_DEBUG, signatur, "Fila %s eksisterer ikke. Prøver å opprette den.", gdpl_kontroller_datafilnavn);
    file = fopen(gdpl_kontroller_datafilnavn,"w");
    if (file == 0) {

      int feilkode = FEILKODE_KAN_IKKE_OPPRETTE_DATAFIL;
      GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
      return feilkode;
  
    }

    GDPL_konkurranse_data_node *new_node; 
    int feilkode = GDPL_konkurranse_opprett_node(&new_node);
	if (feilkode != 0) {
      GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
      return feilkode;
    }

    gdpl_kontroller_konkurranseliste_root_ptr = new_node;
    gdpl_kontroller_konkurranseliste_valgt_ptr = 0;

    fclose(file);
    file = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;

  } else {

    /* Fila eksisterer, les inn data i datatruktur. */

    GDPL_log(GDPL_DEBUG, signatur, "Fila %s eksisterer.", gdpl_kontroller_datafilnavn);

    //TODO: les inn data fra fila og initier datastrukturer ...

  }

  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}

/* ----------------------------------------------------------------------------
 * Funksjon
 *  void GDPL_log(GDPL_GDPL_log_type type, const char* signature, char* message, ...)
 * ---------------------------------------------------------------------------- 
 * Beskrivelse
 *  Skriv ei logg melding med tidsangivelse. To globale variabler styrer hva 
 *  som skal logges og hvor det skal logges;
 *
 *  gdpl_log_stream - definerer hvor; stdout, stderr, eller til fil.
 *  gdpl_log_nivaa  - definerer minimumsnivået; DEBUG, INFO, WARNING.
 * 
 * Parametre  
 *  type - DEBUG, INFO, WARNING, ERROR
 *  signatur - navn på funksjon
 *  melding - loggmeldinga
 *  ... - variablet sett med argumenter til loggmeldinga, ala printf()
 * 
 * Returnerer
 *  ingen ting.
 * 
 * Eksempel på bruk
 *  GDPL_log(GDPL_DEBUG, "main()", "Hello %s", "world");
 * ----------------------------------------------------------------------------
 */ 
void GDPL_log(GDPL_log_type type, const char* signatur, const char* melding, ...)
{
  time_t rawtime;
  struct tm * timeinfo; 
  va_list ap;
  char *p, *sval;
  char type_str[16] = "";
  int ival;
  double dval;
  
  if (gdpl_log_nivaa > type)
    return;

  switch (type) {
  case GDPL_DEBUG:
    strcpy(type_str,"DEBUG");
    break;
  case GDPL_INFO:
    strcpy(type_str,"INFO");
    break;
  case GDPL_WARNING:
    strcpy(type_str,"WARNING");
    break;
  case GDPL_ERROR:
    strcpy(type_str,"ERROR");
    break;
  default:
    strcpy(type_str,"");
  }

  time (&rawtime);
  timeinfo = localtime (&rawtime);
  char time_str[32];
  sprintf(time_str,"%d:%d:%d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

  fprintf(gdpl_log_stream,"%s %s %s: ",type_str, time_str, signatur);

  va_start(ap, melding);
  for (p = (char*)melding; *p; p++) {
    if (*p != '%') {
      fputc(*p,gdpl_log_stream);
      continue;
    }
    switch (*++p) {
    case 'd':
      ival = va_arg(ap, int);
      fprintf(gdpl_log_stream,"%d", ival);
      break;
    case 'f':
      dval = va_arg(ap, double);
      fprintf(gdpl_log_stream,"%f", dval);
      break;
    case 's':
      for (sval = va_arg(ap, char *); *sval; sval++)
        putchar(*sval);
      break;
    default:
      fputc(*p,gdpl_log_stream);
      break;
    }
  }
 
  fprintf(gdpl_log_stream, "\n");
  fflush(gdpl_log_stream);
}

/* ----------------------------------------------------------------------------
 *
 * Funksjon 
 *  int GDPL_init() 
 *
 * ---------------------------------------------------------------------------- 
 *
 * Beskrivelse
 *  Initier GDPLib.
 *   
 * Parametre
 *  nivaa - DEBUG, INFO, WARNING eller ERROR
 *  stream - stdout, stderr eller en annen filstream.
 *
 * Return 
 *  0 - ok
 *  alt annet  - feil. Den returnerte verdien kan brukes som indeks til 
 *  gdpl_kontroller_feilkoder[] for å hente ut ei feilmelding.
 * 
 * Eksempel på bruk
 *  int error_nr; 
 *  error_nr = GDPL_initier()
 *  if (error_nr > 0) 'handle error'
 *
 * ---------------------------------------------------------------------------- 
 */ 
int GDPL_init(GDPL_log_type nivaa, FILE * stream) 
{
  const char* signatur = "GDPL_init";
  
  /* Initier gdpl_max_antall_par til -1,
     noe som indikerer at denne ikke er satt. */
  gdpl_max_antall_par = -1;
  
  /* Sett locale til 'norsk bokmål', 
     noe som bør fikse øæå -problematikk. */
  setlocale(LC_ALL,"nb_NO.utf8");
  
  /* Sett loggnivå og loggdestinasjon ihht inputparametre. */
  gdpl_log_nivaa = nivaa;
  gdpl_log_stream = stream; 

  GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
  return 0;
}
