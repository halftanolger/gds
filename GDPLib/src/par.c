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
#include "par.h"
#include "diverse.h"
#include "kontroller.h"


int GDPL_par_opprett_node(GDPL_par_data_node **new_node)
{
    const char* signatur = "GDPL_par_opprett_node(GDPL_par_data_node**)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if ((*new_node) != 0) {
        int feilkode = FEILKODE_KAN_IKKE_ALLOKERE_MINNE_S;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;
    }

    *new_node = (GDPL_par_data_node*)
            malloc (sizeof (GDPL_par_data_node));

    if (*new_node == 0) {
        int feilkode = FEILKODE_KAN_IKKE_ALLOKERE_MINNE;
        GDPL_log(GDPL_ERROR, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;
    }

    (*new_node)->id = 0;
    (*new_node)->herre_person_id = 0;
    (*new_node)->dame_person_id = 0;
    (*new_node)->start_nr = 0;
    (*new_node)->tids_poeng = 0;
    (*new_node)->oppgave_poeng = 0;
    (*new_node)->neste = 0;
    (*new_node)->start_tid.timer = 0;
    (*new_node)->start_tid.minutt = 0;
    (*new_node)->start_tid.sekund = 0;
    (*new_node)->maal_tid.timer = 0;
    (*new_node)->maal_tid.minutt = 0;
    (*new_node)->maal_tid.sekund = 0;
    (*new_node)->anvendt_tid.timer = 0;
    (*new_node)->anvendt_tid.minutt = 0;
    (*new_node)->anvendt_tid.sekund = 0;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_par_legg_til(GDPL_par_data_node *data)
{
    const char* signatur = "GDPL_par_legg_til(GDPL_par_data_node*)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
    
    GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
    if (kroot == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr;
    if (root == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr er null");
        return FEILKODE_FEIL;
    }

    /* Litt inputparameter-sjekking. */

    if (data == 0) {
        GDPL_log(GDPL_ERROR, signatur, "data == 0");
        return FEILKODE_FEIL;
    }

    if (data->id == 0) {
        GDPL_log(GDPL_ERROR, signatur, "data->id == 0, id må være større enn null");
        return FEILKODE_FEIL;
    }

    /* Sjekk om ny id finnes fra før i lista. */
    int id_eksisterer = 0;
    GDPL_par_data_node *runner = root;

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
        GDPL_log(GDPL_DEBUG, signatur, gdpl_kontroller_feilkoder[feilkode]);
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


int GDPL_par_fjern_fra(GDPL_par_data_node *data)
{
    const char* signatur = "GDPL_par_fjern_fra(GDPL_par_data_node*)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");
    
    GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
    if (kroot == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr;
    if (root == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr er null");
        return FEILKODE_FEIL;
    }

    /* Litt inputparameter-sjekking. */

    if (data->id == 0) {
        GDPL_log(GDPL_DEBUG, signatur, "data.id == 0");
        return FEILKODE_FEIL;
    }

    /* Sjekk om id finnes i lista. */

    int id_eksisterer = 0;
    GDPL_par_data_node *runner = root;

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
        GDPL_log(GDPL_DEBUG, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;
    }

    GDPL_par_data_node *runner2 = root;

    while (runner2->neste != runner) {
        runner2 = runner2->neste;
    }
    
    /* Fjern noden fra lista. */

    runner2->neste = runner->neste;

    free(runner);

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_par_hent(int id, GDPL_par_data_node **data)
{
    const char* signatur = "GDPL_par_hent(int,GDPL_par_data_node**)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
    if (kroot == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr;
    if (root == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr er null");
        return FEILKODE_FEIL;
    }

    /* Litt inputparameter-sjekking. */

    if (*data != 0) {
        GDPL_log(GDPL_DEBUG, signatur, "data != 0");
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
        GDPL_log(GDPL_DEBUG, signatur, gdpl_kontroller_feilkoder[feilkode]);
        return feilkode;
    }
    
    *data = runner;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}


int GDPL_par_antall_i_liste(int *antall)
{
    const char* signatur = "GDPL_par_antall_i_liste(int*)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
    if (kroot == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr;
    if (root == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr er null");
        return FEILKODE_FEIL;
    }

    /* Litt inputparameter-sjekking. */

    if (antall == 0) {
        GDPL_log(GDPL_DEBUG, signatur, "antall == 0, I'm out of here ...");
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

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");
    return 0;
}


int GDPL_par_finn_neste_ledige_id(int *id)
{
    const char* signatur = "GDPL_par_finn_neste_ledige_id(..)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
    if (kroot == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr;
    if (root == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr er null");
        return FEILKODE_FEIL;
    }

    if (id == 0) {
        GDPL_log(GDPL_ERROR, signatur, "id må peke på et heltall, id peker på null");
        return FEILKODE_FEIL;
    }

    /* Finn neste unike id mht par-lista. */

    int tmp_id = 1;
    do {
        int i = root->id;
        if (tmp_id <= i) {
            tmp_id = i + 1;
        }
        root = root->neste;
    } while (root != 0);
    *id = tmp_id;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_par_valider_tid(struct GDPL_tid tid)
{
    const char* signatur = "GDPL_par_private_valider_tid()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (tid.timer > 24 || tid.timer < 0) {
        GDPL_log(GDPL_ERROR, signatur, "tid.timer > 24 || tid.timer < 0");
        return FEILKODE_FEIL;
    }

    if (tid.minutt > 60 || tid.minutt < 0) {
        GDPL_log(GDPL_ERROR, signatur, "tid.minutt > 60 || tid.minutt < 0");
        return FEILKODE_FEIL;
    }

    if (tid.sekund > 60 || tid.sekund < 0) {
        GDPL_log(GDPL_ERROR, signatur, "tid.sekund > 60 || tid.sekund < 0");
        return FEILKODE_FEIL;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_par_valider_starttid_mot_maaltid(struct GDPL_tid starttid, struct GDPL_tid maaltid)
{
    const char* signatur = "GDPL_par_valider_starttid_mot_maaltid()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    /* Måltid må være større eller lik starttid. */

    int s = starttid.sekund + starttid.minutt*60 + starttid.timer*60*60;
    int m = maaltid.sekund + maaltid.minutt*60 + maaltid.timer*60*60;

    if ( m < s ) {
        GDPL_log(GDPL_ERROR, signatur, "start-tid > mål-tid");
        return FEILKODE_FEIL;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_par_beregn_middel_tid(struct GDPL_tid *middel_tid)
{
    const char* signatur = "GDPL_par_beregn_middel_tid()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
    if (kroot == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node *p = gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr;
    if (p == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr er null");
        return FEILKODE_FEIL;
    }

    if (middel_tid == 0) {
        GDPL_log(GDPL_ERROR, signatur, "middel_tid må peke på en GDPL_tid-struct, middel_tid peker på null");
        return FEILKODE_FEIL;
    }

    int antall_par = 0;
    int antall_sekund = 0;

    p = p->neste;

    while (p != 0) {

        antall_par++;

        if (GDPL_par_beregn_anvendt_tid(p) > 0)
            return FEILKODE_FEIL;

        int s = p->anvendt_tid.sekund + p->anvendt_tid.minutt*60 + p->anvendt_tid.timer*60*60;
        antall_sekund += s;

        p = p->neste;

    }

    int middel_tid_sekund = antall_sekund / antall_par;
    middel_tid->timer = middel_tid_sekund / (60*60);
    int _middel_tid = middel_tid_sekund % (60*60);
    middel_tid->minutt = _middel_tid / 60;
    middel_tid->sekund = _middel_tid % 60;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;
}


int GDPL_par_beregn_tids_poeng(GDPL_par_data_node *data, struct GDPL_tid middel_tid)
{
    const char* signatur = "GDPL_par_beregn_anvendt_tid(GDPL_par_data_node*)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (data == 0) {
        GDPL_log(GDPL_ERROR, signatur, "data == 0");
        return FEILKODE_FEIL;
    }

    /* Beregnet middel-tid må være et ok tidsobjekt. */

    if (GDPL_par_valider_tid(middel_tid)>0)
        return FEILKODE_FEIL;

    /* Beregnet anvendt-tid må være et ok tidsobjekt. */

    if (GDPL_par_valider_tid(data->anvendt_tid)>0)
        return FEILKODE_FEIL;

    int s1 = data->anvendt_tid.sekund + data->anvendt_tid.minutt*60 + data->anvendt_tid.timer*60*60;
    int s2 = middel_tid.sekund + middel_tid.minutt*60 + middel_tid.timer*60*60;

    int d = abs(s1-s2);

    data->tids_poeng = 60 - (d/60);

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;

}


int GDPL_par_beregn_anvendt_tid(GDPL_par_data_node *data)
{
    const char* signatur = "GDPL_par_beregn_anvendt_tid(GDPL_par_data_node*)";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    if (data == 0) {
        GDPL_log(GDPL_ERROR, signatur, "data == 0");
        return FEILKODE_FEIL;
    }

    /* Valider tidsobjektene hver for seg. */

    if (GDPL_par_valider_tid(data->start_tid) > 0)
        return FEILKODE_FEIL;

    if (GDPL_par_valider_tid(data->maal_tid) > 0)
        return FEILKODE_FEIL;

    /* Valider at starttid er mindre eller lik måltid. */

    if (GDPL_par_valider_starttid_mot_maaltid(data->start_tid,data->maal_tid) > 0)
        return FEILKODE_FEIL;

    /* Beregn anvendt tid. */

    struct GDPL_tid t1, t2, diff;

    t1.timer = data->maal_tid.timer;
    t1.minutt = data->maal_tid.minutt;
    t1.sekund = data->maal_tid.sekund;

    t2.timer = data->start_tid.timer;
    t2.minutt = data->start_tid.minutt;
    t2.sekund = data->start_tid.sekund;

    if (t2.sekund > t1.sekund) {
        t1.minutt --;
        t1.sekund += 60;
    }

    diff.sekund = t1.sekund - t2.sekund;

    if (t2.minutt > t1.minutt) {
        t1.timer --;
        t1.minutt += 60;
    }

    diff.minutt = t1.minutt - t2.minutt;
    diff.timer = t1.timer - t2.timer;

    data->anvendt_tid.timer = diff.timer;
    data->anvendt_tid.minutt = diff.minutt;
    data->anvendt_tid.sekund = diff.sekund;

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;

}


int GDPL_par_sammenlign_tids_poeng_stigende(GDPL_par_data_node *a, GDPL_par_data_node *b)
{
    int ta = a->tids_poeng;
    int tb = b->tids_poeng;

    if (ta == tb) {
        return 0;
    } else if (ta > tb) {
        return 1;
    } else {
        return -1;
    }
}


int GDPL_par_sammenlign_tids_poeng_synkende(GDPL_par_data_node *a, GDPL_par_data_node *b)
{
    int ta = a->tids_poeng;
    int tb = b->tids_poeng;

    if (ta == tb) {
        return 0;
    } else if (ta > tb) {
        return -1;
    } else {
        return 1;
    }
}


int GDPL_par_sammenlign_oppgave_poeng_stigende(GDPL_par_data_node *a, GDPL_par_data_node *b)
{
    int ta = a->oppgave_poeng;
    int tb = b->oppgave_poeng;

    if (ta == tb) {
        return 0;
    } else if (ta > tb) {
        return 1;
    } else {
        return -1;
    }
}


int GDPL_par_sammenlign_oppgave_poeng_synkende(GDPL_par_data_node *a, GDPL_par_data_node *b)
{
    int ta = a->oppgave_poeng;
    int tb = b->oppgave_poeng;

    if (ta == tb) {
        return 0;
    } else if (ta > tb) {
        return -1;
    } else {
        return 1;
    }
}


int GDPL_par_sammenlign_totalt_poeng_stigende(GDPL_par_data_node *a, GDPL_par_data_node *b)
{
    int ta = a->oppgave_poeng + a->tids_poeng;
    int tb = b->oppgave_poeng + b->tids_poeng;

    if (ta == tb) {
        return 0;
    } else if (ta > tb) {
        return 1;
    } else {
        return -1;
    }
}


int GDPL_par_sammenlign_totalt_poeng_synkende(GDPL_par_data_node *a, GDPL_par_data_node *b)
{
    int ta = a->oppgave_poeng + a->tids_poeng;
    int tb = b->oppgave_poeng + b->tids_poeng;

    if (ta == tb) {
        return 0;
    } else if (ta > tb) {
        return -1;
    } else {
        return 1;
    }
}


int GDPL_par_sammenlign_start_nr_stigende(GDPL_par_data_node *a, GDPL_par_data_node *b)
{
    int ta = a->start_nr;
    int tb = b->start_nr;

    if (ta == tb) {
        return 0;
    } else if (ta > tb) {
        return 1;
    } else {
        return -1;
    }
}


int GDPL_par_sammenlign_start_nr_synkende(GDPL_par_data_node *a, GDPL_par_data_node *b)
{
    int ta = a->start_nr;
    int tb = b->start_nr;

    if (ta == tb) {
        return 0;
    } else if (ta > tb) {
        return -1;
    } else {
        return 1;
    }
}

/* Funksjonspekere som benyttes i GDPL_par_sorter() ut fra GDPL_par_sorter_type */

int (*function_ptr_array[])(GDPL_par_data_node *a, GDPL_par_data_node *b) = {

        GDPL_par_sammenlign_tids_poeng_synkende,     /* TIDS_POENG_SYNKENDE    */
        GDPL_par_sammenlign_tids_poeng_stigende,     /* TIDS_POENG_STIGENDE    */
        GDPL_par_sammenlign_oppgave_poeng_synkende,  /* OPPGAVE_POENG_SYNKENDE */
        GDPL_par_sammenlign_oppgave_poeng_stigende,  /* OPPGAVE_POENG_STIGENDE */
        GDPL_par_sammenlign_totalt_poeng_synkende,   /* TOTALT_POENG_SYNKENDE  */
        GDPL_par_sammenlign_totalt_poeng_stigende,   /* TOTALT_POENG_STIGENDE  */
        GDPL_par_sammenlign_start_nr_synkende,       /* START_NR_SYNKENDE      */
        GDPL_par_sammenlign_start_nr_stigende        /* START_NR_STIGENDE      */

        };


int GDPL_par_sorter(GDPL_par_sorter_type type)
{
    const char* signatur = "GDPL_par_sorter()";

    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    GDPL_konkurranse_data_node *kroot = gdpl_modell_konkurranseliste_valgt_ptr;
    if (kroot == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr er null");
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node *root = gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr;
    if (root == 0) {
        GDPL_log(GDPL_ERROR, signatur, "gdpl_modell_konkurranseliste_valgt_ptr->par_liste_root_ptr er null");
        return FEILKODE_FEIL;
    }

    GDPL_par_data_node *runner = root;
    while (runner != 0) {
        GDPL_par_data_node *p = root;
        while (p != 0) {
            if (p->neste != 0) {
                if (p->neste->neste != 0) {
                    int c = function_ptr_array[type](p->neste,p->neste->neste);
                    if (c > 0) {
                        /* Swap */
                        GDPL_par_data_node *tail = p->neste->neste->neste;
                        GDPL_par_data_node *tmpA = p->neste;
                        GDPL_par_data_node *tmpB = p->neste->neste;
                        p->neste = tmpB;
                        tmpB->neste = tmpA;
                        tmpA->neste = tail;
                    }
                }
            }
            p = p->neste;
        }
        runner = runner->neste;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Slutt funksjon.");

    return 0;

}


