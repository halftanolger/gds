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
**
** The development of this library is so called test driven. Therefor every
** public function has several tests related to it. These test functions are
** defined in this file. Reading the tests, may help you understading how to
** utilize the library.
**
** The main function here is GDPL_test(). If you call this one, and it return
** zero, everything should be fine. If not, you are in for a debug session.
**
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "gdpl.h"


int GDPL_test_GDPL_controller_set_filename_a();
int GDPL_test_GDPL_controller_set_filename_b();
int GDPL_test_GDPL_controller_set_filename_c();

int GDPL_test_GDPL_controller_read_from_file_a();
int GDPL_test_GDPL_controller_read_from_file_b();


int GDPL_run_test(int (*function_ptr)()) 
{
	return (*function_ptr)();
}


/* 
 * Function 
 *  int GDPL_test() 
 *  
 * Description 
 *  Runs the GDPLib test suite. 
 * 
 * Parameters  
 * 
 * Return 
 *  0 - ok
 *  any thing else - one or more test failed.  
 * 
 * Examples of Usage
 *  int r;
 *  r = GDPL_test();
 *  if (r > 0) 'find out why'
 * 
 */ 
int GDPL_test() 
{

	const char* signature = "test()";

	int r, number_of_tests, number_of_tests_ok, number_of_tests_failed;

	int (*function_ptr_array[])() = { 

		GDPL_test_GDPL_controller_set_filename_a,
		GDPL_test_GDPL_controller_set_filename_b,
		GDPL_test_GDPL_controller_set_filename_c,

                GDPL_test_GDPL_controller_read_from_file_a,
                GDPL_test_GDPL_controller_read_from_file_b

 	};

	number_of_tests = 5;
	number_of_tests_ok = 0;
	number_of_tests_failed = 0;
	
	GDPL_util_log(INFO, signature, "Running %d tests.", number_of_tests);
	for (r = 0; r < number_of_tests; r++) {
		if (GDPL_run_test( function_ptr_array[r] ) == 0) 
			number_of_tests_ok++;
        	else
			number_of_tests_failed++;
	}
	GDPL_util_log(INFO, signature, "%d ok, %d failed",
		number_of_tests_ok,number_of_tests_failed);

	if (number_of_tests_failed > 0)
		GDPL_util_log(WARNING, signature, 
			" === THERE ARE %d FAILED %s ===",
			number_of_tests_failed, 
			number_of_tests_failed == 1?"TEST":"TESTS");

	return number_of_tests_failed;
}


/*
 *
 * ----- GDPLibrary test suite ------------------------------------------------
 *
 */


/* 
 * Test description
 * 
 *  If the data file do not exist; create the file, and initiate the 
 *  controllers data. I.e. create the root node for the competition list, 
 *  and let the competition_list_root_ptr point to it. Let the
 *  competition_list_selected_ptr point to zero.
 * 
 */ 
int GDPL_test_GDPL_controller_read_from_file_a()
{

	const char* signature = "GDPL_test_GDPL_controller_read_from_file_a()";

	/* Set up test case. The data filename shall be a non existing file. */	
	int random_number, seed;
	char random_filename[128];
	seed = time(NULL);
  	srand(seed);
	random_number = rand();
	sprintf(random_filename,"%d_tempfile.dat",random_number);
	GDPL_util_log(DEBUG, signature, "Create temporary filename %s", random_filename);
	 
	GDPL_controller_set_filename(random_filename);
        GDPL_controller_read_from_file();

	/* Clean up test case. */
	GDPL_util_log(DEBUG, signature, "Remove temporary file %s", random_filename);
	remove(random_filename); 

	/* Post condition is to be that the filename is an existing file,
           and that competition_list_root_ptr points to a root node, and
           that competition_list_selected_ptr point to zero. */  

	if (GDPL_controller_competition_list_root_ptr == 0) {
		GDPL_util_log(DEBUG, signature, "Test a FAILED");
		return 1;
	}
	if (GDPL_controller_competition_list_root_ptr->id != 0) {
		GDPL_util_log(DEBUG, signature, "Test b FAILED");
		return 1;
	}
	if (GDPL_controller_competition_list_root_ptr->year != 0) {
		GDPL_util_log(DEBUG, signature, "Test c FAILED");
		return 1;
	}
	if (GDPL_controller_competition_list_root_ptr->person_list_root_ptr != 0) {
		GDPL_util_log(DEBUG, signature, "Test d FAILED");
		return 1;
	}
	if (GDPL_controller_competition_list_root_ptr->couple_list_root_ptr != 0) {
		GDPL_util_log(DEBUG, signature, "Test e FAILED");
		return 1;
	}
	if (GDPL_controller_competition_list_root_ptr->next != 0) {
		GDPL_util_log(DEBUG, signature, "Test f FAILED");
		return 1;
	}

	GDPL_util_log(DEBUG, signature, "Test OK");
	return 0;
}


