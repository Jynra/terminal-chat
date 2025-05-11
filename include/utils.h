#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>
# include <errno.h>

# define KEY_LENGTH 16
# define MAX_PSEUDO_LENGTH 32
# define BUFFER_SIZE 1024

void	generate_random_key(char *key, int length);
void	clear_input_buffer(void);
void	log_message(const char *format, ...);
void	error_exit(const char *message);

#endif