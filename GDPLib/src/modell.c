
#include <string.h>
#include "modell.h"
#include "diverse.h"
#include "kontroller.h"
#include "konkurranse.h"

char gdpl_modell_datafilnavn[GDPL_MAX_FILNAVN_LENGDE] = "";

GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_root_ptr;

GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_valgt_ptr;


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


int GDPL_modell_les_inn_fra_eksisterende_fil(FILE* file)
{

    const char* signatur = "GDPL_modell_opprett_ny_fil()";
    int feilkode = 0;

    /* Fila eksisterer, les inn data i datatruktur. */

    GDPL_log(GDPL_DEBUG, signatur, "Fila %s eksisterer.", gdpl_modell_datafilnavn);

    /* Sjekk om den inneholder noe data, e.g. har size > 0 */

    fseek(file,0L, SEEK_END);
    size_t sz = ftell(file);

    if (sz == 0) {

        const char* msg = "Fila eksisterer, men er tom. Dette er feil!";
        GDPL_log(GDPL_DEBUG, signatur, msg);

        feilkode = FEILKODE_FEIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    }

    fseek(file,0L,0L);

    int mt = 0;
    sz = fread(&mt,sizeof(int),1,file);
    GDPL_log(GDPL_DEBUG, signatur, "sz=%d",sz);

    if (mt != 12345) {

        const char* msg = "Fila starter ikke med det 'magiske tallet'; 12345, i.e. dette er ikke ei korrekt datafil!";
        GDPL_log(GDPL_DEBUG, signatur, msg);

        feilkode = FEILKODE_FEIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;

    }

    GDPL_log(GDPL_DEBUG, signatur, "Les inn data fra fil.");

    GDPL_konkurranse_data_node *konkurranse_data_node_root = 0;

    GDPL_konkurranse_data_node *konkurranse_data_node_fokus = 0;
    GDPL_konkurranse_data_node *konkurranse_data_node;

    GDPL_person_data_node *person_data_node_fokus = 0;
    GDPL_person_data_node *person_data_node;

    GDPL_par_data_node *par_data_node_fokus = 0;
    GDPL_par_data_node *par_data_node;

    do {

        /* Opprett en konkurranse-data-node */

        feilkode = GDPL_konkurranse_opprett_node(&konkurranse_data_node);
        if (feilkode != 0) {
            GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
            GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
            return feilkode;
        }

        if (konkurranse_data_node_root == 0) {
            konkurranse_data_node_root = konkurranse_data_node;
        }

        if (konkurranse_data_node_fokus == 0) {
            konkurranse_data_node_fokus = konkurranse_data_node;
        } else {
            konkurranse_data_node_fokus->neste = konkurranse_data_node;
            konkurranse_data_node_fokus = konkurranse_data_node;
        }

        /* Fyll konkurranse-data-noden med data fra fil. */

        sz = fread(konkurranse_data_node,sizeof(GDPL_konkurranse_data_node),1,file);
        GDPL_log(GDPL_DEBUG, signatur, "konkurranse_data_node sz=%d",sz);

        /* Enhver konkurranse-data-node skal inneholde en root-person-data-node.
           Denne root-person-data-noden kan ha person-data-noder 'under seg'.*/

        do {

            /* Opprett en person-data-node */

            feilkode = GDPL_person_opprett_node(&person_data_node);
            if (feilkode != 0) {
                GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
                GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
                return feilkode;
            }

            if (konkurranse_data_node->person_liste_root_ptr == 0) {
                konkurranse_data_node->person_liste_root_ptr = person_data_node;
            }

            if (person_data_node_fokus == 0) {
                person_data_node_fokus = person_data_node;
            } else {
                person_data_node_fokus->neste = person_data_node;
                person_data_node_fokus = person_data_node;
            }

            /* Fyll person-data-noden med data fra fil. */

            sz = fread(person_data_node,sizeof(GDPL_person_data_node),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "person_data_node sz=%d",sz);

        } while (person_data_node->neste != 0);

        /* Enhver konkurranse-data-node skal inneholde en root-par-data-node.
           Denne root-par-data-noden kan ha par-data-noder 'under seg'.*/

        do {

            /* Opprett en par-data-node */

            feilkode = GDPL_par_opprett_node(&par_data_node);
            if (feilkode != 0) {
                GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
                GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
                return feilkode;
            }

            if (konkurranse_data_node->par_liste_root_ptr == 0) {
                konkurranse_data_node->par_liste_root_ptr = par_data_node;
            }

            if (par_data_node_fokus == 0) {
                par_data_node_fokus = par_data_node;
            } else {
                par_data_node_fokus->neste = par_data_node;
                par_data_node_fokus = par_data_node;
            }

            /* Fyll par-data-noden med data fra fil. */

            sz = fread(par_data_node,sizeof(GDPL_par_data_node),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "par_data_node sz=%d",sz);

        } while (par_data_node->neste != 0);

    } while (konkurranse_data_node->neste != 0);

   gdpl_modell_konkurranseliste_root_ptr = konkurranse_data_node_root;
   gdpl_modell_konkurranseliste_valgt_ptr = 0;

    fclose(file);
    file = 0;


    //-------------------------------------------------------------------------------
    GDPL_log(GDPL_DEBUG, signatur, "Debugging");
    GDPL_konkurranse_data_node *a = 0;
    GDPL_person_data_node *b = 0;
    GDPL_par_data_node *c= 0;

    a =gdpl_modell_konkurranseliste_root_ptr;
    b = a->person_liste_root_ptr;
    c = a->par_liste_root_ptr;

    if (a != 0) {
        GDPL_log(GDPL_DEBUG, signatur, "a ok id=%d",a->id);
    }
    if (b != 0) {
        GDPL_log(GDPL_DEBUG, signatur, "b ok id=%d",b->id);
    }
    if (c != 0) {
        GDPL_log(GDPL_DEBUG, signatur, "c ok id=%d",c->id);
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;

}


int GDPL_modell_opprett_ny_fil()
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

    GDPL_person_data_node *root_person_data_node;
    feilkode = GDPL_person_opprett_node(&root_person_data_node);
    if (feilkode != 0) {
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;
    }

    GDPL_par_data_node *root_par_data_node;
    feilkode = GDPL_par_opprett_node(&root_par_data_node);
    if (feilkode != 0) {
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;
    }

    GDPL_konkurranse_data_node *root_konkurranse_data_node;
    feilkode = GDPL_konkurranse_opprett_node(&root_konkurranse_data_node);
    if (feilkode != 0) {
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return feilkode;
    }

    root_konkurranse_data_node->person_liste_root_ptr = root_person_data_node;
    root_konkurranse_data_node->par_liste_root_ptr = root_par_data_node;

    /* initier hoved-root-pekeren og indiker samtidig at ingen konkurranse
       enda ikke er valgt */

   gdpl_modell_konkurranseliste_root_ptr = root_konkurranse_data_node;
   gdpl_modell_konkurranseliste_valgt_ptr = 0;

    fclose(file);
    file = 0;

    /* Skriv denne noden til fila. */

    feilkode = GDPL_modell_skriv_til_fil();
    if (feilkode > 0) {
        return feilkode;
    }

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
int GDPL_modell_les_fra_fil()
{
    const char* signatur = "GDPL_modell_les_fra_fil()";
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
    file = fopen(gdpl_modell_datafilnavn,"r");

    if (file == 0) {

        return GDPL_modell_opprett_ny_fil();

    } else {

        feilkode = GDPL_modell_les_inn_fra_eksisterende_fil(file);

        fclose(file);
        file = 0;

        return feilkode;

    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}





int GDPL_modell_skriv_til_fil()
{
    const char* signatur = "GDPL_modell_skriv_til_fil()";
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

    /* Skriv magiskt tall til fila. */

    int mt = 12345;
    fwrite(&mt,sizeof(int),1,file);

    /* Skriv data til fil. */

    GDPL_konkurranse_data_node *konkurranse_data_node =gdpl_modell_konkurranseliste_root_ptr;
    GDPL_person_data_node *person_data_node;
    GDPL_par_data_node *par_data_node;

    do {

        int konkurranse_data_node_id = konkurranse_data_node->id;
        int konkurranse_data_node_aar = konkurranse_data_node->aar;
        int konkurranse_data_node_har_neste = konkurranse_data_node->neste != 0?1:0;

        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse id=%d til fil.",konkurranse_data_node_id);
        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse aar=%d til fil.",konkurranse_data_node_aar);
        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse harNeste=%d til fil.",konkurranse_data_node_har_neste);

        fwrite(&konkurranse_data_node_id,sizeof(int),1,file);
        fwrite(&konkurranse_data_node_aar,sizeof(int),1,file);
        fwrite(&konkurranse_data_node_har_neste,sizeof(int),1,file);

        person_data_node = konkurranse_data_node->person_liste_root_ptr;
        if (person_data_node == 0) {
            GDPL_log(GDPL_DEBUG, signatur, "konkurranse id=%d har nullptr som person root.",konkurranse_data_node->id);
            feilkode = FEILKODE_FEIL;
            GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
            GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
            return feilkode;
        } else {
            GDPL_log(GDPL_DEBUG, signatur, "konkurranse id=%d har ok ptr som person root.",konkurranse_data_node->id);
        }

        do {

            int person_data_node_id = person_data_node->id;
            char person_data_node_fnavn[GDPL_MAX_PERSONNAVN_LENGDE];
            char person_data_node_enavn[GDPL_MAX_PERSONNAVN_LENGDE];
            strcpy(person_data_node_fnavn,person_data_node->fnavn);
            strcpy(person_data_node_enavn,person_data_node->enavn);
            int person_data_node_har_neste = person_data_node->neste != 0?1:0;

            GDPL_log(GDPL_DEBUG, signatur, "Skriver person id=%d til fil.",person_data_node_id);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver person fnavn=%s til fil.",person_data_node_fnavn);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver person enavn=%s til fil.",person_data_node_enavn);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver person harNeste=%d til fil.",person_data_node_har_neste);

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
            int par_data_node_startnr = par_data_node->startnr;
            int par_data_node_oppgave_poeng = par_data_node->oppgave_poeng;
            char par_data_node_start_tid[GDPL_MAX_TID_LENGDE];
            char par_data_node_maal_tid[GDPL_MAX_TID_LENGDE];
            strcpy(par_data_node_start_tid,par_data_node->start_tid);
            strcpy(par_data_node_maal_tid,par_data_node->maal_tid);
            int par_data_node_har_neste = par_data_node->neste != 0?1:0;

            GDPL_log(GDPL_DEBUG, signatur, "Skriver par id=%d til fil.",par_data_node_id);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par herre_person_id=%d til fil.",par_data_node_herre_person_id);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par dame_person_id=%d til fil.",par_data_node_dame_person_id);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par startnr=%d til fil.",par_data_node_startnr);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par oppgave_poeng=%d til fil.",par_data_node_oppgave_poeng);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par start_tid=%s til fil.",par_data_node_start_tid);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par maal_tid=%s til fil.",par_data_node_maal_tid);
            GDPL_log(GDPL_DEBUG, signatur, "Skriver par harNeste=%d til fil.",par_data_node_har_neste);

            fortsett her ...

            fwrite(&person_data_node_id,sizeof(int),1,file);
            fwrite(&person_data_node_fnavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            fwrite(&person_data_node_enavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            fwrite(&person_data_node_har_neste,sizeof(int),1,file);





            par_data_node = par_data_node->neste;
        } while (par_data_node != 0);
        konkurranse_data_node = konkurranse_data_node->neste;
    } while (konkurranse_data_node != 0);

    fclose(file);
    file = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}



