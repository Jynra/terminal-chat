#include <stdarg.h>
#include "utils.h"

void	log_message(const char *format, ...)
{
	va_list	args;
	time_t	current_time;
	char	time_string[20];
	
	current_time = time(NULL);
	strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", 
		localtime(&current_time));
	
	printf("[%s] ", time_string);
	
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	
	printf("\n");
}