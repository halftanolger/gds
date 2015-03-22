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

const char* info = "\n"
        "\n"
        "Gubberenn Dataprogram\n"
        "\n"
        "Dette er en enkel testklient for GDPLib. Programmet tar et regneark\n"
        "som input, og beregner \n"
        "\n"
        "   - anvendt tid pr par\n"
        "   - gjennomsnittstid\n"
        "   - avvik mellom anvendt tid og gjennomsnittstid\n"
        "   - tidspoeng basert på beregnet avvik\n"
        "   - sum poeng; tidspoeng + oppgavepoeng\n"
        "\n"
        "Porgrammet oppretter et nytt regneark, kalt 'output_'<filnavn> \n"
        "hvor disse beregnede kolonnene er med.\n"
        "\n"
        "Krav til inputfil:\n"
        "\n"
        "Inputfila må være ei semikolondelt datafil. Ei slik fil kan du \n"
        "opprette i de fleste regneark-programmene, f.eks Excel.\n"
        "\n"
        "Eksempel på input: (viser hvilke kolonner som må være med) \n"
        "\n"
        "Startnummer;Herre-fornavn;Herre-etternavn;Dame-fornavn;Dame-etternavn;Start-tid;Slutt-tid;Oppgave-poeng\n"
        "100;Ola1;Nordmann1;Kari1;Nordkvinne1;12:00:00;14:32:00;20\n"
        "101;Ola2;Nordmann2;Kari2;Nordkvinne2;12:01:00;14:00:00;21\n"
        "102;Ola3;Nordmann3;Kari3;Nordkvinne3;12:02:00;14:36:00;22\n"
        "103;Ola4;Nordmann4;Kari4;Nordkvinne4;12:03:00;14:38:00;23\n"
        "104;Ola5;Nordmann5;Kari5;Nordkvinne5;12:04:00;14:50:00;24\n"
        "105;Ola6;Nordmann6;Kari6;Nordkvinne6;12:05:00;14:42:00;25\n"
        "\n"
        "Eksempel på output:\n"
        "\n"
        "Plassering;Startnummer;Herre-navn;Dame-navn;Start-tid;Slutt-tid;Oppgave-poeng;Beregnet middel-tid;Beregnet anvendt-tid;Beregnet avveket-tid;Beregnet tids-poeng;Beregnet total-poeng;\n"
        "1;102;Ola3 Nordmann3;Kari3 Nordkvinne3;12:2:0;14:36:0;22;2:33:32;2:34:0;0:0:28;60;82;\n"
        "2;103;Ola4 Nordmann4;Kari4 Nordkvinne4;12:3:0;14:38:0;23;2:33:32;2:35:0;0:1:28;59;82;\n"
        "3;105;Ola6 Nordmann6;Kari6 Nordkvinne6;12:5:0;14:42:0;25;2:33:32;2:37:0;0:3:28;57;82;\n"
        "4;106;Ola7 Nordmann7;Kari7 Nordkvinne7;12:6:0;14:44:0;26;2:33:32;2:38:0;0:4:28;56;82;\n"
        "5;107;Ola8 Nordmann8;Kari8 Nordkvinne8;12:7:0;14:46:0;27;2:33:32;2:39:0;0:5:28;55;82;\n"
        "\n"
        "Eksempel på bruk av programmet:\n"
        "\n"
        "  gdp.exe -i mininputfil.csv\n"
        "\n"
        "skal resultere i fila output_mininputfil.cvs. Denne fila åpner du typisk i Excel.\n"
        "\n"
        "\n";

int filversjon(char* inputfil, int loglevel);

#ifdef __cplusplus
}
#endif

#endif // MAIN_H


