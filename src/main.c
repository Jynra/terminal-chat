#include <signal.h>
#include <stdlib.h>  /* Pour system() */
#include "server.h"
#include "client.h"
#include "colors.h"

void	display_menu(void);
int		get_user_choice(void);
void	get_user_pseudo(char *pseudo);
void	get_server_key(char *server_key);
void	handle_sigint(int sig);

t_server	*g_server = NULL;
t_client	*g_client = NULL;

int	main(void)
{
	int		choice;
	char	pseudo[MAX_PSEUDO_LENGTH];
	
	signal(SIGINT, handle_sigint);
	
	display_menu();
	choice = get_user_choice();
	
	if (choice == 1)
	{
		t_server	server;
		
		/* Pour le serveur, pas besoin de pseudo, utiliser un nom par défaut */
		strcpy(pseudo, "Server");
		
		if (init_server(&server, pseudo) < 0)
		{
			log_message("Failed to initialize server");
			return (1);
		}
		
		g_server = &server;
		/* Ne pas afficher de bannière ici, elle sera affichée dans start_server() */
		
		start_server(&server);
		server_cleanup(&server);
	}
	else if (choice == 2)
	{
		t_client	client;
		char		server_key[KEY_LENGTH + 1];
		
		/* Clear l'écran au début du processus client avec system */
		system("clear");
		print_welcome_banner();
		printf(BLUE "Connecting to server...\n\n" RESET);
		
		/* Utiliser readline pour la clé */
		get_server_key(server_key);
		
		/* Demander le pseudo ici, APRÈS la clé */
		get_user_pseudo(pseudo);
		
		if (init_client(&client, pseudo, server_key) < 0)
		{
			log_message("Failed to initialize client");
			return (1);
		}
		
		g_client = &client;
		
		if (connect_client(&client) < 0)
		{
			log_message("Failed to connect to server");
			return (1);
		}
		
		send_message(&client);
	}
	else
	{
		printf(RED "Invalid choice. Exiting...\n" RESET);
		return (1);
	}
	
	return (0);
}

void	display_menu(void)
{
	system("clear");  /* Clear avant d'afficher le menu principal */
	print_welcome_banner();
	
	/* Utiliser des caractères ASCII pour le menu */
	printf(CYAN "+-------------------------+\n");
	printf("|   Choose your option:   |\n");
	printf("+-------------------------+\n");
	printf("|  " GREEN "1" CYAN " - Start Server        |\n");
	printf("|  " BLUE "2" CYAN " - Connect to Server   |\n");
	printf("+-------------------------+\n" RESET);
	printf("\nEnter your choice (1 or 2): ");
}

int	get_user_choice(void)
{
	int	choice;
	
	scanf("%d", &choice);
	clear_input_buffer();
	
	return (choice);
}

void	get_server_key(char *server_key)
{
	char	*input;
	
	input = readline(YELLOW "Enter server key: " CYAN);
	
	if (input && strlen(input) > 0)
	{
		strncpy(server_key, input, KEY_LENGTH);
		server_key[KEY_LENGTH] = '\0';
	}
	else
	{
		strcpy(server_key, "");  /* Clé vide si rien entré */
	}
	
	printf(RESET);
	free(input);
}

void	get_user_pseudo(char *pseudo)
{
	char	*input;
	
	printf("\n");
	input = readline(YELLOW "Enter your pseudo: " CYAN);
	
	if (input && strlen(input) > 0)
	{
		strncpy(pseudo, input, MAX_PSEUDO_LENGTH - 1);
		pseudo[MAX_PSEUDO_LENGTH - 1] = '\0';
	}
	else
	{
		strcpy(pseudo, "Anonymous");  /* Pseudo par défaut */
	}
	
	printf(RESET);
	free(input);
}

void	handle_sigint(int sig)
{
	(void)sig;
	
	printf("\n\n" YELLOW "Exiting chat application...\n" RESET);
	
	if (g_server)
		server_cleanup(g_server);
	
	if (g_client)
		client_cleanup(g_client);
	
	exit(0);
}