/* 
 * Test description
 * 
 *  If the data file do exist; create the file, and initiate the 
 *  controllers data. TODO: beskriv hva som skal initieres ...
 * 
 */ 
int GDPL_test_GDPL_controller_read_from_file_b()
{

	const char* signature = "GDPL_test_GDPL_controller_read_from_file_b()";

	/* Set up test case */
	
	//GDPL_controller_read_from_file();

	GDPL_util_log(DEBUG, signature, "Test FAILED - not implemented");
	return 1;
}



/* 
 * Test description
 * 
 *  If GDPL_controller_set_filename() gets a null pointer as argument,
 *  the data file should get its default name, gdp.dat.
 * 
 */ 
int GDPL_test_GDPL_controller_set_filename_a() 
{

	const char* signature = "GDPL_test_GDPL_controller_set_filename_a()";

	GDPL_controller_set_filename(0);

	if (strcmp("gdp.dat",GDPL_controller_data_file_name) == 0) {
		GDPL_util_log(DEBUG, signature, "Test OK");
		return 0;
	} else {
		GDPL_util_log(DEBUG, signature, "Test FAILED");
		return 1;
        }
		
}

/* 
 * Test description
 * 
 *  If GDPL_controller_set_filename() gets a filename as argument,
 *  the data file should get its name from that string.
 * 
 */ 
int GDPL_test_GDPL_controller_set_filename_b() 
{

	const char* signature = "GDPL_test_GDPL_controller_set_filename_b()";

	const char* the_filename = "mydata.dat";

	GDPL_controller_set_filename(the_filename);

	if (strcmp(the_filename,GDPL_controller_data_file_name) == 0) {
		GDPL_util_log(DEBUG, signature, "Test OK");
		return 0;
	} else {
		GDPL_util_log(DEBUG, signature, "Test FAILED");
		return 1;
        }

}

/* 
 * Test description
 * 
 *  If GDPL_controller_set_filename() gets a filename as argument,
 *  and the filename length > GDPL_MAX_DATA_FILENAME_LENGTH, the 
 *  GDPL_controller_set_filename() function shall return an 
 *  integer > 0 indicating an error number.
 * 
 */ 
int GDPL_test_GDPL_controller_set_filename_c() 
{

	const char* signature = "GDPL_test_GDPL_controller_set_filename_c()";
	
	char to_long_filename[GDPL_MAX_DATA_FILENAME_LENGTH + 1];

	memset(&to_long_filename,'x',GDPL_MAX_DATA_FILENAME_LENGTH + 1);
        to_long_filename[GDPL_MAX_DATA_FILENAME_LENGTH + 1]='\0';
		
	if (GDPL_controller_set_filename(to_long_filename) > 0) {
		GDPL_util_log(DEBUG, signature, "Test OK");
		return 0;
	} else {
		GDPL_util_log(DEBUG, signature, "Test FAILED");
		return 1;
        }

}



