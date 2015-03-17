
#include <string.h>
#include <stdlib.h>
#include "modell.h"
#include "diverse.h"
#include "kontroller.h"
#include "konkurranse.h"
#include "person.h"
#include "par.h"


/* Variabel-definisjoner */


char gdpl_modell_datafilnavn[GDPL_MAX_FILNAVN_LENGDE] = "";

GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_root_ptr;

GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_valgt_ptr;


/* 'Private' funksjons-deklareringer */


int GDPL_modell_privat_opprett_ny_fil();

int GDPL_modell_privat_skriv_til_eksisterende_fil();

int GDPL_modell_privat_les_inn_fra_eksisterende_fil();

int GDPL_modell_privat_sjekk_filnavn(const char* filnavn);

int GDPL_modell_privat_slett_alt();


/* 'Public' funksjons-definisjoner */


int GDPL_modell_angi_filnavn(const char *filnavn)
{   
    const char* signatur = "GDPL_modell_angi_filnavn()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (filnavn == 0) {
        /* Angi default datafilnavn. */
        strcpy(gdpl_modell_datafilnavn, "gdp.dat");
    } else {
        if (GDPL_modell_privat_sjekk_filnavn(filnavn) > 0) {
            return FEILKODE_FEIL;
        }
        strcpy(gdpl_modell_datafilnavn, filnavn);
    }

    GDPL_log(GDPL_DEBUG, signatur, "gdpl_modell_datafilnavn=%s",gdpl_modell_datafilnavn);

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_modell_les_data()
{
    const char* signatur = "GDPL_modell_les_data()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (GDPL_modell_privat_sjekk_filnavn(gdpl_modell_datafilnavn) > 0) {
        return FEILKODE_FEIL;
    }

    FILE *fptr;
    fptr = fopen(gdpl_modell_datafilnavn,"rb+");

    if (fptr == NULL) {

        GDPL_modell_privat_opprett_ny_fil();

    } else {

        fclose(fptr);

    }

    /* Nå skal vi ha ei ok data fil å lese fra. */

    if (GDPL_modell_privat_les_inn_fra_eksisterende_fil() > 0) {
        return FEILKODE_FEIL;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_modell_skriv_data()
{
    const char* signatur = "GDPL_modell_skriv_data()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (GDPL_modell_privat_sjekk_filnavn(gdpl_modell_datafilnavn) > 0) {
        return FEILKODE_FEIL;
    }

    if (gdpl_modell_konkurranseliste_root_ptr == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_root_ptr == 0");
        return FEILKODE_FEIL;
    }

    FILE *file;
    file = fopen(gdpl_modell_datafilnavn,"w");

    if (file == 0) {

        int feilkode = FEILKODE_KAN_IKKE_OPPRETTE_DATAFIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;

    }

    fclose(file);

    if (GDPL_modell_privat_skriv_til_eksisterende_fil() > 0) {
        return FEILKODE_FEIL;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}



/* Private funksjons-definisjoner */


int GDPL_modell_privat_opprett_ny_fil()
{
    const char* signatur = "GDPL_modell_privat_opprett_ny_fil()";

    GDPL_konkurranse_data_node *root_konkurranse_data_node = 0;
    if (GDPL_konkurranse_opprett_node(&root_konkurranse_data_node) > 0) {
        return FEILKODE_FEIL;
    }

    GDPL_person_data_node* node_person = 0;
    if (GDPL_person_opprett_node(&node_person) > 0) {
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node* node_par = 0;
    if (GDPL_par_opprett_node(&node_par) > 0) {
        return FEILKODE_FEIL;
    }

    root_konkurranse_data_node->person_liste_root_ptr = node_person;
    root_konkurranse_data_node->par_liste_root_ptr = node_par;

    /* Initier globale pekere. */

    gdpl_modell_konkurranseliste_root_ptr = root_konkurranse_data_node;
    gdpl_modell_konkurranseliste_valgt_ptr = 0;

    if (GDPL_modell_skriv_data() > 0) {
        return FEILKODE_FEIL;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;

}

int GDPL_modell_privat_skriv_til_eksisterende_fil()
{
    const char* signatur = "GDPL_modell_privat_skriv_til_eksisterende_fil()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    FILE *file;
    file = fopen(gdpl_modell_datafilnavn,"w");

    if (file == NULL) {

        int feilkode = FEILKODE_KAN_IKKE_SKRIVE_TIL_DATAFIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;

    }

    /* Skriv magiskt tall til fila. */

    int mt = 12345;
    size_t sz = fwrite(&mt,sizeof(int),1,file);
    if (sz != 1) {
        GDPL_log(GDPL_ERROR, signatur, "fwrite, sz != 1");
    }

    /* Skriv data til fil. */

    GDPL_konkurranse_data_node *konkurranse_data_node = gdpl_modell_konkurranseliste_root_ptr;
    GDPL_person_data_node *person_data_node;
    GDPL_par_data_node *par_data_node;

    do {

        GDPL_log(GDPL_DEBUG, signatur, "--------------------------------------");

        int konkurranse_data_node_id = konkurranse_data_node->id;
        int konkurranse_data_node_aar = konkurranse_data_node->aar;
        int konkurranse_data_node_har_neste = konkurranse_data_node->neste == 0?0:1;

        size_t sz;
        sz = fwrite(&konkurranse_data_node_id,sizeof(int),1,file);
        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse id=%d (%d)",konkurranse_data_node_id, sz);
        sz = fwrite(&konkurranse_data_node_aar,sizeof(int),1,file);
        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse aar=%d (%d)",konkurranse_data_node_aar, sz);
        sz = fwrite(&konkurranse_data_node_har_neste,sizeof(int),1,file);
        GDPL_log(GDPL_DEBUG, signatur, "Skriver konkurranse harNeste=%d (%d)",konkurranse_data_node_har_neste, sz);

        person_data_node = konkurranse_data_node->person_liste_root_ptr;

        do {
            int person_data_node_id = person_data_node->id;
            char person_data_node_fnavn[GDPL_MAX_PERSONNAVN_LENGDE];
            char person_data_node_enavn[GDPL_MAX_PERSONNAVN_LENGDE];
            memset(&person_data_node_fnavn, 0, GDPL_MAX_PERSONNAVN_LENGDE);
            memset(&person_data_node_enavn, 0, GDPL_MAX_PERSONNAVN_LENGDE);
            strcpy((char*)&person_data_node_fnavn,person_data_node->fnavn);
            strcpy((char*)&person_data_node_enavn,person_data_node->enavn);
            int person_data_node_har_neste = person_data_node->neste == 0?0:1;

            size_t sz = 0;
            sz = fwrite(&person_data_node_id,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver person id=%d (%d) -------",person_data_node_id, sz);
            sz = fwrite(&person_data_node_fnavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver person fnavn=%s (%d)",person_data_node_fnavn, sz);
            sz = fwrite(&person_data_node_enavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver person enavn=%s (%d)",person_data_node_enavn, sz);
            sz = fwrite(&person_data_node_har_neste,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver person harNeste=%d (%d)",person_data_node_har_neste, sz);

            person_data_node = person_data_node->neste;
        } while (person_data_node != 0);

        par_data_node = konkurranse_data_node->par_liste_root_ptr;

        do {
            int par_data_node_id = par_data_node->id;
            int par_data_node_herre_person_id = par_data_node->herre_person_id;
            int par_data_node_dame_person_id = par_data_node->dame_person_id;
            int par_data_node_start_nr = par_data_node->start_nr;
            int par_data_node_tids_poeng = par_data_node->tids_poeng;
            int par_data_node_oppgave_poeng = par_data_node->oppgave_poeng;
            char par_data_node_start_tid[GDPL_MAX_TID_LENGDE];
            char par_data_node_maal_tid[GDPL_MAX_TID_LENGDE];
            memset(par_data_node_start_tid, 0, GDPL_MAX_TID_LENGDE);
            memset(par_data_node_maal_tid, 0, GDPL_MAX_TID_LENGDE);
            strcpy((char*)&par_data_node_start_tid, par_data_node->start_tid);
            strcpy((char*)&par_data_node_maal_tid, par_data_node->maal_tid);
            int par_data_node_har_neste = par_data_node->neste != 0?1:0;

            size_t sz = 0;
            sz = fwrite(&par_data_node_id,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par id=%d (%d)-----------",par_data_node_id, sz);
            sz = fwrite(&par_data_node_herre_person_id,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par herre_person_id=%d (%d)",par_data_node_herre_person_id, sz);
            sz = fwrite(&par_data_node_dame_person_id,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par dame_person_id=%d (%d)",par_data_node_dame_person_id, sz);
            sz = fwrite(&par_data_node_start_nr,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par start_nr=%d (%d)",par_data_node_start_nr, sz);
            sz = fwrite(&par_data_node_tids_poeng,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par tids_poeng=%d (%d)",par_data_node_tids_poeng, sz);
            sz = fwrite(&par_data_node_oppgave_poeng,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par oppgave_poeng=%d (%d)",par_data_node_oppgave_poeng, sz);
            sz = fwrite(&par_data_node_start_tid,sizeof(char),GDPL_MAX_TID_LENGDE,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par start_tid=%s (%d)",par_data_node_start_tid, sz);
            sz = fwrite(&par_data_node_maal_tid,sizeof(char),GDPL_MAX_TID_LENGDE,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par maal_tid=%s (%d)",par_data_node_maal_tid, sz);
            sz = fwrite(&par_data_node_har_neste,sizeof(int),1,file);
            GDPL_log(GDPL_DEBUG, signatur, "  Skriver par harNeste=%d (%d)",par_data_node_har_neste, sz);

            par_data_node = par_data_node->neste;
        } while (par_data_node != 0);

        konkurranse_data_node = konkurranse_data_node->neste;
    } while (konkurranse_data_node != 0);

    if (fclose(file) != 0) {
        GDPL_log(GDPL_ERROR, signatur, "fclose(file) != 0");
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_modell_privat_les_inn_fra_eksisterende_fil()
{
    const char* signatur = "GDPL_modell_privat_les_inn_fra_eksisterende_fil()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    FILE *file;
    file = fopen(gdpl_modell_datafilnavn,"r");

    if (file == NULL) {
        int feilkode = FEILKODE_KAN_IKKE_LESE_FRA_DATAFIL;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;
    }

    int mt = 0;
    size_t sz = fread(&mt,sizeof(int),1,file);
    if (sz != 1) {
        GDPL_log(GDPL_ERROR, signatur, "fread, sz != 1");
    }

    if (mt != 12345) {
        int feilkode = FEILKODE_DATAFIL_UKJENT_MN;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        if (fclose(file) != 0) {
            GDPL_log(GDPL_ERROR, signatur, "fclose(file) != 0");
        }
        return feilkode;
    }

    GDPL_konkurranse_data_node *konkurranse_data_node_root = 0;

    GDPL_konkurranse_data_node *konkurranse_data_node = 0;
    GDPL_konkurranse_data_node *konkurranse_data_node_tmp = 0;

    GDPL_person_data_node *person_data_node = 0;
    GDPL_person_data_node *person_data_node_tmp = 0;

    GDPL_par_data_node *par_data_node = 0;
    GDPL_par_data_node *par_data_node_tmp = 0;

    int konkurranse_data_node_har_neste = 0;
    int person_data_node_har_neste = 0;
    int par_data_node_har_neste = 0;

    do {

        GDPL_log(GDPL_INFO, signatur, "--------------------------------------");

        /* Opprett en konkurranse-data-node */

        konkurranse_data_node_tmp = 0;
        if (GDPL_konkurranse_opprett_node(&konkurranse_data_node_tmp) > 0) {
            return FEILKODE_FEIL;
        }

        /* Fyll konkurranse-data-noden med data fra fil. */

        int konkurranse_data_node_id = 0;
        int konkurranse_data_node_aar = 0;

        size_t sz = 0;
        sz = fread(&konkurranse_data_node_id,sizeof(int),1,file);
        GDPL_log(GDPL_INFO, signatur, "Leser konkurranse id=%d (%d)",konkurranse_data_node_id,sz);
        sz = fread(&konkurranse_data_node_aar,sizeof(int),1,file);
        GDPL_log(GDPL_INFO, signatur, "Leser konkurranse aar=%d (%d)",konkurranse_data_node_aar,sz);
        sz = fread(&konkurranse_data_node_har_neste,sizeof(int),1,file);
        GDPL_log(GDPL_INFO, signatur, "Leser konkurranse harNeste=%d (%d)",konkurranse_data_node_har_neste,sz);

        konkurranse_data_node_tmp->id = konkurranse_data_node_id;
        konkurranse_data_node_tmp->aar = konkurranse_data_node_aar;

        do {

            /* Opprett en person-data-node */

            person_data_node_tmp = 0;
            if (GDPL_person_opprett_node(&person_data_node_tmp) > 0) {
                return FEILKODE_FEIL;
            }

            /* Fyll person-data-noden med data fra fil. */

            int person_data_node_id = 0;
            char person_data_node_fnavn[GDPL_MAX_PERSONNAVN_LENGDE];
            char person_data_node_enavn[GDPL_MAX_PERSONNAVN_LENGDE];
            memset(person_data_node_fnavn, 0, GDPL_MAX_PERSONNAVN_LENGDE);
            memset(person_data_node_enavn, 0, GDPL_MAX_PERSONNAVN_LENGDE);

            size_t sz = 0;
            sz = fread(&person_data_node_id,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser person id=%d (%d)----",person_data_node_id,sz);
            sz = fread(&person_data_node_fnavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser person fnavn=%s (%d)",person_data_node_fnavn,sz);
            sz = fread(&person_data_node_enavn,sizeof(char),GDPL_MAX_PERSONNAVN_LENGDE,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser person enavn=%s (%d)",person_data_node_enavn,sz);
            sz = fread(&person_data_node_har_neste,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser person harNeste=%d (%d)",person_data_node_har_neste,sz);

            person_data_node_tmp->id = person_data_node_id;
            strncpy(person_data_node_tmp->fnavn,person_data_node_fnavn,GDPL_MAX_PERSONNAVN_LENGDE);
            strncpy(person_data_node_tmp->enavn,person_data_node_enavn,GDPL_MAX_PERSONNAVN_LENGDE);

            if (konkurranse_data_node_tmp->person_liste_root_ptr == 0) {
                konkurranse_data_node_tmp->person_liste_root_ptr = person_data_node_tmp;
                person_data_node = person_data_node_tmp;
            } else {
                person_data_node->neste = person_data_node_tmp;
                person_data_node = person_data_node_tmp;
            }

        } while (person_data_node_har_neste != 0);

        do {

            /* Opprett en par-data-node */

            par_data_node_tmp = 0;
            if (GDPL_par_opprett_node(&par_data_node_tmp) > 0) {
                return FEILKODE_FEIL;
            }

            /* Fyll par-data-noden med data fra fil. */

            int par_data_node_id = 0;
            int par_data_node_herre_person_id = 0;
            int par_data_node_dame_person_id = 0;
            int par_data_node_start_nr = 0;
            int par_data_node_tids_poeng = 0;
            int par_data_node_oppgave_poeng = 0;
            char par_data_node_start_tid[GDPL_MAX_TID_LENGDE];
            char par_data_node_maal_tid[GDPL_MAX_TID_LENGDE];
            memset(par_data_node_start_tid, 0, GDPL_MAX_TID_LENGDE);
            memset(par_data_node_maal_tid, 0, GDPL_MAX_TID_LENGDE);

            size_t sz = 0;
            sz = fread(&par_data_node_id,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par id=%d (%d)-------",par_data_node_id,sz);
            sz = fread(&par_data_node_herre_person_id,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par herre_person_id=%d (%d)",par_data_node_herre_person_id,sz);
            sz = fread(&par_data_node_dame_person_id,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par dame_person_id=%d (%d)",par_data_node_dame_person_id,sz);
            sz = fread(&par_data_node_start_nr,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par start_nr=%d (%d)",par_data_node_start_nr,sz);
            sz = fread(&par_data_node_tids_poeng,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par tids_poeng=%d (%d)",par_data_node_tids_poeng,sz);
            sz = fread(&par_data_node_oppgave_poeng,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par oppgave_poeng=%d (%d)",par_data_node_oppgave_poeng,sz);
            sz = fread(&par_data_node_start_tid,sizeof(char),GDPL_MAX_TID_LENGDE,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par start_tid=%s (%d)",par_data_node_start_tid,sz);
            sz = fread(&par_data_node_maal_tid,sizeof(char),GDPL_MAX_TID_LENGDE,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par maal_tid=%s (%d)",par_data_node_maal_tid,sz);
            sz = fread(&par_data_node_har_neste,sizeof(int),1,file);
            GDPL_log(GDPL_INFO, signatur, "  Leser par harNeste=%d (%d)",par_data_node_har_neste,sz);

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

    if (gdpl_modell_konkurranseliste_root_ptr != 0) {
        if (GDPL_modell_privat_slett_alt() > 0) {
            return FEILKODE_FEIL;
        }
    }

    gdpl_modell_konkurranseliste_root_ptr = konkurranse_data_node_root;
    gdpl_modell_konkurranseliste_valgt_ptr = 0;

    if (fclose(file) != 0) {
        GDPL_log(GDPL_ERROR, signatur, "fclose(file) != 0");
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_modell_privat_slett_alt()
{
    const char* signatur = "GDPL_modell_privat_slett_alt()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (gdpl_modell_konkurranseliste_root_ptr == 0) {
        GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
        return 0;
    }

    GDPL_konkurranse_data_node *konkurranse_data_node = gdpl_modell_konkurranseliste_root_ptr;
    GDPL_person_data_node *person_data_node;
    GDPL_par_data_node *par_data_node;

    GDPL_konkurranse_data_node *ptrKonkurranse;
    GDPL_person_data_node *ptrPerson;
    GDPL_par_data_node *ptrPar;

    do {
        person_data_node = konkurranse_data_node->person_liste_root_ptr;
        do {
            ptrPerson = person_data_node;
            person_data_node = person_data_node->neste;
            free(ptrPerson);
        } while (person_data_node != 0);
        par_data_node = konkurranse_data_node->par_liste_root_ptr;
        do {
            ptrPar = par_data_node;
            par_data_node = par_data_node->neste;
            free(ptrPar);
        } while (par_data_node != 0);
        ptrKonkurranse = konkurranse_data_node;
        konkurranse_data_node = konkurranse_data_node->neste;
        free(ptrKonkurranse);
    } while (konkurranse_data_node != 0);

    if (gdpl_modell_konkurranseliste_root_ptr == 0) {
        GDPL_log(GDPL_ERROR, signatur, "%d == DEBUG Systemfeil. Klarte ikke å slette.",__LINE__);
        return 0;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_modell_privat_sjekk_filnavn(const char* filnavn)
{    
    const char* signatur = "GDPL_modell_privat_sjekk_filnavn(const char*)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (filnavn == 0) {
        int feilkode = FEILKODE_DATAFILNAVN_IKKE_DEFINERT;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;
    }

    if (strlen(filnavn) > GDPL_MAX_FILNAVN_LENGDE) {
        int feilkode = FEILKODE_DATAFILNAVN_FOR_LANGT;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;
    }

    if (strlen(filnavn) < GDPL_MIN_FILNAVN_LENGDE) {
        int feilkode = FEILKODE_DATAFILNAVN_FOR_KORT;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon. (filnavn er ok)");

    return 0;

}


int GDPL_modell_dump()
{
    const char* signatur = "GDPL_modell_dump()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    GDPL_konkurranse_data_node *k = 0;
    GDPL_person_data_node *person = 0;
    GDPL_par_data_node *par = 0;

    k = gdpl_modell_konkurranseliste_root_ptr;

    do {
        GDPL_log(GDPL_INFO, signatur, "----------------------------------------");
        GDPL_log(GDPL_INFO, signatur, "konkurranse id :%d",k->id);
        GDPL_log(GDPL_INFO, signatur, "konkurranse aar:%d",k->aar);
        person = k->person_liste_root_ptr;
        do {
            GDPL_log(GDPL_INFO, signatur, "   -------------------------------------");
            GDPL_log(GDPL_INFO, signatur, "   person id    :%d",person->id);
            GDPL_log(GDPL_INFO, signatur, "   person fnavn :%s",person->fnavn);
            GDPL_log(GDPL_INFO, signatur, "   person enavn :%s",person->enavn);
            person = person->neste;
        } while(person!=0);
        par = k->par_liste_root_ptr;
        do {
            GDPL_log(GDPL_INFO, signatur, "   -------------------------------------");
            GDPL_log(GDPL_INFO, signatur, "   par id              :%d",par->id);
            GDPL_log(GDPL_INFO, signatur, "   par herre_person_id :%d",par->herre_person_id);
            GDPL_log(GDPL_INFO, signatur, "   par dame_person_id  :%d",par->dame_person_id);
            GDPL_log(GDPL_INFO, signatur, "   par start_nr        :%d",par->start_nr);
            GDPL_log(GDPL_INFO, signatur, "   par start_tid       :%s",par->start_tid);
            GDPL_log(GDPL_INFO, signatur, "   par maal_tid        :%s",par->maal_tid);
            GDPL_log(GDPL_INFO, signatur, "   par tids_poeng      :%d",par->tids_poeng);
            GDPL_log(GDPL_INFO, signatur, "   par oppgave_poeng   :%d",par->oppgave_poeng);
            par = par->neste;
        } while(par!=0);
        k = k->neste;
    } while (k!=0);

    fflush(NULL);

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}
