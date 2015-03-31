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
******************************************************************************
*/

#ifndef PAR_H
#define PAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "modell.h"

enum GDPL_par_sorter_enum { TIDS_POENG_SYNKENDE,    /* 0 */
                            TIDS_POENG_STIGENDE,    /* 1 */
                            OPPGAVE_POENG_SYNKENDE, /* 2 */
                            OPPGAVE_POENG_STIGENDE, /* 3 */
                            TOTALT_POENG_SYNKENDE,  /* 4 */
                            TOTALT_POENG_STIGENDE,  /* 5 */
                            START_NR_SYNKENDE,      /* 6 */
                            START_NR_STIGENDE       /* 7 */
                          };

typedef enum GDPL_par_sorter_enum GDPL_par_sorter_type;

int GDPL_par_opprett_node(GDPL_par_data_node **new_node);

int GDPL_par_legg_til(GDPL_par_data_node *data);

int GDPL_par_fjern_fra(GDPL_par_data_node *data);

int GDPL_par_hent(int id, GDPL_par_data_node **data);

int GDPL_par_hent_i_rekke(int nr, GDPL_par_data_node **data);

int GDPL_par_antall_i_liste(int *antall);

int GDPL_par_finn_neste_ledige_id(int *id);

int GDPL_par_valider_tid(struct GDPL_tid tid);

int GDPL_par_valider_starttid_mot_maaltid(struct GDPL_tid starttid, struct GDPL_tid maaltid);

int GDPL_par_beregn();

int GDPL_par_beregn_anvendt_tid(GDPL_par_data_node *data);

int GDPL_par_beregn_middel_tid(struct GDPL_tid *middel_tid);

int GDPL_par_beregn_tids_poeng(GDPL_par_data_node *data, struct GDPL_tid middel_tid);

int GDPL_par_beregn_avvik(struct GDPL_tid *avvik, struct GDPL_tid a, struct GDPL_tid b);

int GDPL_par_sorter(GDPL_par_sorter_type type);

#ifdef __cplusplus
}
#endif

#endif /* PAR_H */


