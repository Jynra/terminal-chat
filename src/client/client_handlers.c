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
				/* Messages des autres utilisateurs à GAUCHE */
				print_bordered_message(message, username, CYAN, 0);
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
	
	/* Configurer l'interface de chat */
	setup_chat_interface();
	
	/* Afficher les informations de connexion */
	print_system_message("Connected successfully!");
	
	char server_info[BUFFER_SIZE];
	sprintf(server_info, "Server: %s:%d", client->server_ip, PORT);
	print_system_message(server_info);
	
	print_system_message("Type your messages below (Ctrl+C to exit)");
	
	while (client->running)
	{
		/* Positionner le curseur dans la zone d'input */
		setup_input_area();
		
		if (fgets(message, BUFFER_SIZE, stdin) == NULL)
			break;
		
		message[strcspn(message, "\n")] = '\0';
		
		if (strlen(message) > 0)
		{
			/* Affichage local - VOS messages à DROITE */
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