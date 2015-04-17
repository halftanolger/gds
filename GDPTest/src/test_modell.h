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

/*! \file test_modell.h
    \brief GDPTest Enhetstester

    Ulike enhetstester mot logikk som er definert i fila modell.c i GDPLib

*/

#ifndef TEST_MODELL_H
#define TEST_MODELL_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \fn int gdp_test_modell_opprett_datafil_a()
    \brief Enhetstest relatert til datafil.

    Om man oppgir en null-peker som input til GDPL_modell_angi_filnavn(const char*)
    skal GDPLib opprette ei default datafil i cwd (current working directory), kalt
    'gdp.dat'.

    Denne testfunksjonen sjekker at det skjer.

*/
int gdp_test_modell_opprett_datafil_a();

/*! \fn int gdp_test_modell_opprett_datafil_b()
    \brief Enhetstest relatert til datafil.

    Om man oppgir et filnavn som input til GDPL_modell_angi_filnavn(const char*) skal GDPLib
    opprette ei datafil i cwd (current working directory), med det oppgitte navnet.

    Denne testfunksjonen sjekker at det skjer.

*/
int gdp_test_modell_opprett_datafil_b();

/*! \fn int gdp_test_modell_opprett_datafil_c()
    \brief Enhetstest relatert til datafil.

    Om man oppgir et filnavn som input til GDPL_modell_angi_filnavn(const char*),
    som er lengre enn GDPL_MAX_FILNAVN_LENGDE, skal funksjonen
    returnere feilkoden FEILKODE_DATAFILNAVN_FOR_LANGT

    Denne testfunksjonen sjekker at det skjer.

*/
int gdp_test_modell_opprett_datafil_c();

/*! \fn int gdp_test_modell_opprett_datafil_d()
    \brief Enhetstest relatert til datafil.

    Om man oppgir et filnavn som input til GDPL_modell_angi_filnavn(const char*),
    som er kortere enn GDPL_MIN_FILNAVN_LENGDE, skal funksjonen
    returnere feilkoden FEILKODE_DATAFILNAVN_FOR_KORT

    Denne testfunksjonen sjekker at det skjer.

*/
int gdp_test_modell_opprett_datafil_d();



//TODO: definer flere tester ...


#ifdef __cplusplus
}
#endif

#endif // TEST_MODELL_H


