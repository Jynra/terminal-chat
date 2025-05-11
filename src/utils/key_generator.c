#include "utils.h"

void	generate_random_key(char *key, int length)
{
	const char	charset[] = "abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int			i;
	
	srand(time(NULL));
	
	for (i = 0; i < length; i++)
		key[i] = charset[rand() % (sizeof(charset) - 1)];
	
	key[length] = '\0';
}