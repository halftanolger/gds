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

#ifndef KONTROLLER_H
#define KONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Variabler */

extern const char *gdpl_kontroller_feilkoder[];

/* Funksjoner */

const char* GDPL_kontroller_gdplib_navn();

const char* GDPL_kontroller_gdplib_versjon();

int GDPL_kontroller_angi_max_antall_par(int antall);

int GDPL_kontroller_hent_max_antall_par(int *antall);

int GDPL_kontroller_hent_par_nummer(int *par_nummer, int type);

#ifdef __cplusplus
}
#endif

#endif /* KONTROLLER_H */
