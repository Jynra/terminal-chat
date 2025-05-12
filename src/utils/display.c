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

int	get_terminal_height(void)
{
	struct winsize	w;
	
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return (w.ws_row);
}

void	move_cursor(int row, int col)
{
	printf("\033[%d;%dH", row, col);
}

void	clear_line(void)
{
	printf("\033[K");
}

void	save_cursor_position(void)
{
	printf("\033[s");
}

void	restore_cursor_position(void)
{
	printf("\033[u");
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

void	setup_chat_interface(void)
{
	int		height;
	int		width;
	int		i;
	
	height = get_terminal_height();
	width = get_terminal_width();
	clear_screen();
	print_welcome_banner();
	
	/* Ligne de séparation en bas */
	move_cursor(height - 2, 1);
	printf(CYAN);
	for (i = 0; i < width; i++)
		printf("─");
	printf(RESET "\n");
	
	setup_input_area();
}

void	setup_input_area(void)
{
	int	height;
	
	height = get_terminal_height();
	move_cursor(height - 1, 1);
	printf(GREEN "You: " RESET);
	fflush(stdout);
}

void	print_bordered_message(const char *message, const char *username, 
	const char *color, int align_right)
{
	int		terminal_width;
	int		terminal_height;
	int		msg_len;
	int		padding;
	int		i;
	static int	current_line = 4;
	
	save_cursor_position();
	
	terminal_width = get_terminal_width();
	terminal_height = get_terminal_height();
	msg_len = strlen(message) + strlen(username) + 2;
	
	/* Vérifier si on doit scroller */
	if (current_line > terminal_height - 4)
	{
		/* On scroll en remontant tout */
		current_line = terminal_height - 4;
		/* Ici on pourrait implémenter un vrai scroll mais c'est complexe */
	}
	
	move_cursor(current_line, 1);
	
	if (align_right)
	{
		/* Messages à droite */
		padding = terminal_width - msg_len - 5;
		for (i = 0; i < padding; i++)
			printf(" ");
		printf("%s%s:%s %s%s\n", color, username, RESET, color, message);
	}
	else
	{
		/* Messages à gauche */
		printf("%s%s:%s %s%s\n", color, username, RESET, color, message);
	}
	
	current_line++;
	restore_cursor_position();
	fflush(stdout);
}

void	print_system_message(const char *message)
{
	int		terminal_width;
	int		terminal_height;
	int		msg_len;
	int		padding;
	int		i;
	static int	current_line = 4;
	
	save_cursor_position();
	
	terminal_width = get_terminal_width();
	terminal_height = get_terminal_height();
	msg_len = strlen(message) + 2;
	padding = (terminal_width - msg_len) / 2;
	
	/* Vérifier si on doit scroller */
	if (current_line > terminal_height - 4)
	{
		current_line = terminal_height - 4;
	}
	
	move_cursor(current_line, 1);
	
	for (i = 0; i < padding; i++)
		printf(" ");
	
	printf("%s▶ %s%s\n", GRAY, message, RESET);
	
	current_line++;
	restore_cursor_position();
	fflush(stdout);
}