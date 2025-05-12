#include "utils.h"
#include "colors.h"

void	clear_screen(void)
{
	printf("\033[2J\033[H");
}

int	get_terminal_width(void)
{
	struct winsize	w;
	
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return (w.ws_col);
}

void	print_welcome_banner(void)
{
	int		width;
	int		i;
	
	width = get_terminal_width();
	clear_screen();
	
	printf(CYAN BOLD);
	for (i = 0; i < width; i++)
		printf("═");
	printf("\n");
	
	printf("║");
	for (i = 0; i < (width - 20) / 2; i++)
		printf(" ");
	printf("Terminal Chat App");
	for (i = 0; i < (width - 20) / 2; i++)
		printf(" ");
	printf("║\n");
	
	for (i = 0; i < width; i++)
		printf("═");
	printf(RESET "\n\n");
}

void	print_bordered_message(const char *message, const char *username, 
	const char *color, int align_right)
{
	int		msg_len;
	int		user_len;
	int		box_width;
	int		terminal_width;
	int		padding;
	int		i;
	
	terminal_width = get_terminal_width();
	user_len = strlen(username);
	msg_len = strlen(message);
	box_width = (msg_len > user_len) ? msg_len + 4 : user_len + 4;
	
	if (box_width > terminal_width - 10)
		box_width = terminal_width - 10;
	
	if (box_width < 15)
		box_width = 15;
	
	if (align_right)
		padding = terminal_width - box_width - 5;
	else
		padding = 2;
	
	/* Espacement */
	for (i = 0; i < padding; i++)
		printf(" ");
	
	/* Ligne du haut */
	printf("%s%s", color, TOP_LEFT);
	for (i = 0; i < box_width; i++)
		printf(HORIZONTAL);
	printf("%s%s\n", TOP_RIGHT, RESET);
	
	/* Message */
	for (i = 0; i < padding; i++)
		printf(" ");
	printf("%s%s %s%s%s", color, VERTICAL, BOLD, message, RESET);
	for (i = 0; i < box_width - msg_len - 1; i++)
		printf(" ");
	printf("%s%s%s\n", color, VERTICAL, RESET);
	
	/* Ligne du bas avec username */
	for (i = 0; i < padding; i++)
		printf(" ");
	printf("%s%s", color, BOTTOM_LEFT);
	for (i = 0; i < box_width - user_len - 1; i++)
		printf(HORIZONTAL);
	printf(" %s%s%s%s%s%s\n", DIM, username, RESET, color, BOTTOM_RIGHT, RESET);
	
	printf("\n");
}

void	print_system_message(const char *message)
{
	int		terminal_width;
	int		msg_len;
	int		padding;
	int		i;
	
	terminal_width = get_terminal_width();
	msg_len = strlen(message);
	padding = (terminal_width - msg_len - 4) / 2;
	
	if (padding < 0)
		padding = 2;
	
	for (i = 0; i < padding; i++)
		printf(" ");
	
	printf("%s▶ %s%s%s\n\n", GRAY, CYAN, message, RESET);
}