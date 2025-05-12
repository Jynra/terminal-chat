#include "client.h"
#include "colors.h"

void	*receive_messages(void *arg)
{
	t_client	*client;
	char		buffer[BUFFER_SIZE];
	char		username[MAX_PSEUDO_LENGTH];
	char		message[BUFFER_SIZE];
	int			read_size;
	
	client = (t_client *)arg;
	
	while (client->running && 
		(read_size = recv(client->socket, buffer, BUFFER_SIZE - 1, 0)) > 0)
	{
		buffer[read_size] = '\0';
		
		/* Parser le message: username:message */
		if (sscanf(buffer, "%[^:]:%[^\n]", username, message) == 2)
		{
			if (strcmp(username, "SYSTEM") == 0)
				print_system_message(message);
			else
			{
				/* Choisir une couleur basée sur le nom d'utilisateur */
				const char *color = LIGHT_BLUE;
				if (strstr(username, client->pseudo) != NULL)
					color = GREEN;
				else if (strlen(username) % 2 == 0)
					color = PURPLE;
				
				print_bordered_message(message, username, color, 1);
			}
		}
		else
		{
			print_system_message(buffer);
		}
	}
	
	if (read_size == 0)
		log_message("Server disconnected");
	else if (read_size < 0)
		log_message("Error receiving message");
	
	client->running = 0;
	
	return (NULL);
}

void	send_message(t_client *client)
{
	char	message[BUFFER_SIZE];
	
	clear_screen();
	print_welcome_banner();
	printf(GREEN BOLD "Connected successfully!\n\n" RESET);
	printf(YELLOW "Server: " CYAN "%s:%d\n" RESET, client->server_ip, PORT);
	printf(GRAY "Type your messages below (Ctrl+C to exit)\n" RESET);
	printf(CYAN "─────────────────────────────────────────\n\n" RESET);
	
	while (client->running)
	{
		printf(GREEN "You: " RESET);
		if (fgets(message, BUFFER_SIZE, stdin) == NULL)
			break;
		
		message[strcspn(message, "\n")] = '\0';
		
		if (strlen(message) > 0)
		{
			/* Affichage local avec bordure */
			print_bordered_message(message, "You", GREEN, 1);
			
			if (send(client->socket, message, strlen(message), 0) < 0)
			{
				log_message("Failed to send message");
				break;
			}
		}
	}
	
	client_cleanup(client);
}