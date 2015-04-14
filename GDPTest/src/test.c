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

#include <stdio.h>

int gdp_test_run() {

    /* Denne funksjonen kjører en rekke enhetstester. Hver test skal 'stå på egne ben',
     * det vil si at den skal ikke ha noen knytning mot andre tester. Den skal kun
     * returnere 0 eller 1, for henholdsvis 'ok' eller 'ikke ok'.
     */

    fprintf ( stderr, "Hello world from gdp_test_run()\n" );

    return 0;

}
