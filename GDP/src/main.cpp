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

#include <QtWidgets>
#include <QDebug>
#include "gdpmainwindows.h"

#include <stdio.h>
#include "gdpl.h"



extern const char* gdpl_kontroller_gdplib_navn;
extern const char* gdpl_kontroller_gdplib_versjon;

int test_gdplib();

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    GDPMainWindows* appWin = new GDPMainWindows();
    appWin->show();
    appWin->setFocus();

    test_gdplib();

    int r = app.exec();
    return r;
}


int test_gdplib() {

    //
    // GDPLib testing
    //
    // Siden dette er et C-bibliotek, er det mye nitty-gritty-stuff som må gjøres for
    // å få noe til å skje. Hensiten med denne oversikten er bare å vise hvordan man
    // kan bruke biblioteket, og hvordan de ulike funksjonene henger sammen.
    //
    //

    int error_nr = 0;


    //
    // Punkt 1.
    //
    // Initier logging
    //

    const char* signatur = "test_gdplib()";
    error_nr = GDPL_init(GDPL_DEBUG, stdout);

    if (error_nr > 0) {
        qWarning() << "Systemfeil. GDPL_init(..) returnerte > 0";
        return error_nr;
    }

    GDPL_log(GDPL_DEBUG, signatur, "Hello world - %s %s", gdpl_kontroller_gdplib_navn, gdpl_kontroller_gdplib_versjon);


    //
    // Punkt 2.
    //
    // Angi hvilken datafil vi ønsker å bruke.
    //

    error_nr = GDPL_kontroller_angi_filnavn("c:/tmp/gdpdata.dat");
    if (error_nr > 0) {
        qWarning() << "Systemfeil. GDPL_kontroller_angi_filnavn(..) returnerte > 0";
        return error_nr;
    }


    //
    // Punkt 3.
    //
    // Initier datastrukturen til biblioteket ved
    // å lese fra den valgte datafila.
    //
    //

    error_nr = GDPL_kontroller_les_fra_fil();
    if (error_nr > 0) {
        qWarning() << "Systemfeil. GDPL_kontroller_angi_filnavn(..) returnerte > 0";
        return error_nr;
    }



    //GDPL_kontroller_angi_max_antall_par(50);



    GDPL_konkurranse_data_node *root_konkurranse = 0;
    GDPL_konkurranse_data_node *valgt_konkurranse = 0;
    GDPL_konkurranse_data_node node_konkurranse;

    node_konkurranse.id = 1;
    node_konkurranse.aar = 2015;


    return 0;


}
