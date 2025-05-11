#include "utils.h"

void	clear_input_buffer(void)
{
	int	c;
	
	while ((c = getchar()) != '\n' && c != EOF);
}

void	error_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}