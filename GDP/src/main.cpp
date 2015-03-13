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
#include "gdpmainwindows.h"

#include <stdio.h>
#include "gdpl.h"

extern const char* gdpl_kontroller_gdplib_navn;
extern const char* gdpl_kontroller_gdplib_versjon;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    GDPMainWindows* appWin = new GDPMainWindows();
    appWin->show();
    appWin->setFocus();


    const char* signatur = "main(int,char)";
    GDPL_init(GDPL_DEBUG, stdout);
    GDPL_log(GDPL_DEBUG, signatur, "Hello world - %s %s", gdpl_kontroller_gdplib_navn, gdpl_kontroller_gdplib_versjon);
    GDPL_test();


    int r = app.exec();
    return r;
}
