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
#include "par.h"

int gubb_rapport_print_start ( GDPL_par_data_node *p, FILE *stream ) {

    fprintf ( stream, "STARTNR: %d\n", p->start_nr );

    return 0;
}

int gubb_rapport_print_res1 ( GDPL_par_data_node *p, FILE *stream ) {
    return 0;
}

int gubb_rapport_print_res2 ( GDPL_par_data_node *p, FILE *stream ) {
    return 0;
}

int (*gubb_rapport_function_ptr_array[]) ( GDPL_par_data_node *p, FILE *stream ) = {
        gubb_rapport_print_start, /* GRT_START */
        gubb_rapport_print_res1,  /* GRT_RES1  */
        gubb_rapport_print_res2,  /* GRT_RES2  */
};

int gubb_rapport_print_record ( GDPL_par_data_node *p, gubb_rapport_type type, FILE *stream ) {
    return gubb_rapport_function_ptr_array[type] ( p, stream );
}
