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


/* 'Bruk' -tekst, skal vises når korrekt input args mangler. */

const char* bruk_info = "BRUK: gubb [-i ARG] [-o ARG] [-l ARG] [-r ARG] [-f ARG] [-k ARG] [-s ARG] [-hvb] ";

/* Hjelpetekst, -h [ --hjelp ] */

const char* hjelp_info = "\n"
"Dette er et hjelpemiddel ifm arrangering av det tradisjonelle Gubberennet, som arrangeres hver skjærtorsdag i Bjørkebakken.\n"
"\n"
"BRUK: gubb [-i ARG] [-o ARG] [-l ARG] [-r ARG] [-f ARG] [-k ARG] [-s ARG] [-hvb] \n"
"\n"
"\n"
"-- Beskrivelse av opsjoner --\n"
"\n"
"Programmet tar inn en rekke argumenter. De to viktigste er 'inputfile' og 'outputfile'. "
"Disse angis ved hhv -i 'filnavn' og -o 'filnavn'. Videre kan man bruke opsjonen -r for å "
"produsere en del ulike rapporter som skal være 100% lik de rapportene som Jan-Erik's DataEase "
"program ville ha produsert, gitt den samme 'input'. Om man i tillegg bruker -f 'filnavn', "
"kan man få en slik rapport lagret til ei tekstfil.\n"
"\n"
"\n"
"Opsjoner og argumenter:\n"
"\n"
" -h [ --hjelp ]              Skriver en hjelpetekst ut på konsollet\n"
" -b [ --bruksanvisning ]     Skriver ei enkel bruksanvisning ut på konsollet\n"
" -v [ --versjon ]            Skriver ut versjonsnummeret til GDPLib\n"
" -l [ --logg ] nivå          Loggnivå, hvor nivå er INF eller DBG\n"
" -i [ --inputfil ] filnavn   Input csv-datafil\n"
" -o [ --outputfil ] filnavn  Output csv-datafil\n"
" -r [ --rapport ] type       Rapport, type er START, RES1 og RES2\n"
" -f [ --rapportfil ] filnavn Rapport, type er START, RES1 og RES2\n"
" -k [ --klient ] portnr      Start gubb-klient, default port er 1234\n"
" -s [ --klient ] portnr      Start gubb-server, default port er 1234\n"
"\n"
"\n"
"Eksempler på bruk:\n"
"\n"
" gubb.exe -i c:/tmp/input.cvs -o c:/tmp/output.cvs \n"
"\n"
"    Produserer ei outputfil basert på ei inputfil. Inputfila produseres typiskt i Excel, i.e. man registrerer inn data vha "
"et regneark, f.eks Microsoft Excel eller LibreOffice Calc. Videre vil man typisk åpne outputfila i et tilsvarende "
"programm.\n"
"\n"
" gubb.exe -i c:/tmp/input.cvs -o c:/tmp/output.cvs -r START -f c:/tmp/start.txt\n"
"\n"
"    Samme som eksempelet ovenfor, bare med den forskjelle at nå blir det også produsert en rapport som lagres i ei "
"tekst fil kalt 'c:/tmp/start.txt'. Rapporten som opprettes er av typen 'START', i.e. ei startliste.\n"
"\n"
"\n"
"For mer info, se ulike hjelpefiler som følger med programmet.\n"
"halftan@online.no\n"
"\n";

/* Bruksanvisningstekst, -b [ --bruksanvisning ] */

const char* bruksanvisning_info = "\n"
        "\n"
        "Hensikten med denne bruksanvisningen er å forklare hvordan man kan benytte programmet i sin aller "
        "enkleste form. Det vil si å kun benytte programmet til å utføre de aktuelle beregningene, som er: "
        "\n"
        "\n"
        "   - beregne anvendt tid pr par\n"
        "   - beregne gjennomsnittstiden, aka idealtiden\n"
        "   - beregne avvik mellom anvendt tid og idealtid, pr. par\n"
        "   - beregne tidspoeng basert på beregnet avvik\n"
        "   - beregne sum poeng; tidspoeng + oppgavepoeng\n"
        "\n"
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
        "  gdp.exe -i mininputfil.csv -o output_mininputfil.cvs \n"
        "\n"
        "skal resultere i at fila output_mininputfil.cvs blir opprettet. Denne fila åpner du typisk i Excel "
        "for videre behandling.\n"
        "\n"
        "-TheEnd-\n"
        "\n"
        "\n";


int filversjon(char* inputfil, int loglevel);



#ifdef __cplusplus
}
#endif

#endif // MAIN_H


