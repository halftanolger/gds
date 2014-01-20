
#include <stdio.h>
#include "gdpl.h"

int main (int argc, char *argv[]) 
{

	const char* signature = "main(int,char)";

	/* Say Hello world */
	GDPL_log_level = DEBUG;	
   	GDPL_util_log(DEBUG, signature, "Hello world - %s %s", GDPL_controller_gdplib_name, GDPL_controller_gdplib_version);

	/* Run test suite */
	GDPL_test();

	return 0;
	
}
