
#include <string.h>
#include "modell.h"
#include "diverse.h"
#include "kontroller.h"
#include "konkurranse.h"


/* Variabel-definisjoner */


char gdpl_modell_datafilnavn[GDPL_MAX_FILNAVN_LENGDE] = "";

GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_root_ptr;

GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_valgt_ptr;


/* Funksjons-definisjoner */


/* ----------------------------------------------------------------------------
 *
 * Funksjon
 *  int GDPL_modell_angi_filnavn(const char *filnavn)
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
 *  GDPL_modell_sett_filnavn("minfil.dat");
 *  GDPL_modell_sett_filnavn(0);
 *
 * ----------------------------------------------------------------------------
 */
int GDPL_modell_angi_filnavn(const char *filnavn)
{
    const char* signatur = "GDPL_kontroller_angi_filnavn(const char*)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (filnavn == 0) {

        /* Angi default datafilnavn. */

        strcpy(gdpl_modell_datafilnavn, "gdp.dat");

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

        strcpy(gdpl_modell_datafilnavn, filnavn);

    }

    GDPL_log(GDPL_DEBUG, signatur, "Datafilnavn er nå satt til '%s'", gdpl_modell_datafilnavn);
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
 *  Utover root-noden til konkurranselista, så skal også root-noden til
 *  personlista og root-noden parlista lages og kobles til konkurranse-
 *  lista.
 *
 *
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
int GDPL_modell_les_data()
{
    const char* signatur = "GDPL_modell_les_data()";
    int feilkode = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (gdpl_modell_datafilnavn == 0) {

        const char* msg = "Datafilnavn er ikke satt/initiert. Dette er feil!";
        GDPL_log(GDPL_DEBUG, signatur, msg);

        feilkode = FEILKODE_DATAFILNAVN_IKKE_DEFINERT;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    }

    FILE *file;
    file = fopen(gdpl_modell_datafilnavn,"rb");

    if (file == 0) {

        feilkode = GDPL_modell_privat_opprett_ny_fil();
        return feilkode;

    } else {

        feilkode = GDPL_modell_privat_les_inn_fra_eksisterende_fil(file);

        fclose(file);
        file = 0;

        return feilkode;

    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}



int GDPL_modell_privat_skriv_til_fil(FILE* file)
{
    const char* signatur = "GDPL_modell_skriv_til_fil()";
    int feilkode = 0;

    /* Skriv magiskt tall til fila. */

    int mt = 12345;
    fwrite(&mt,sizeof(int),1,file);

    /* Skriv data til fil. */

    GDPL_konkurranse_data_node *konkurranse_data_node =gdpl_modell_konkurranseliste_root_ptr;
    GDPL_person_data_node *person_data_node;
    GDPL_par_data_node *par_data_node;

    do {

        GDPL_log(GDPL_DEBUG, signatur, "--------------------------------------");

        int konkurranse_data_node_id = konkurranse_data_node->id;
        int konkurranse_data_node_aar = konkurranse_data_node->aar;
        int konkurranse_data_node_har_neste = konkurranse_data_node->neste == 0?0:1;

        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse id=%d",konkurranse_data_node_id);
        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse aar=%d",konkurranse_data_node_aar);
        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse harNeste=%d",konkurranse_data_node_har_neste);

        fwrite(&konkurranse_data_node_id,sizeof(int),1,file);
        fwrite(&konkurranse_data_node_aar,sizeof(int),1,file);
        fwrite(&konkurranse_data_node_har_neste,sizeof(int),1,file);

        person_data_node = konkurranse_data_node->person_liste_root_ptr;

        do {
            int person_data_node_id = person_data_node->id;
            char person_data_node_fnavn[GDPL_MAX_PERSONNAVN_LENGDE];
            char person_data_node_enavn[GDPL_MAX_PERSONNAVN_LENGDE];
            memset(person_data_node_fnavn, 0, GDPL_MAX_PERSONNAVN_LENGDE);
            memset(person_data_node_enavn, 0, GDPL_MAX_PERSONNAVN_LENGDE);
            strcpy(person_data_node_fnavn,person_data_node->fnavn);
            strcpy(person_data_node_enavn,person_data_node->enavn);
            int person_data_node_har_neste = person_data_node->neste == 0?0:1;

            GDPL_log(GDPL_DEBUG, signatur, "Skriver person id=%d ",person_data_node_id);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver person fnavn=%s ",person_data_node_fnavn);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver person enavn=%s ",person_data_node_enavn);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver person harNeste=%d ",person_data_node_har_neste);

            fwrite(&person_data_node_id,sizeof(int),1,file);
            fwrite(&person_data_node_fnavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            fwrite(&person_data_node_enavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            fwrite(&person_data_node_har_neste,sizeof(int),1,file);

            person_data_node = person_data_node->neste;
        } while (person_data_node != 0);

        par_data_node = konkurranse_data_node->par_liste_root_ptr;
        if (par_data_node == 0) {
            GDPL_log(GDPL_DEBUG, signatur, "konkurranse id=%d har nullptr som par root.",konkurranse_data_node->id);
            feilkode = FEILKODE_FEIL;
            GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
            GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
            return feilkode;
        } else {
            GDPL_log(GDPL_DEBUG, signatur, "konkurranse id=%d har ok ptr som par root.",konkurranse_data_node->id);
        }

        do {
            int par_data_node_id = par_data_node->id;
            int par_data_node_herre_person_id = par_data_node->herre_person_id;
            int par_data_node_dame_person_id = par_data_node->dame_person_id;
            int par_data_node_start_nr = par_data_node->start_nr;
            int par_data_node_oppgave_poeng = par_data_node->oppgave_poeng;
            char par_data_node_start_tid[GDPL_MAX_TID_LENGDE];
            char par_data_node_maal_tid[GDPL_MAX_TID_LENGDE];
            memset(par_data_node_start_tid, 0, GDPL_MAX_TID_LENGDE);
            memset(par_data_node_maal_tid, 0, GDPL_MAX_TID_LENGDE);
            strncpy(par_data_node_start_tid,par_data_node->start_tid,GDPL_MAX_TID_LENGDE);
            strncpy(par_data_node_maal_tid,par_data_node->maal_tid,GDPL_MAX_TID_LENGDE);
            int par_data_node_har_neste = par_data_node->neste != 0?1:0;

            GDPL_log(GDPL_DEBUG, signatur, "Skriver par id=%d",par_data_node_id);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par herre_person_id=%d",par_data_node_herre_person_id);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par dame_person_id=%d",par_data_node_dame_person_id);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par start_nr=%d",par_data_node_start_nr);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par oppgave_poeng=%d",par_data_node_oppgave_poeng);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par start_tid=%s",par_data_node_start_tid);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par maal_tid=%s",par_data_node_maal_tid);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par harNeste=%d",par_data_node_har_neste);

            fwrite(&par_data_node_id,sizeof(int),1,file);
            fwrite(&par_data_node_herre_person_id,sizeof(int),1,file);
            fwrite(&par_data_node_dame_person_id,sizeof(int),1,file);
            fwrite(&par_data_node_start_nr,sizeof(int),1,file);
            fwrite(&par_data_node_oppgave_poeng,sizeof(int),1,file);
            fwrite(&par_data_node_start_tid,sizeof(char),GDPL_MAX_TID_LENGDE,file);
            fwrite(&par_data_node_maal_tid,sizeof(char),GDPL_MAX_TID_LENGDE,file);
            fwrite(&par_data_node_har_neste,sizeof(int),1,file);

            par_data_node = par_data_node->neste;
        } while (par_data_node != 0);
        konkurranse_data_node = konkurranse_data_node->neste;
    } while (konkurranse_data_node != 0);


    return feilkode;
}

