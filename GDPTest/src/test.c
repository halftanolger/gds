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
#include "log.h"

#include "test_modell.h"

int gdp_test_run() {

    /* Denne funksjonen kjører en rekke enhetstester. Hver test skal 'stå på egne ben',
     * det vil si at den skal ikke ha noen knytning mot andre tester. Den skal kun
     * returnere 0 eller 1, for henholdsvis 'ok' eller 'ikke ok'.
     */

    const char* signatur = "gdp_test_run()";
    GDPL_log(GDPL_DEBUG, signatur, "Start funksjon.");

    int i;
    int antall_feil;
    int antall_tester;

    i = 0;
    antall_feil = 0;
    antall_tester = 0;

    int (*gdp_test_function_ptr_array[]) ( ) = {
            gdp_test_modell_opprett_datafil_a,
            gdp_test_modell_opprett_datafil_b,
            gdp_test_modell_opprett_datafil_c,
            gdp_test_modell_opprett_datafil_d

    };

    antall_tester = 4;

    for ( i = 0; i < antall_tester; i++ ) {
        if ( gdp_test_function_ptr_array[i]() > 0 )
            antall_feil++;
    }

    GDPL_log ( GDPL_INFO, signatur, "Antall tester %d, antall ok %d, antall feil %d", antall_tester, antall_tester - antall_feil, antall_feil );

    GDPL_log ( GDPL_DEBUG, signatur, "Slutt funksjon." );
    return antall_feil;

}
