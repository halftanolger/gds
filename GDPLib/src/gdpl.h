
#ifndef _GDPL_H_
#define _GDPL_H_


#define GDPL_MAX_DATA_FILENAME_LENGTH  255


/*
 * Error codes. If a library function return anything but zero, it will
 * be one of these codes. You can use this number as an index to the
 * string array GDPL_controller_error_codes[] to get the actual error
 * message.
 */
#define ERROR_FILENAME_TO_LONG           1
#define ERROR_FILENAME_TO_SHORT          2
#define ERROR_FILENAME_IS_NOT_SET        3
#define ERROR_CAN_NOT_CREATE_DATAFILE    4
#define ERROR_CAN_NOT_ALLOCATE_MEMORY    5


enum GDPL_log_type {DEBUG, INFO, WARNING, ERROR};

enum GDPL_log_type GDPL_log_level;

struct GDPL_couple_data_node {
	int id;
	int male_id;
	int female_id;
	int start_nr;
	char start_time[16];
	char finish_time[16];
	double assignment_score;	
	struct GDPL_person_data_node *next;
};

struct GDPL_person_data_node {
	int id;
	char fname[64];
	char lname[64];
	struct GDPL_person_data_node *next;
};

struct GDPL_competition_data_node {
	int id;
	int year;
        struct GDPL_person_data_node *person_list_root_ptr;
	struct GDPL_couple_data_node *couple_list_root_ptr;
	struct GDPL_competition_data_node *next;
};

extern char* GDPL_controller_gdplib_name;
extern char* GDPL_controller_gdplib_version;
extern char* GDPL_controller_error_codes[];
extern char GDPL_controller_data_file_name[];
extern struct GDPL_competition_data_node *GDPL_controller_competition_list_root_ptr;
extern struct GDPL_competition_data_node *GDPL_controller_competition_list_selected_ptr;


/* 
 * Functions declaration. 
 */


/* Controller */

int GDPL_controller_set_filename(const char *filename);

int GDPL_controller_save_to_file();

int GDPL_controller_read_from_file();

/* Competition */

struct GDPL_competition_data_node* GDPL_competition_create_empty_node(); 

/* Util */

void GDPL_util_log(enum GDPL_log_type, const char*, char*, ...);

/* Test */

int GDPL_test();



#endif //GDPL_H
