
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>     
#include "gdpl.h"

void GDPL_util_log(enum GDPL_log_type type, const char* signature, char* message, ...) {

 	time_t rawtime;
  	struct tm * timeinfo; 	
	va_list ap;
	char *p, *sval;
        char type_str[16] = "";
	int ival;
	double dval;

	if (GDPL_log_level > type)
		return;

        switch (type) {
	case DEBUG:
		strcpy(type_str,"DEBUG");
		break;
 	case INFO:
		strcpy(type_str,"INFO");
		break;
	case WARNING:
		strcpy(type_str,"WARNING");
		break;
	case ERROR:
		strcpy(type_str,"ERROR");
		break;
	default:
		strcpy(type_str,"");
	}

  	time (&rawtime);
  	timeinfo = localtime (&rawtime);
	char time_str[32];
	sprintf(time_str,"%d:%d:%d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

	printf("%s %s %s: ",type_str, time_str, signature);

	va_start(ap, message);
	for (p = message; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		switch (*++p) {
		case 'd':
			ival = va_arg(ap, int);
			printf("%d", ival);
			break;
		case 'f':
			dval = va_arg(ap, double);
			printf("%f", dval);
			break;
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++)
				putchar(*sval);
			break;
		default:
			putchar(*p);
			break;
		}
	}
	
	printf("\n");

}

