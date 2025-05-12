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
		
		/* Au premier message reçu (pas système), effacer seulement les infos */
		if (!client->chat_started)
		{
			/* Vérifier si c'est un vrai message de chat (pas system) */
			if (sscanf(buffer, "%[^:]:%[^\n]", username, message) == 2 && 
				strcmp(username, "SYSTEM") != 0)
			{
				client->chat_started = 1;
				clear_info_messages();  /* Efface seulement les infos */
			}
		}
		
		/* Parser le message: username:message */
		if (sscanf(buffer, "%[^:]:%[^\n]", username, message) == 2)
		{
			if (strcmp(username, "SYSTEM") == 0)
			{
				/* Afficher les messages système comme des messages normaux */
				print_system_as_message(message);
			}
			else
			{
				/* Messages des autres utilisateurs à GAUCHE */
				print_bordered_message(message, username, CYAN, 0);
			}
		}
		else
		{
			/* Au cas où le format serait différent */
			print_system_message(buffer);
		}
		
		/* Readline gère la zone d'input automatiquement */
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
	char	*input;
	char	prompt[BUFFER_SIZE];
	
	/* Configurer l'interface de chat */
	setup_chat_interface();
	
	/* Afficher les informations de connexion */
	print_system_message("Connected successfully!");
	
	char server_info[BUFFER_SIZE];
	sprintf(server_info, "Server: %s:%d", client->server_ip, PORT);
	print_system_message(server_info);
	
	print_system_message("Type your messages below (Ctrl+C to exit)");
	
	/* Préparer le prompt une seule fois */
	sprintf(prompt, GREEN "%s (You): " RESET, client->pseudo);
	
	while (client->running)
	{
		/* Utiliser readline pour l'input avec édition */
		input = get_input_with_prompt(prompt);
		
		if (!input)  /* Ctrl+D ou erreur */
			break;
		
		if (strlen(input) > 0)
		{
			/* Au premier message envoyé, effacer seulement les infos */
			if (!client->chat_started)
			{
				client->chat_started = 1;
				clear_info_messages();  /* Efface seulement les infos */
			}
			
			/* Affichage local - VOS messages à DROITE avec votre pseudo */
			char display_name[BUFFER_SIZE];
			sprintf(display_name, "%s (You)", client->pseudo);
			print_bordered_message(input, display_name, GREEN, 1);
			
			if (send(client->socket, input, strlen(input), 0) < 0)
			{
				log_message("Failed to send message");
				free(input);
				break;
			}
		}
		
		/* Libérer la mémoire allouée par readline */
		free(input);
	}
	
	client_cleanup(client);
}