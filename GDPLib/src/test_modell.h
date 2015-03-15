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

#ifndef TEST_MODELL_H
#define TEST_MODELL_H

#ifdef __cplusplus
extern "C" {
#endif


/* Variabel-deklareringer */


/* Funksjons-deklareringer */

int GDPL_test_modell_alle_funksjoner();

int GDPL_test_modell_angi_filnavn(const char *filnavn);

int GDPL_test_modell_les_data();

int GDPL_test_modell_skriv_data();

/* 'Private' funksjons-deklareringer */


#ifdef __cplusplus
}
#endif

#endif // TEST_MODELL_H

