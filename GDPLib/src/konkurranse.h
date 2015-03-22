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

#ifndef KONKURRANSE_H
#define KONKURRANSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "modell.h"

int GDPL_konkurranse_opprett_node(GDPL_konkurranse_data_node **new_node);

int GDPL_konkurranse_legg_til(GDPL_konkurranse_data_node *data);

int GDPL_konkurranse_fjern_fra(GDPL_konkurranse_data_node* data);

int GDPL_konkurranse_hent(int id, GDPL_konkurranse_data_node **data);

int GDPL_konkurranse_antall_i_liste(int *antall);

int GDPL_konkurranse_sett_valgt_konkurranse(int id);

#ifdef __cplusplus
}
#endif
#endif // KONKURRANSE_H
