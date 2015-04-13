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

#include "rapport.h"
#include "util.h"
#include "par.h"

int gubb_rapport_print_start ( gubb_rapport_print_record_data *d, FILE *stream ) {

    GDPL_par_data_node *par;
    GDPL_person_data_node *hperson;
    GDPL_person_data_node *dperson;

    par = d->par;
    hperson = d->herre;
    dperson = d->dame;

    fprintf ( stream, "STARTNR          FORNAVN            ETTERNAVN  STARTTID\n" );
    fprintf ( stream, " %03d     %15s %20s  %02d:%02d:%02d\n", par->start_nr, dperson->fnavn, dperson->enavn,par->start_tid.timer,par->start_tid.minutt,par->start_tid.sekund );
    fprintf ( stream, "         %15s %20s\n", hperson->fnavn, hperson->enavn  );
    fprintf ( stream, "-------------------------------------------------------\n");

    return 0;
}

int gubb_rapport_print_res1 ( gubb_rapport_print_record_data *p, FILE *stream ) {

    int *plassering;
    GDPL_par_data_node *par;
    GDPL_person_data_node *hperson;
    GDPL_person_data_node *dperson;
    struct GDPL_tid *middel_tid;

    plassering = p->plassering;
    par = p->par;
    hperson = p->herre;
    dperson = p->dame;
    middel_tid = p->middel_tid;

    fprintf ( stream, "\n\n===================================================================\n\n");
    fprintf ( stream, "               STARTNR :%.3d\n\n",par->start_nr);
    fprintf ( stream, "  PLASS :%.2d    %s %s\n",*plassering,dperson->fnavn, dperson->enavn);
    fprintf ( stream, "               %s %s\n\n",hperson->fnavn, hperson->enavn);
    fprintf ( stream, "                 START TID : %02d:%02d:%02d         STARTPOENG :    60,00\n",par->start_tid.timer,par->start_tid.minutt,par->start_tid.sekund);
    fprintf ( stream, "                 SLUTT TID : %02d:%02d:%02d        - TIDSPOENG :    %02.02f\n",par->maal_tid.timer,par->maal_tid.minutt,par->maal_tid.sekund,par->tids_poeng);
    fprintf ( stream, "               ANVENDT TID : %02d:%02d:%02d        = LØPSPOENG :    %02.02f\n",par->anvendt_tid.timer,par->anvendt_tid.minutt,par->anvendt_tid.sekund, gubb_util_rund_av(60.0 - par->tids_poeng));
    fprintf ( stream, "              GJ.SNITT TID : %02d:%02d:%02d       + OPPG.POENG :    %02.02f\n",middel_tid->timer,middel_tid->minutt,middel_tid->sekund,par->oppgave_poeng);
    fprintf ( stream, "                             --------       = SLUTTPOENG :    %02.02f\n",gubb_util_rund_av((60.0 - par->tids_poeng) + par->oppgave_poeng));
    fprintf ( stream, "                                                           ========\n");

    return 0;
}

int gubb_rapport_print_res2 ( gubb_rapport_print_record_data *p, FILE *stream ) {

    int *plassering;
    GDPL_par_data_node *par;
    GDPL_person_data_node *hperson;
    GDPL_person_data_node *dperson;
    struct GDPL_tid *middel_tid;

    plassering = p->plassering;
    par = p->par;
    hperson = p->herre;
    dperson = p->dame;
    middel_tid = p->middel_tid;

    fprintf ( stream, "-------------------------------------------------------------------\n");
    fprintf ( stream, "  PLASSERING   : %.2d    %s %s\n",*plassering,dperson->fnavn, dperson->enavn);
    fprintf ( stream, "                       %s %s\n",hperson->fnavn, hperson->enavn);
    fprintf ( stream, "  POENG        : %02.02f\n",gubb_util_rund_av((60.0 - par->tids_poeng) + par->oppgave_poeng));
    fprintf ( stream, "  STARTNR      : %.3d\n",par->start_nr);

    return 0;
}

int (*gubb_rapport_function_ptr_array[]) ( gubb_rapport_print_record_data *p, FILE *stream ) = {
        gubb_rapport_print_start, /* GRT_START */
        gubb_rapport_print_res1,  /* GRT_RES1  */
        gubb_rapport_print_res2,  /* GRT_RES2  */
};

int gubb_rapport_print_record ( gubb_rapport_print_record_data *p, gubb_rapport_type type, FILE *stream ) {
    return gubb_rapport_function_ptr_array[type] ( p, stream );
}
