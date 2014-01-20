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
**
** Description
** This file defines functions related to the controller entity.
**
**
**
**
******************************************************************************/


#include <stdio.h>
#include <string.h>
#include "gdpl.h"


char* GDPL_controller_gdplib_name = "GDPLib";

char* GDPL_controller_gdplib_version = "0.1";

char *GDPL_controller_error_codes[] = { "ERROR 0:",                        
	                                "ERROR 1: The filename is to long, max 256.", 
 	                                "ERROR 2: The filename is to short, min 10.",
 	                                "ERROR 3: The filename is not set.",
 	                                "ERROR 4: Can not create the data file."


	                              };

char GDPL_controller_data_file_name[GDPL_MAX_DATA_FILENAME_LENGTH];




           
/* 
 * Function 
 *  int GDPL_controller_set_filename(const char *filename) 
 *  
 * Description 
 *  Set the name of the data file to be used. If the filename is a null pointer
 *  a default filename will be set. 
 * 
 * Parameters  
 *  filename - a filename, or 0 to indicate a default filename. 
 * 
 * Return 
 *  0 - ok
 *  anything else - failure. The returned value can be used as an
 *  index into GDPL_controller_error_codes[] to get an error message.  
 * 
 * Examples of Usage 
 *  GDPL_controller_set_filename("myfile.dat");
 *  GDPL_controller_set_filename(0);
 * 
 */ 
int GDPL_controller_set_filename(const char *filename)
{

	const char* signature = "GDPL_controller_set_filename(const char*)";

    	GDPL_util_log(DEBUG, signature, "Enter method.");

	if (filename == 0) {
		strcpy(GDPL_controller_data_file_name, "gdp.dat");	
	} else {

		if (strlen(filename) > 255) {
     			GDPL_util_log(ERROR, signature, 
				GDPL_controller_error_codes[ERROR_FILENAME_TO_LONG]);

		    	GDPL_util_log(DEBUG, signature, "Exit method.");
			return ERROR_FILENAME_TO_LONG;	
		}

		if (strlen(filename) < 10) {
     			GDPL_util_log(ERROR, signature, 
				GDPL_controller_error_codes[ERROR_FILENAME_TO_SHORT]);

		    	GDPL_util_log(DEBUG, signature, "Exit method.");
			return ERROR_FILENAME_TO_SHORT;	
		}
		strcpy(GDPL_controller_data_file_name, filename);

	}
	
	char msg[GDPL_MAX_DATA_FILENAME_LENGTH + 16];
	sprintf(msg,"data file = %s",GDPL_controller_data_file_name);
     	GDPL_util_log(DEBUG, signature, msg);

    	GDPL_util_log(DEBUG, signature, "Exit method.");

	return 0;

}

/* 
 * Function 
 *  int GDPL_controller_read_from_file() 
 *  
 * Description 
 *  If the data file name exists, open the file, read in data, fill
 *  internal data structures. Let all root pointers point to their
 *  root node. Let all 'selected pointers' point to null.
 *  If the data file does not exists, create the file and make a
 *  competition list root node, which the competition root pointer
 *  shall point to.
 *   
 * Parameters  
 *  nop
 *
 * Return 
 *  0 - ok
 *  anything else - failure. The returned value can be used as an
 *  index into GDPL_controller_error_codes[] to get an error message.  
 * 
 * Examples of Usage
 *  int error_nr; 
 *  error_nr = GDPL_controller_read_from_file()
 *  if (error_nr > 0) 'handle error'
 * 
 */ 
int GDPL_controller_read_from_file()
{

	const char* signature = "GDPL_controller_read_from_file";

    	GDPL_util_log(DEBUG, signature, "Enter method.");

	char *datafilename = GDPL_controller_data_file_name;
	
	if (datafilename == 0) {
     		GDPL_util_log(ERROR, signature, 
				GDPL_controller_error_codes[ERROR_FILENAME_IS_NOT_SET]);
		return ERROR_FILENAME_IS_NOT_SET;			
	}

	FILE *file;
 	file = fopen(datafilename,"r"); 

	if (file == 0) {

		GDPL_util_log(DEBUG, signature, "The file %s does not exist.", datafilename);
		file = fopen(datafilename,"w");
		
		if (file == 0) {
			GDPL_util_log(ERROR, signature, 
					GDPL_controller_error_codes[ERROR_CAN_NOT_CREATE_DATAFILE]);
			GDPL_util_log(DEBUG, signature, "Exit method, error_code=%d",ERROR_CAN_NOT_CREATE_DATAFILE);
			return ERROR_CAN_NOT_CREATE_DATAFILE;
		}

		struct GDPL_competition_data_node *new_node = 0;
            	new_node = GDPL_competition_create_empty_node(); 
		if(new_node == 0) {
			GDPL_util_log(DEBUG, signature, "Exit method, error_code=%d",ERROR_CAN_NOT_ALLOCATE_MEMORY);
			return ERROR_CAN_NOT_ALLOCATE_MEMORY;
		}
			
		GDPL_controller_competition_list_root_ptr = new_node;

		GDPL_controller_competition_list_selected_ptr = 0;

		GDPL_util_log(DEBUG, signature, "Close the file %s", datafilename);

		fclose(file);
		file = 0;

    		GDPL_util_log(DEBUG, signature, "Exit method.");
		return 0;
		

	} else {

		GDPL_util_log(DEBUG, signature, "The file %s does exist.", datafilename);


		//TODO: read data fra file ...

	}
		
    	GDPL_util_log(DEBUG, signature, "Exit method.");
	return 0;
}



int GDPL_controller_save_to_file()
{

	return 0;

}


