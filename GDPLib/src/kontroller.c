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
#include <stdarg.h>
#include <locale.h>
#include <time.h>     
#include "gdpl.h"

const char* gdpl_kontroller_gdplib_navn = "GDPLib";

const char* gdpl_kontroller_gdplib_versjon = "1.0";

const char *gdpl_kontroller_feilkoder[] = { 
  "FEILKODE 0: Generell feil.",
  "FEILKODE 1: Filnavnet er for langt.", 
  "FEILKODE 2: Filnavnet er for kort.",
  "FEILKODE 3: Filnavnet er ikke definert.",
  "FEILKODE 4: Kan ikke opprette datafil.",
  "FEILKODE 5: Kan lese fra datafil.",
  "FEILKODE 6: Kan skrive til datafil.",
  "FEILKODE 7: Kan allokere minne."

};

/*
 * Denne variabelen holder på datafilnavnet.
 */
char gdpl_kontroller_datafilnavn[GDPL_MAX_FILNAVN_LENGDE];

/*
 * Denne variabelen blir brukt av GDPL_log(). 
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

  GDPL_log(DEBUG, signatur, "Start funksjon.");

  if (filnavn == 0) {
  
    /* Angi default datafilnavn. */

    strcpy(gdpl_kontroller_datafilnavn, "gdp.dat");	
    
  } else {
  
    /* Angi oppgitt filnavn, såfremt det ikke er for langt eller kort. */  

    if (strlen(filnavn) > GDPL_MAX_FILNAVN_LENGDE) {

	  int feilkode = FEILKODE_DATAFILNAVN_FOR_LANGT;
      GDPL_log(ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(DEBUG, signatur, "Slutt funksjon.");
      return feilkode;
  
    }

    if (strlen(filnavn) < GDPL_MIN_FILNAVN_LENGDE) {
	  
	  int feilkode = FEILKODE_DATAFILNAVN_FOR_KORT;
      GDPL_log(ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(DEBUG, signatur, "Slutt funksjon.");
      return feilkode;
	  
    }

    strcpy(gdpl_kontroller_datafilnavn, filnavn);

  }
    
  GDPL_log(DEBUG, signatur, "Datafilnavn er nå satt til '%s'", gdpl_kontroller_datafilnavn);
  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
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
  
  GDPL_log(DEBUG, signatur, "Start funksjon.");

  /* Datafilnavn må være initiert. */
 
  if (gdpl_kontroller_datafilnavn == 0) {
  
    int feilkode = FEILKODE_DATAFILNAVN_IKKE_DEFINERT;
    GDPL_log(ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return feilkode;
	
  }

  FILE *file;
  file = fopen(gdpl_kontroller_datafilnavn,"r"); 

  if (file == 0) {

    /* Fila eksisterer ikke, opprett fil og initier grunnleggende datastruktur. */

    GDPL_log(DEBUG, signatur, "Fila %s eksisterer ikke. Prøver å opprette den.", gdpl_kontroller_datafilnavn);
    file = fopen(gdpl_kontroller_datafilnavn,"w");
    if (file == 0) {
	
      int feilkode = FEILKODE_KAN_IKKE_OPPRETTE_DATAFIL;
      GDPL_log(ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(DEBUG, signatur, "Slutt funksjon.");
      return feilkode;
	  
    }

    GDPL_konkurranse_data_node *new_node = GDPL_konkurranse_opprett_node(); 
    if(new_node == 0) {
	
      int feilkode = FEILKODE_FEIL;
      GDPL_log(ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
      GDPL_log(DEBUG, signatur, "Slutt funksjon.");
      return feilkode;
	  
    }

    gdpl_kontroller_konkurranseliste_root_ptr = new_node;
    gdpl_kontroller_konkurranseliste_valgt_ptr = 0;

    fclose(file);
    file = 0;

    GDPL_log(DEBUG, signatur, "Slutt funksjon.");
    return 0;

  } else {

    /* Fila eksisterer, les inn data i datatruktur. */

    GDPL_log(DEBUG, signatur, "Fila %s eksisterer.", gdpl_kontroller_datafilnavn);

    //TODO: les inn data fra fila og initier datastrukturer ...

  }

  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return 0;
}




















/* ----------------------------------------------------------------------------
 * Funksjon
 *  void GDPL_log(GDPL_log_type type, const char* signature, char* message, ...)
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
 *  GDPL_log(DEBUG, "main()", "Hello %s", "world"); 
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
  case DEBUG:
    strcpy(type_str,"DEBUG");
    break;
  case INFO:
    strcpy(type_str,"INFO");
    break;
  case WARNING:
    strcpy(type_str,"WARNING");
    break;
  case ERROR:
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
  
  setlocale(LC_ALL,"nb_NO.utf8");
  gdpl_log_nivaa = nivaa;
  gdpl_log_stream = stream; 

  GDPL_log(DEBUG, signatur, "Slutt funksjon.");
  return 0;
}
