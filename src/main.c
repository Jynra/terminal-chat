#include <signal.h>
#include "server.h"
#include "client.h"

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
		log_message("Server key: %s", server.key);
		log_message("Share this key with clients who want to connect");
		
		start_server(&server);
		server_cleanup(&server);
	}
	else if (choice == 2)
	{
		t_client	client;
		char		server_key[KEY_LENGTH + 1];
		
		printf("Enter server key: ");
		scanf("%16s", server_key);
		clear_input_buffer();
		
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
		log_message("Invalid choice. Exiting...");
		return (1);
	}
	
	return (0);
}

void	display_menu(void)
{
	printf("===== Terminal Chat =====\n");
	printf("1 - Start Server\n");
	printf("2 - Connect to Server\n");
	printf("Choose an option (1 or 2): ");
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
	printf("Enter your pseudo: ");
	fgets(pseudo, MAX_PSEUDO_LENGTH, stdin);
	
	pseudo[strcspn(pseudo, "\n")] = '\0';
}

void	handle_sigint(int sig)
{
	(void)sig;
	
	printf("\nExiting chat application...\n");
	
	if (g_server)
		server_cleanup(g_server);
	
	if (g_client)
		client_cleanup(g_client);
	
	exit(0);
}