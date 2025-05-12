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
	
	/* Zone de conversation */
	move_cursor(1, 1);
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
	printf(RESET "\n");
	
	/* Séparation entre chat et input */
	move_cursor(height - INPUT_HEIGHT, 1);
	printf(CYAN);
	for (i = 0; i < width; i++)
		printf("─");
	printf(RESET "\n");
	
	/* Zone d'input */
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
	int		msg_len;
	int		user_len;
	int		box_width;
	int		terminal_width;
	int		terminal_height;
	int		padding;
	int		i;
	static int	current_line = 4;
	
	/* Sauvegarder la position du curseur */
	save_cursor_position();
	
	terminal_width = get_terminal_width();
	terminal_height = get_terminal_height();
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
	
	/* Vérifier si on doit scroller */
	if (current_line > terminal_height - INPUT_HEIGHT - 1)
	{
		/* Scroller vers le haut */
		printf("\033[M");
		current_line = terminal_height - INPUT_HEIGHT - 1;
	}
	
	/* Se déplacer à la ligne courante */
	move_cursor(current_line, 1);
	
	/* Espacement */
	for (i = 0; i < padding; i++)
		printf(" ");
	
	/* Ligne du haut */
	printf("%s%s", color, TOP_LEFT);
	for (i = 0; i < box_width; i++)
		printf(HORIZONTAL);
	printf("%s%s\n", TOP_RIGHT, RESET);
	
	current_line++;
	move_cursor(current_line, 1);
	
	/* Message */
	for (i = 0; i < padding; i++)
		printf(" ");
	printf("%s%s %s%s%s", color, VERTICAL, BOLD, message, RESET);
	for (i = 0; i < box_width - msg_len - 1; i++)
		printf(" ");
	printf("%s%s%s\n", color, VERTICAL, RESET);
	
	current_line++;
	move_cursor(current_line, 1);
	
	/* Ligne du bas avec username */
	for (i = 0; i < padding; i++)
		printf(" ");
	printf("%s%s", color, BOTTOM_LEFT);
	for (i = 0; i < box_width - user_len - 1; i++)
		printf(HORIZONTAL);
	printf(" %s%s%s%s%s%s\n", DIM, username, RESET, color, BOTTOM_RIGHT, RESET);
	
	current_line++;
	current_line++;
	
	/* Restaurer la position du curseur (zone d'input) */
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
	
	/* Sauvegarder la position du curseur */
	save_cursor_position();
	
	terminal_width = get_terminal_width();
	terminal_height = get_terminal_height();
	msg_len = strlen(message);
	padding = (terminal_width - msg_len - 4) / 2;
	
	if (padding < 0)
		padding = 2;
	
	/* Vérifier si on doit scroller */
	if (current_line > terminal_height - INPUT_HEIGHT - 1)
	{
		printf("\033[M");
		current_line = terminal_height - INPUT_HEIGHT - 1;
	}
	
	move_cursor(current_line, 1);
	
	for (i = 0; i < padding; i++)
		printf(" ");
	
	printf("%s▶ %s%s%s\n\n", GRAY, CYAN, message, RESET);
	
	current_line += 2;
	
	/* Restaurer la position du curseur */
	restore_cursor_position();
	fflush(stdout);
}