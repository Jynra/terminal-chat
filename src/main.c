#include <signal.h>
#include "server.h"
#include "client.h"
#include "colors.h"

void	display_menu(void);
int		get_user_choice(void);
void	get_user_pseudo(char *pseudo);
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
	get_user_pseudo(pseudo);
	
	if (choice == 1)
	{
		t_server	server;
		
		if (init_server(&server, pseudo) < 0)
		{
			log_message("Failed to initialize server");
			return (1);
		}
		
		g_server = &server;
		clear_screen();
		print_welcome_banner();
		printf(GREEN BOLD "Server successfully started!\n\n" RESET);
		printf(YELLOW "Server key: " CYAN BOLD "%s\n" RESET, server.key);
		printf(DIM "Share this key with clients who want to connect\n" RESET);
		printf(CYAN "─────────────────────────────────────────\n\n" RESET);
		
		start_server(&server);
		server_cleanup(&server);
	}
	else if (choice == 2)
	{
		t_client	client;
		char		server_key[KEY_LENGTH + 1];
		
		clear_screen();
		print_welcome_banner();
		printf(BLUE "Connecting to server...\n\n" RESET);
		printf("Enter server key: " CYAN);
		scanf("%16s", server_key);
		clear_input_buffer();
		printf(RESET);
		
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
	print_welcome_banner();
	
	printf(CYAN "┌─────────────────────────┐\n");
	printf("│   Choose your option:   │\n");
	printf("├─────────────────────────┤\n");
	printf("│  " GREEN "1" CYAN " - Start Server        │\n");
	printf("│  " BLUE "2" CYAN " - Connect to Server   │\n");
	printf("└─────────────────────────┘\n" RESET);
	printf("\nEnter your choice (1 or 2): ");
}

int	get_user_choice(void)
{
	int	choice;
	
	scanf("%d", &choice);
	clear_input_buffer();
	
	return (choice);
}

void	get_user_pseudo(char *pseudo)
{
	printf("\n" YELLOW "Enter your pseudo: " CYAN);
	fgets(pseudo, MAX_PSEUDO_LENGTH, stdin);
	printf(RESET);
	
	pseudo[strcspn(pseudo, "\n")] = '\0';
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