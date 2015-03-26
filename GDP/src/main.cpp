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

#include <locale>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include "gdpl.h"

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
int filversjon(const char* filnavn, int loglevel);

int main(int argc, char **argv)
{
    std::setlocale(LC_ALL,"");

    if (argc > 1) {
        if (strcmp(argv[1],"-v")==0) {
            const char *navn = GDPL_kontroller_gdplib_navn();
            const char *ver =  GDPL_kontroller_gdplib_versjon();
            std::wcout << L"\nGubberenn Dataprogram er basert på " << navn << " " << ver << "\n" << std::endl;
            return 0;
        }
        if (strcmp(argv[1],"-h")==0) {
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"   -x- Gubberenn Dataprogram ver. 1.0 -x-   " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Gubberenn  Dataprogram  er  et  hjelpemiddel" << std::endl;
            std::wcout << L"som kan benyttes i forbindelse med avvikling" << std::endl;
            std::wcout << L"av et Gubberenn. Et Gubberenn er et  skirenn" << std::endl;
            std::wcout << L"som arrangeres hver  skjaertorsdag i  påske-" << std::endl;
            std::wcout << L"uken av Bjørkebakken & Omegn Grendelag.     " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Når man  arrangerer  dette  rennet,  må  man" << std::endl;
            std::wcout << L"regne ut noen tider og poengsummer.         " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Gubberenn Dataprogram beregner; anvendt tid," << std::endl;
            std::wcout << L"gjennomsnittstid, avvik mellom  anvendt tid " << std::endl;
            std::wcout << L"og gjennomsnittstid,  tidspoeng  og  totalt " << std::endl;
            std::wcout << L"antall poeng, dvs summen av oppgavepoeng og " << std::endl;
            std::wcout << L"tidspoeng.                                  " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"             tidspoeng = 60 - x             " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"  x er ant. minutt avvik fra middeltiden    " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Input  til programmet er et regneark, f.eks " << std::endl;
            std::wcout << L"et Excel-regneark, lagret  som  ei  csv-fil." << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Eksempel på et regneark  som  kan  benyttes:" << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Startnummer;Herre-fornavn;Herre-etternavn;Dame-fornavn;Dame-etternavn;Start-tid;Slutt-tid;Oppgave-poeng" << std::endl;
            std::wcout << L"100;Ola1;Nordmann1;Kari1;Nordkvinne1;12:00:00;14:32:00;20" << std::endl;
            std::wcout << L"101;Ola2;Nordmann2;Kari2;Nordkvinne2;12:01:00;14:00:00;21" << std::endl;
            std::wcout << L"102;Ola3;Nordmann3;Kari3;Nordkvinne3;12:02:00;14:36:00;22" << std::endl;
            std::wcout << L"103;Ola4;Nordmann4;Kari4;Nordkvinne4;12:03:00;14:38:00;23" << std::endl;
            std::wcout << L"104;Ola5;Nordmann5;Kari5;Nordkvinne5;12:04:00;14:50:00;24" << std::endl;
            std::wcout << L"105;Ola6;Nordmann6;Kari6;Nordkvinne6;12:05:00;14:42:00;25" << std::endl;
            std::wcout << L"106;Ola7;Nordmann7;Kari7;Nordkvinne7;12:06:00;14:44:00;26" << std::endl;
            std::wcout << L"107;Ola8;Nordmann8;Kari8;Nordkvinne8;12:07:00;14:46:00;27" << std::endl;
            std::wcout << L"108;Ola9;Nordmann9;Kari9;Nordkvinne9;12:08:00;14:14:00;28" << std::endl;
            std::wcout << L"109;Ola10;Nordmann10;Kari10;Nordkvinne10;12:09:00;15:00:00;29" << std::endl;
            std::wcout << L"110;Ola11;Nordmann11;Kari11;Nordkvinne11;12:10:00;15:02:00;30" << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Dette regnearket gir man til programmet, som" << std::endl;
            std::wcout << L"vist nedenfor:                              " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"    gdp.exe -i testinput.csv                " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Programmet  oppretter da et  nytt  regneark," << std::endl;
            std::wcout << L"som inneholder  de kolonnene som  'mangler':" << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"Plassering;Startnummer;Herre-navn;Dame-navn;Start-tid;Slutt-tid;Oppgave-poeng;Beregnet middel-tid;Beregnet anvendt-tid;Beregnet avveket-tid;Beregnet tids-poeng;Beregnet total-poeng;" << std::endl;
            std::wcout << L"1;102;Ola3 Nordmann3;Kari3 Nordkvinne3;12:2:0;14:36:0;22;2:33:32;2:34:0;0:0:28;60;82;" << std::endl;
            std::wcout << L"2;103;Ola4 Nordmann4;Kari4 Nordkvinne4;12:3:0;14:38:0;23;2:33:32;2:35:0;0:1:28;59;82;" << std::endl;
            std::wcout << L"3;105;Ola6 Nordmann6;Kari6 Nordkvinne6;12:5:0;14:42:0;25;2:33:32;2:37:0;0:3:28;57;82;" << std::endl;
            std::wcout << L"4;106;Ola7 Nordmann7;Kari7 Nordkvinne7;12:6:0;14:44:0;26;2:33:32;2:38:0;0:4:28;56;82;" << std::endl;
            std::wcout << L"5;107;Ola8 Nordmann8;Kari8 Nordkvinne8;12:7:0;14:46:0;27;2:33:32;2:39:0;0:5:28;55;82;" << std::endl;
            std::wcout << L"6;100;Ola1 Nordmann1;Kari1 Nordkvinne1;12:0:0;14:32:0;20;2:33:32;2:32:0;0:1:32;59;79;" << std::endl;
            std::wcout << L"7;104;Ola5 Nordmann5;Kari5 Nordkvinne5;12:4:0;14:50:0;24;2:33:32;2:46:0;0:12:28;48;72;" << std::endl;
            std::wcout << L"8;109;Ola10 Nordmann10;Kari10 Nordkvinne10;12:9:0;15:0:0;29;2:33:32;2:51:0;0:17:28;43;72;" << std::endl;
            std::wcout << L"9;110;Ola11 Nordmann11;Kari11 Nordkvinne11;12:10:0;15:2:0;30;2:33:32;2:52:0;0:18:28;42;72;" << std::endl;
            std::wcout << L"10;108;Ola9 Nordmann9;Kari9 Nordkvinne9;12:8:0;14:14:0;28;2:33:32;2:6:0;0:27:32;33;61;" << std::endl;
            std::wcout << L"11;101;Ola2 Nordmann2;Kari2 Nordkvinne2;12:1:0;14:0:0;21;2:33:32;1:59:0;0:34:32;26;47;" << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"  https://github.com/halftanolger/gds.git   " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"               God Påske                    " << std::endl;
            std::wcout << L"                                            " << std::endl;
            std::wcout << L"                                            " << std::endl;
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

    std::wcout << L"\nGubberenn Dataprogram ver. 1.0 " << std::endl;
    std::wcout << L"\nFeil: mangler input.\n\nEksempel på bruk:\n" << std::endl;
    std::wcout << "  " << argv[0] << L" -i inputfil.cvs\n" << std::endl;
    std::wcout << "  " << argv[0] << L" -v \n" << std::endl;
    std::wcout << "  " << argv[0] << L" -h \n" << std::endl;
    return 0;
}

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str,line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while(std::getline(lineStream,cell,';'))
    {
        result.push_back(cell);
    }
    return result;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

int filversjon(const char* filnavn, int loglevel)
{
    const char* signatur = "filversjon";
    if (loglevel == 0)
        GDPL_log_init(GDPL_ERROR, stdout);
    else
        GDPL_log_init(GDPL_DEBUG, stdout);

    const char *navn = GDPL_kontroller_gdplib_navn();
    const char *ver =  GDPL_kontroller_gdplib_versjon();
    GDPL_log(GDPL_INFO, signatur, "%s %s", navn, ver);

    if (GDPL_modell_angi_filnavn(0) > 0)
        return 1;

    if (GDPL_modell_les_data() > 0)
        return 1;

    int antall = 0;
    if (GDPL_konkurranse_antall_i_liste(&antall) > 0)
        return 1;

    if (antall > 0) {
        if (GDPL_modell_nullstill()>0) {
            return 1;
        }
    }

    /* Her skal vi ha 'blank' modell. */

    GDPL_modell_dump();

    /* Legg til en ny konkurranse. */

    GDPL_konkurranse_data_node *k = 0;
    if (GDPL_konkurranse_opprett_node(&k)>0)
        return FEILKODE_FEIL;

    GDPL_person_data_node* person = 0;
    if (GDPL_person_opprett_node(&person)>0)
        return FEILKODE_FEIL;

    GDPL_par_data_node* par = 0;
    if (GDPL_par_opprett_node(&par)>0)
        return FEILKODE_FEIL;

    k->id = 1;
    k->aar = 2015;
    k->person_liste_root_ptr = person;
    k->par_liste_root_ptr = par;

    if (GDPL_konkurranse_legg_til(k) > 0)
        return FEILKODE_FEIL;

    /* Velg denne konkurransen */

    if (GDPL_konkurranse_sett_valgt_konkurranse(1)>0)
        return FEILKODE_FEIL;

    /* Last inn data fra cvs-fil. */

    std::ifstream file(filnavn);

    if (!file.is_open()) {
        GDPL_log(GDPL_ERROR,"Klarer ikke aa aapne fila %s", filnavn);
        return FEILKODE_FEIL;
    }

    /* Første linje er bare kolonnenavn. */

    std::vector<std::string> v = getNextLineAndSplitIntoTokens(file);

    bool loop = true;
    do {

        v = getNextLineAndSplitIntoTokens(file);
        if (v.size() == 0) {
            loop = false;
        } else {

            int antall = v.size();

            /* Antall kolonner skal være 8 */

            if (antall != 8) {
                GDPL_log(GDPL_ERROR, signatur, "Feil antall kolonner i cvs-input-fil. Er %d, skal være 8.",antall);
            }

            std::string  start_nr_str = v.at(0);
            int start_nr = atoi(start_nr_str.c_str());

            std::string hfnavn_str = v.at(1);
            const char *hfnavn = hfnavn_str.c_str();

            std::string henavn_str = v.at(2);
            const char *henavn = henavn_str.c_str();

            std::string dfnavn_str = v.at(3);
            const char *dfnavn = dfnavn_str.c_str();

            std::string denavn_str = v.at(4);
            const char *denavn = denavn_str.c_str();

            std::string start_tid_str = v.at(5);
            std::vector<std::string> start_tid_elementer = split(start_tid_str, ':');
            int start_tid_t = atoi(start_tid_elementer.at(0).c_str());
            int start_tid_m = atoi(start_tid_elementer.at(1).c_str());
            int start_tid_s = atoi(start_tid_elementer.at(2).c_str());

            std::string maal_tid_str = v.at(6);
            std::vector<std::string> maal_tid_elementer = split(maal_tid_str, ':');
            int maal_tid_t = atoi(maal_tid_elementer.at(0).c_str());
            int maal_tid_m = atoi(maal_tid_elementer.at(1).c_str());
            int maal_tid_s = atoi(maal_tid_elementer.at(2).c_str());

            std::string oppgave_poeng_str = v.at(7);

            const char *str = oppgave_poeng_str.c_str();
            double d;
            sscanf(str, "%lf", &d);
            d *= 100;
            d = ((int)(d * 1 + .5) / 1.0);
            int oppgave_poeng = (int)d;

            std::cout << oppgave_poeng << std::endl;


            /* Legg til herre-person */

            GDPL_person_data_node *person = 0;
            if(GDPL_person_opprett_node(&person)>0)
                return 1;

            int ny_id_h = 0;
            if(GDPL_person_finn_neste_ledige_id(&ny_id_h)>0)
                return 1;

            person->id = ny_id_h;
            strcpy(person->fnavn,hfnavn);
            strcpy(person->enavn,henavn);
            if (GDPL_person_legg_til(person) > 0)
                return 1;

            /* Legg til dame-person */

            person = 0;
            GDPL_person_opprett_node(&person);
            int ny_id_d = 0;
            if(GDPL_person_finn_neste_ledige_id(&ny_id_d)>0)
                return 1;

            person->id = ny_id_d;
            strcpy(person->fnavn,dfnavn);
            strcpy(person->enavn,denavn);
            if (GDPL_person_legg_til(person) > 0)
                return 1;

            /* Opprett par */

            GDPL_par_data_node *par = 0;
            GDPL_par_opprett_node(&par);
            int ny_id = 0;
            if(GDPL_par_finn_neste_ledige_id(&ny_id)>0)
                return 1;

            par->id = ny_id;
            par->herre_person_id = ny_id_h;
            par->dame_person_id = ny_id_d;
            par->start_nr=start_nr;
            par->start_tid.timer = start_tid_t;
            par->start_tid.minutt = start_tid_m;
            par->start_tid.sekund = start_tid_s;
            par->maal_tid.timer = maal_tid_t;
            par->maal_tid.minutt = maal_tid_m;
            par->maal_tid.sekund = maal_tid_s;
            par->oppgave_poeng = oppgave_poeng;

            if (GDPL_par_legg_til(par) > 0)
                return 1;
        }

    } while (loop);

    if (GDPL_par_beregn()>0)
        return 1;

    GDPL_modell_dump();

    int antall_par = 0;
    if (GDPL_par_antall_i_liste(&antall_par)>0)
        return 1;

    std::string fn(filnavn);
    fn += "_output.csv";

    std::ofstream myfile;
    myfile.open(fn);

    if (!myfile.is_open()) {
        GDPL_log(GDPL_ERROR,"Klarer ikke aa aapne fila %s", fn.c_str());
        return FEILKODE_FEIL;
    }

    myfile << "Plassering" << ";"
           << "Startnummer" << ";"
           << "Herre-navn" << ";"
           << "Dame-navn" << ";"
           << "Start-tid" << ";"
           << "Slutt-tid" << ";"
           << "Oppgave-poeng" << ";"
           << "Beregnet middel-tid" << ";"
           << "Beregnet anvendt-tid" << ";"
           << "Beregnet avveket-tid" << ";"
           << "Beregnet tids-poeng" << ";"
           << "Beregnet total-poeng" << ";\n";

    struct GDPL_tid middel_tid;
    if (GDPL_par_beregn_middel_tid(&middel_tid)>0)
        return FEILKODE_FEIL;

    for (int i=1; i<=antall_par; i++) {
        GDPL_par_data_node *data = 0;
        if( GDPL_par_hent_i_rekke(i, &data)>0) {
            myfile.close();
            return 1;
        }

        GDPL_person_data_node *hperson = 0;
        if (GDPL_person_hent(data->herre_person_id, &hperson)>0)
            return 1;

        GDPL_person_data_node *dperson = 0;
        if (GDPL_person_hent(data->dame_person_id, &dperson)>0)
            return 1;

        struct GDPL_tid avveket_tid;
        if (GDPL_par_beregn_avvik(&avveket_tid, middel_tid, data->anvendt_tid)>0)
            return 1;

        myfile << i << ";"
               << data->start_nr << ";"
               << hperson->fnavn << " "
               << hperson->enavn << ";"
               << dperson->fnavn << " "
               << dperson->enavn << ";"
               << data->start_tid.timer << ":" << data->start_tid.minutt << ":" << data->start_tid.sekund << ";"
               << data->maal_tid.timer << ":" << data->maal_tid.minutt << ":" << data->maal_tid.sekund << ";"
               <<  ( (double)data->oppgave_poeng / 100.0 )  << ";"
               << middel_tid.timer << ":" << middel_tid.minutt << ":" << middel_tid.sekund << ";"
               << data->anvendt_tid.timer << ":" << data->anvendt_tid.minutt << ":" << data->anvendt_tid.sekund << ";"
               << avveket_tid.timer << ":" << avveket_tid.minutt << ":" << avveket_tid.sekund << ";"
               << ( (double)data->tids_poeng / 100.0 ) << ";"
               << ( ( (double)data->tids_poeng / 100.0 ) + (double)data->oppgave_poeng / 100.0) << ";\n";

    }

    myfile.close();
    return 0;

}




