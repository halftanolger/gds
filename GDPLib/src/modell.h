/*! \file modell.h
    \brief GDPL modell    
    
    Definerer datamodellen til GDPL, samt funksjonalitet for å skrive og lese 
    denne modellen til og fra ei fil. 
    
    Datamodellen består av ei lenka liste av konkurransenoder. Hver konkurransenode 
    har ei lenka liste med person-noder og ei lenka liste med par-noder.
*/    

#ifndef MODELL_H
#define MODELL_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \def GDPL_MAX_PERSONNAVN_LENGDE
    \brief Max antall tegn et person-navn kan være.
    
    Med tegn menes her 'unsigned int'. Et UTF-8 encoded tegn kan være opp til fire slike 'tegn'.
*/
#define GDPL_MAX_PERSONNAVN_LENGDE           128
/*! \def GDPL_MIN_PERSONNAVN_LENGDE
    \brief Minimum antall tegn en person-navn må være.
*/
#define GDPL_MIN_PERSONNAVN_LENGDE           1
/*! \def GDPL_MAX_FILNAVN_LENGDE
    \brief Max antall tegn et fil-navn kan være.
    
    Med tegn menes her 'unsigned int'. Et UTF-8 encoded tegn kan være opp til fire slike 'tegn'.
*/
#define GDPL_MAX_FILNAVN_LENGDE              516
/*! \def GDPL_MIN_FILNAVN_LENGDE
    \brief Minimum antall tegn en person-navn må være.
*/
#define GDPL_MIN_FILNAVN_LENGDE              1
/*! \def GDPL_MAX_ANTALL_PAR
    \brief Max antall par som det er mulig å registrere.
    
    I forbindelse med logikk som skal trekke et tilfeldig tall, må bruker av 
    denne koden definere et tak, eller vindu, som det skal trekkes innenfor.
    Denne verdien angir en max-verdi for dette vinduet.
*/
#define GDPL_MAX_ANTALL_PAR                  200

/*! \struct GDPL_tid
    \brief Definerer atributtene til en tid.   
    
    Benyttes som dataholder ifm registrering av start- og måltid, anvendt tid
    og middeltid.    
*/
struct GDPL_tid {
    int timer;
    int minutt;
    int sekund;
};

/*! \struct GDPL_par_data_node_struct
    \brief Definerer atributtene til et par.   
    
    TODO: forklar ...
*/
struct GDPL_par_data_node_struct {
    int id;
    int herre_person_id;
    int dame_person_id;
    int start_nr;
    struct GDPL_tid start_tid;
    struct GDPL_tid maal_tid;
    struct GDPL_tid anvendt_tid;
    float tids_poeng;
    float oppgave_poeng;
    struct GDPL_par_data_node_struct *neste;
};

/*! \struct GDPL_person_data_node_struct
    \brief Definerer atributtene til en person.   
    
    TODO: forklar ...
*/
struct GDPL_person_data_node_struct {
    int id;    
    char fnavn[GDPL_MAX_PERSONNAVN_LENGDE];
    char enavn[GDPL_MAX_PERSONNAVN_LENGDE];
    struct GDPL_person_data_node_struct *neste;
};

/*! \var typedef struct GDPL_par_data_node_struct GDPL_par_data_node
    \brief En typedefinisjon av GDPL_par_data_node
*/
typedef struct GDPL_par_data_node_struct GDPL_par_data_node;

/*! \var typedef struct GDPL_person_data_node_struct GDPL_person_data_node
    \brief En typedefinisjon av GDPL_person_data_node
*/
typedef struct GDPL_person_data_node_struct GDPL_person_data_node;

/*! \struct GDPL_konkurranse_data_node_struct
    \brief Definerer atributtene til en konkurranse.   
    
    TODO: forklar ...
*/
struct GDPL_konkurranse_data_node_struct {
    int id;
    int aar;
    GDPL_person_data_node *person_liste_root_ptr;
    GDPL_par_data_node *par_liste_root_ptr;
    struct GDPL_konkurranse_data_node_struct *neste;
};

/*! \var typedef struct GDPL_konkurranse_data_node_struct GDPL_konkurranse_data_node
    \brief En typedefinisjon av GDPL_konkurranse_data_node
*/
typedef struct GDPL_konkurranse_data_node_struct  GDPL_konkurranse_data_node;

/*! \var extern GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_root_ptr;
    \brief Definerer root-pekeren for hele datastrukturen.    
*/
extern GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_root_ptr;

/*! \var extern GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_valgt_ptr;
    \brief Definerer den valgte konkurransen. Det vil si den konkurransen 'vi jobber med'. 
*/
extern GDPL_konkurranse_data_node *gdpl_modell_konkurranseliste_valgt_ptr;


/*! \fn int GDPL_modell_angi_filnavn(const char *filnavn)
    \brief Angir \a filnavnet som modellen skal bruke som persistent lager.
    \param filnavn Et filnavn. Om denne pekeren er null, benyttes et default filnavn.
*/
int GDPL_modell_angi_filnavn(const char *filnavn);

/*! \fn int GDPL_modell_les_data()
    \brief Les inn modellen fra fil.
    \return Feilkode i form av en \a int. Om denne er ulik null, har en feil inntruffet. 
    
    Leser inn modellen fra fil. Om fila ikke eksisterer, opprettes denne, og en
    ny tom modell klargjøres til bruk.
*/
int GDPL_modell_les_data();

/*! \fn int GDPL_modell_skriv_data()
    \brief Skriver modellen til fil.
    \return Feilkode i form av en \a int. Om denne er ulik null, har en feil inntruffet. 
    
    TODO: beskriv feilkoder ...
*/
int GDPL_modell_skriv_data();

/*! \fn int GDPL_modell_dump()
    \brief Dumper modellen til log.
    \return Feilkode i form av en \a int. Om denne er ulik null, har en feil inntruffet. 
    
    Log-nivå må være større eller lik 'info'.    
*/
int GDPL_modell_dump();

/*! \fn int GDPL_modell_nullstill()
    \brief Nullstiller en eksisterende modell.
    \return Feilkode i form av en \a int. Om denne er ulik null, har en feil inntruffet. 
    
    Nullstiller eller sletter en modell.
*/
int GDPL_modell_nullstill();

#ifdef __cplusplus
}
#endif

#endif // MODELL_H
