#ifndef KONKURRANSE_H
#define KONKURRANSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "diverse.h"
#include "modell.h"


// Variabler



// Funksjoner

int GDPL_konkurranse_opprett_node(GDPL_konkurranse_data_node **new_node);

int GDPL_konkurranse_legg_til(GDPL_konkurranse_data_node *data, GDPL_konkurranse_data_node *root);

int GDPL_konkurranse_fjern_fra(GDPL_konkurranse_data_node* data, GDPL_konkurranse_data_node *root);

int GDPL_konkurranse_hent(int id, GDPL_konkurranse_data_node **data, GDPL_konkurranse_data_node *root);

int GDPL_konkurranse_antall_i_liste(int *antall, GDPL_konkurranse_data_node *root);



#ifdef __cplusplus
}
#endif
#endif // KONKURRANSE_H