int GDPL_modell_skriv_data()
{
    const char* signatur = "GDPL_modell_skriv_data()";
    int feilkode = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (gdpl_modell_datafilnavn == 0) {

        /* Datafilnavn er ikke initiert! */

        feilkode = FEILKODE_DATAFILNAVN_IKKE_DEFINERT;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    }

    if (gdpl_modell_konkurranseliste_root_ptr == 0) {

        /* Vi har ingen data å skrive. Root-pekeren er tom! */

        feilkode = FEILKODE_FEIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    }

    FILE *file;
    file = fopen(gdpl_modell_datafilnavn,"wb");

    if (file == 0) {

        /* Klarer ikke å åpne fila */

        feilkode = FEILKODE_FEIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    }

    GDPL_modell_privat_skriv_til_fil(file);

    fclose(file);
    file = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}



/* Private funksjons-definisjoner */


int GDPL_modell_privat_opprett_ny_fil()
{
    const char* signatur = "GDPL_modell_opprett_ny_fil()";
    int feilkode = 0;

    /* Fila eksisterer ikke, opprett fil og initier grunnleggende datastruktur. */

    GDPL_log(GDPL_DEBUG, signatur, "Fila %s eksisterer ikke. Prøver å opprette den.", gdpl_modell_datafilnavn);
    FILE* file = fopen(gdpl_modell_datafilnavn,"wb");
    if (file == 0) {

        const char* msg = "Fila eksisterer ikke, og jeg klarer ikke å opprette den heller. Dette er feil!";
        GDPL_log(GDPL_DEBUG, signatur, msg);

        feilkode = FEILKODE_KAN_IKKE_OPPRETTE_DATAFIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    }

    /* Ok, fila er opprettet. Opprett nå den interne grunnstrukturen med en
       root-konkurranse-node med tilhørende person- og par-root-noder.*/

    GDPL_konkurranse_data_node *root_konkurranse_data_node;
    feilkode = GDPL_konkurranse_opprett_node(&root_konkurranse_data_node);
    if (feilkode != 0) {
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;
    }


    /* Når man oppretter en ny konkurranse, så må man også opprette nye
       root-pekere til person og par -lista. */

    GDPL_person_data_node* node_person;
    GDPL_person_opprett_node(&node_person);

    GDPL_par_data_node* node_par;
    GDPL_par_opprett_node(&node_par);

    root_konkurranse_data_node->person_liste_root_ptr = node_person;
    root_konkurranse_data_node->par_liste_root_ptr = node_par;


    /* initier hoved-root-pekeren og indiker samtidig at ingen konkurranse
       enda ikke er valgt */

    gdpl_modell_konkurranseliste_root_ptr = root_konkurranse_data_node;
    gdpl_modell_konkurranseliste_valgt_ptr = 0;

    fclose(file);
    file = 0;

    /* Skriv denne noden til fila. */

    feilkode = GDPL_modell_skriv_data();
    if (feilkode > 0) {
        return feilkode;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;

}


int GDPL_modell_privat_les_inn_fra_eksisterende_fil(FILE* file)
{

    const char* signatur = "GDPL_modell_opprett_ny_fil()";
    int feilkode = 0;

    /* Fila eksisterer, les inn data i datatruktur. */

    GDPL_log(GDPL_DEBUG, signatur, "Fila %s eksisterer.", gdpl_modell_datafilnavn);

    /* Sjekk om den inneholder noe data, e.g. har size > 0 */

    fseek(file,0L, SEEK_END);
    size_t sz = ftell(file);

    if (sz == 0) {

        //TODO: her bør jeg kanskje kalle opp funksjonen 'opprette ei ny fil'...

        const char* msg = "Fila eksisterer, men er tom. Dette er feil!";
        GDPL_log(GDPL_DEBUG, signatur, msg);

        feilkode = FEILKODE_FEIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    }

    fseek(file,0L,0L);

    int mt = 0;
    fread(&mt,sizeof(int),1,file);

    if (mt != 12345) {

        const char* msg = "Fila starter ikke med det 'magiske tallet'; 12345, i.e. dette er ikke ei korrekt GDPLib-datafil!";
        GDPL_log(GDPL_DEBUG, signatur, msg);

        feilkode = FEILKODE_FEIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    } else {

        const char* msg = "Fila starter med det 'magiske tallet'; 12345, i.e. dette er kanskje ei korrekt GDPLib-datafil...";
        GDPL_log(GDPL_DEBUG, signatur, msg);

    }

    GDPL_log(GDPL_DEBUG, signatur, "Les inn data fra fil.");

    GDPL_konkurranse_data_node *konkurranse_data_node_root = 0;

    GDPL_konkurranse_data_node *konkurranse_data_node;
    GDPL_konkurranse_data_node *konkurranse_data_node_tmp;

    GDPL_person_data_node *person_data_node;
    GDPL_person_data_node *person_data_node_tmp;

    GDPL_par_data_node *par_data_node;
    GDPL_par_data_node *par_data_node_tmp;

    int konkurranse_data_node_har_neste = 0;
    int person_data_node_har_neste = 0;
    int par_data_node_har_neste = 0;

    do {

        /* Opprett en konkurranse-data-node */

        GDPL_konkurranse_opprett_node(&konkurranse_data_node_tmp);

        /* Fyll konkurranse-data-noden med data fra fil. */

        int konkurranse_data_node_id = 0;
        int konkurranse_data_node_aar = 0;

        fread(&konkurranse_data_node_id,sizeof(int),1,file);
        fread(&konkurranse_data_node_aar,sizeof(int),1,file);
        fread(&konkurranse_data_node_har_neste,sizeof(int),1,file);

        GDPL_log(GDPL_DEBUG, signatur, "Leser konkurranse id=%d til fil.",konkurranse_data_node_id);
        GDPL_log(GDPL_DEBUG, signatur, "Leser konkurranse aar=%d til fil.",konkurranse_data_node_aar);
        GDPL_log(GDPL_DEBUG, signatur, "Leser konkurranse harNeste=%d til fil.",konkurranse_data_node_har_neste);

        konkurranse_data_node_tmp->id = konkurranse_data_node_id;
        konkurranse_data_node_tmp->aar = konkurranse_data_node_aar;

        do {

            /* Opprett en person-data-node */

            GDPL_person_opprett_node(&person_data_node_tmp);

            /* Fyll person-data-noden med data fra fil. */

            int person_data_node_id = 0;
            char person_data_node_fnavn[GDPL_MAX_PERSONNAVN_LENGDE];
            char person_data_node_enavn[GDPL_MAX_PERSONNAVN_LENGDE];
            memset(person_data_node_fnavn, 0, GDPL_MAX_PERSONNAVN_LENGDE);
            memset(person_data_node_enavn, 0, GDPL_MAX_PERSONNAVN_LENGDE);
            int person_data_node_har_neste = 0;

            fread(&person_data_node_id,sizeof(int),1,file);
            fread(&person_data_node_fnavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            fread(&person_data_node_enavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            fread(&person_data_node_har_neste,sizeof(int),1,file);

            GDPL_log(GDPL_DEBUG, signatur, "Leser person id=%d ",person_data_node_id);
            GDPL_log(GDPL_DEBUG, signatur, "Leser person fnavn=%s ",person_data_node_fnavn);
            GDPL_log(GDPL_DEBUG, signatur, "Leser person enavn=%s ",person_data_node_enavn);
            GDPL_log(GDPL_DEBUG, signatur, "Leser person harNeste=%d ",person_data_node_har_neste);

            size_t sz = strlen(&person_data_node_fnavn);
            GDPL_log(GDPL_DEBUG, signatur, "strlen(&person_data_node_fnavn)=%d",sz);
            char *fnavn = (char*) malloc(sz);
            strncpy(fnavn,&person_data_node_fnavn,sz);

            sz = strlen(&person_data_node_enavn);
            GDPL_log(GDPL_DEBUG, signatur, "strlen(&person_data_node_enavn)=%d",sz);
            char *enavn = (char*) malloc(sz);
            strncpy(enavn,&person_data_node_enavn,sz);


            person_data_node_tmp->id = person_data_node_id;
            person_data_node_tmp->fnavn = fnavn;
            person_data_node_tmp->enavn = enavn;

            GDPL_log(GDPL_DEBUG, signatur, "a");

            if (konkurranse_data_node_tmp->person_liste_root_ptr == 0) {
                konkurranse_data_node_tmp->person_liste_root_ptr = person_data_node_tmp;
                person_data_node = person_data_node_tmp;
            } else {
                person_data_node->neste = person_data_node_tmp;
                person_data_node = person_data_node_tmp;
            }

            GDPL_log(GDPL_DEBUG, signatur, "b");

        } while (person_data_node_har_neste != 0);

        GDPL_log(GDPL_DEBUG, signatur, "Ferdig med personer, går til par.");

        do {

            /* Opprett en par-data-node */

            GDPL_par_opprett_node(&par_data_node_tmp);

            /* Fyll par-data-noden med data fra fil. */

            int par_data_node_id = 0;
            int par_data_node_herre_person_id = 0;
            int par_data_node_dame_person_id = 0;
            int par_data_node_start_nr = 0;
            int par_data_node_oppgave_poeng = 0;
            char par_data_node_start_tid[GDPL_MAX_TID_LENGDE];
            char par_data_node_maal_tid[GDPL_MAX_TID_LENGDE];
            memset(par_data_node_start_tid, 0, GDPL_MAX_TID_LENGDE);
            memset(par_data_node_maal_tid, 0, GDPL_MAX_TID_LENGDE);
            int par_data_node_har_neste = 0;

            fread(&par_data_node_id,sizeof(int),1,file);
            fread(&par_data_node_herre_person_id,sizeof(int),1,file);
            fread(&par_data_node_dame_person_id,sizeof(int),1,file);
            fread(&par_data_node_start_nr,sizeof(int),1,file);
            fread(&par_data_node_oppgave_poeng,sizeof(int),1,file);
            fread(&par_data_node_start_tid,sizeof(char),GDPL_MAX_TID_LENGDE,file);
            fread(&par_data_node_maal_tid,sizeof(char),GDPL_MAX_TID_LENGDE,file);
            fread(&par_data_node_har_neste,sizeof(int),1,file);

            GDPL_log(GDPL_DEBUG, signatur, "Leser par id=%d",par_data_node_id);
            GDPL_log(GDPL_DEBUG, signatur, "Leser par herre_person_id=%d",par_data_node_herre_person_id);
            GDPL_log(GDPL_DEBUG, signatur, "Leser par dame_person_id=%d",par_data_node_dame_person_id);
            GDPL_log(GDPL_DEBUG, signatur, "Leser par start_nr=%d",par_data_node_start_nr);
            GDPL_log(GDPL_DEBUG, signatur, "Leser par oppgave_poeng=%d",par_data_node_oppgave_poeng);
            GDPL_log(GDPL_DEBUG, signatur, "Leser par start_tid=%s",par_data_node_start_tid);
            GDPL_log(GDPL_DEBUG, signatur, "Leser par maal_tid=%s",par_data_node_maal_tid);
            GDPL_log(GDPL_DEBUG, signatur, "Leser par harNeste=%d",par_data_node_har_neste);

            par_data_node_tmp->id = par_data_node_id;
            par_data_node_tmp->herre_person_id = par_data_node_herre_person_id;
            par_data_node_tmp->dame_person_id = par_data_node_dame_person_id;
            par_data_node_tmp->start_nr = par_data_node_start_nr;
            par_data_node_tmp->oppgave_poeng = par_data_node_oppgave_poeng;
            strncpy(par_data_node_tmp->start_tid,par_data_node_start_tid,GDPL_MAX_TID_LENGDE);
            strncpy(par_data_node_tmp->maal_tid,par_data_node_maal_tid,GDPL_MAX_TID_LENGDE);

            if (konkurranse_data_node_tmp->par_liste_root_ptr == 0) {
                konkurranse_data_node_tmp->par_liste_root_ptr = par_data_node_tmp;
                par_data_node = par_data_node_tmp;
            } else {
                par_data_node->neste = par_data_node_tmp;
                par_data_node = par_data_node_tmp;
            }

        } while (par_data_node_har_neste != 0);

        if (konkurranse_data_node_root == 0) {
            konkurranse_data_node_root = konkurranse_data_node_tmp;
            konkurranse_data_node = konkurranse_data_node_tmp;
        } else {
            konkurranse_data_node->neste = konkurranse_data_node_tmp;
            konkurranse_data_node = konkurranse_data_node_tmp;
        }

    } while (konkurranse_data_node_har_neste != 0);

    gdpl_modell_konkurranseliste_root_ptr = konkurranse_data_node_root;
    gdpl_modell_konkurranseliste_valgt_ptr = 0;

    fclose(file);
    file = 0;

    int antall_konkurranser = -1;
    GDPL_konkurranse_antall_i_liste(&antall_konkurranser, gdpl_modell_konkurranseliste_root_ptr);
    GDPL_log(GDPL_DEBUG, signatur, "antall_konkurranser=%d",antall_konkurranser);

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;

}
