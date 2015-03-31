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

#ifndef PERSON_H
#define PERSON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "modell.h"

int GDPL_person_opprett_node(GDPL_person_data_node **new_node);

int GDPL_person_legg_til(GDPL_person_data_node *data);

int GDPL_person_fjern_fra(GDPL_person_data_node data);

int GDPL_person_hent(int id, GDPL_person_data_node **data);

int GDPL_person_antall_i_liste(int *antall);

int GDPL_person_finn_neste_ledige_id(int *id);

#ifdef __cplusplus
}
#endif

#endif /* PERSON_H */

