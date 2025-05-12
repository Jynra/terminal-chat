#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define KEY_LENGTH 16
# define MAX_PSEUDO_LENGTH 32
# define BUFFER_SIZE 1024

void	generate_random_key(char *key, int length);
void	clear_input_buffer(void);
void	log_message(const char *format, ...);
void	error_exit(const char *message);

/* Fonctions pour l'affichage */
void	clear_screen(void);
int		get_terminal_width(void);
int		get_terminal_height(void);
void	print_welcome_banner(void);
void	print_bordered_message(const char *message, const char *username, 
		const char *color, int align_right);
void	print_system_message(const char *message);
void	print_system_as_message(const char *message);
void	setup_chat_interface(void);
void	setup_input_area(void);
void	setup_input_area_with_pseudo(const char *pseudo);
void	clear_info_messages(void);

/* Nouvelle fonction pour readline */
char	*get_input_with_prompt(const char *prompt);

#endif