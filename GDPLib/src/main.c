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

#include <stdio.h>
#include "gdpl.h"
#include "main.h"

int main (int argc, char *argv[]) 
{           
    if (argc > 1) {
        if (strcmp(argv[1],"-v")==0) {
            const char *navn = GDPL_kontroller_gdplib_navn();
            const char *ver =  GDPL_kontroller_gdplib_versjon();
            printf("\nGubberenn Dataprogram er basert på %s %s\n", navn, ver);
            return 0;
        }
        if (strcmp(argv[1],"-h")==0) {
            printf("%s",info);
            return 0;
        }

        if (strcmp(argv[1],"-i")==0 && argc >= 3) {

            int loglevel = 0;
            if (argc == 5 && strcmp(argv[3],"-l")==0 && strcmp(argv[4],"debug")==0) {
                loglevel = 1;
            }

            const char *filnavn = argv[2];
            return filversjon(filnavn,loglevel);
        }
    }
    printf("\n");
    printf("\nFeil: mangler input.\n\nEksempel på bruk:\n");
    printf(" -i inputfil.cvs\n");
    printf(" -v \n");
    printf(" -h \n");
    return 0;
}

int filversjon(char* inputfil, int loglevel)
{
    return 0;
}
