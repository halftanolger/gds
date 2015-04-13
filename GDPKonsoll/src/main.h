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

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"

/*! \fn int gubb(gubb_input_args *input)
    \brief Oppretter et nytt regneark med de beregnede verdiene, basert på input-regnearket.
    \param input Struktur hvor de ulike inputparametrene er lagret.

    Dette er hovedfunksjonen til konsoll-programmet. Den lager i utgangspunktet
    kun ei ny csv-fil, basert på input-csv-fila, men kan også lage ulike tekst-
    filer som innholder rapporter av den typen som DataEase-programmet til
    Jan-Eirik lager. Alt dette styres vha input-parametrene.

*/
int gubb(gubb_input_args *input);

#ifdef __cplusplus
}
#endif

#endif // MAIN_H


