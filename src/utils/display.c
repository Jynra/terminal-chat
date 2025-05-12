#include "utils.h"
#include "colors.h"

/* Variable globale pour la ligne courante - partagée entre toutes les fonctions */
static int	g_current_line = 4;

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

void	clear_info_messages(void)
{
	int		i;
	int		height;
	
	height = get_terminal_height();
	
	/* Effacer depuis la ligne 4 jusqu'à la ligne courante - 1 */
	/* Cela efface tous les messages d'info quel que soit leur nombre */
	for (i = 4; i < g_current_line && i < height - 3; i++)
	{
		move_cursor(i, 1);
		clear_line();
	}
	
	/* Remettre le curseur après les informations et réinitialiser le compteur */
	move_cursor(4, 1);
	g_current_line = 4;
	fflush(stdout);
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
	
	/* Réinitialiser le compteur de lignes */
	g_current_line = 4;
	
	/* Ne pas appeler setup_input_area() ici, 
	   laisser l'appelant décider quel format utiliser */
}

void	setup_input_area(void)
{
	int	height;
	
	height = get_terminal_height();
	move_cursor(height - 1, 1);
	clear_line();
	printf(GREEN "You: " RESET);
	fflush(stdout);
}

void	setup_input_area_with_pseudo(const char *pseudo)
{
	int	height;
	
	height = get_terminal_height();
	move_cursor(height - 1, 1);
	clear_line();
	printf(GREEN "%s (You): " RESET, pseudo);
	fflush(stdout);
}

char	*get_input_with_prompt(const char *prompt)
{
	int		height;
	char	*input;
	
	/* Position le curseur en bas de l'écran */
	height = get_terminal_height();
	move_cursor(height - 1, 1);
	clear_line();
	
	/* Utilise readline avec le prompt coloré */
	input = readline(prompt);
	
	/* Ajouter à l'historique si non vide */
	if (input && *input)
		add_history(input);
	
	return (input);
}

void	print_bordered_message(const char *message, const char *username, 
	const char *color, int align_right)
{
	int		terminal_width;
	int		terminal_height;
	int		msg_len;
	int		padding;
	int		i;
	
	save_cursor_position();
	
	terminal_width = get_terminal_width();
	terminal_height = get_terminal_height();
	msg_len = strlen(message) + strlen(username) + 2;
	
	/* Vérifier si on doit scroller */
	if (g_current_line > terminal_height - 4)
	{
		/* On scroll en remontant tout */
		g_current_line = terminal_height - 4;
		/* Ici on pourrait implémenter un vrai scroll mais c'est complexe */
	}
	
	move_cursor(g_current_line, 1);
	
	if (align_right)
	{
		/* Messages à droite - Seulement le pseudo en couleur */
		padding = terminal_width - msg_len - 5;
		for (i = 0; i < padding; i++)
			printf(" ");
		printf("%s%s%s: %s\n", color, username, RESET, message);
	}
	else
	{
		/* Messages à gauche - Seulement le pseudo en couleur */
		printf("%s%s%s: %s\n", color, username, RESET, message);
	}
	
	g_current_line++;
	g_current_line++;  /* Ligne supplémentaire pour l'espacement */
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
	
	save_cursor_position();
	
	terminal_width = get_terminal_width();
	terminal_height = get_terminal_height();
	msg_len = strlen(message) + 2;
	padding = (terminal_width - msg_len) / 2;
	
	/* Vérifier si on doit scroller */
	if (g_current_line > terminal_height - 4)
	{
		g_current_line = terminal_height - 4;
	}
	
	move_cursor(g_current_line, 1);
	
	for (i = 0; i < padding; i++)
		printf(" ");
	
	printf("%s▶ %s%s\n", GRAY, message, RESET);
	
	g_current_line++;
	g_current_line++;  /* Ligne supplémentaire pour l'espacement */
	restore_cursor_position();
	fflush(stdout);
}

void	print_system_as_message(const char *message)
{
	int		terminal_width;
	int		terminal_height;
	int		msg_len;
	int		padding;
	int		i;
	
	save_cursor_position();
	
	terminal_width = get_terminal_width();
	terminal_height = get_terminal_height();
	msg_len = strlen(message);
	padding = (terminal_width - msg_len) / 2;
	
	/* Vérifier si on doit scroller */
	if (g_current_line > terminal_height - 4)
	{
		g_current_line = terminal_height - 4;
	}
	
	move_cursor(g_current_line, 1);
	
	/* Centrer le message */
	if (padding > 0)
	{
		for (i = 0; i < padding; i++)
			printf(" ");
	}
	
	printf("%s%s%s\n", YELLOW, message, RESET);
	
	g_current_line++;
	g_current_line++;  /* Ligne supplémentaire pour l'espacement */
	restore_cursor_position();
	fflush(stdout);
}