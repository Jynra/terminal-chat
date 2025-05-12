#include "client.h"
#include "colors.h"

int	init_client(t_client *client, const char *pseudo, const char *key)
{
	memset(client, 0, sizeof(t_client));
	strncpy(client->pseudo, pseudo, MAX_PSEUDO_LENGTH - 1);
	strncpy(client->server_key, key, KEY_LENGTH);
	client->running = 1;
	client->chat_started = 0;	/* Initialiser le nouveau champ */
	
	client->socket = create_socket();
	if (client->socket < 0)
		return (-1);
	
	return (0);
}

int	connect_client(t_client *client)
{
	t_connection	conn;
	char			*input;
	
	input = readline(YELLOW "Enter server IP address: " CYAN);
	
	if (input && strlen(input) > 0)
	{
		strncpy(client->server_ip, input, 15);
		client->server_ip[15] = '\0';
	}
	else
	{
		strcpy(client->server_ip, "127.0.0.1");  /* IP par défaut */
	}
	
	printf(RESET);
	free(input);
	
	conn.socket = client->socket;
	if (connect_to_server(&conn, client->server_ip) < 0)
	{
		log_message("Failed to connect to %s:%d", client->server_ip, PORT);
		return (-1);
	}
	
	/* Envoyer la clé (sans retour à la ligne) */
	if (send(client->socket, client->server_key, KEY_LENGTH, 0) < 0)
	{
		log_message("Failed to send key");
		return (-1);
	}
	
	/* Petite pause pour s'assurer que la clé est bien reçue */
	usleep(10000);
	
	/* Envoyer le pseudo */
	if (send(client->socket, client->pseudo, strlen(client->pseudo), 0) < 0)
	{
		log_message("Failed to send pseudo");
		return (-1);
	}
	
	log_message("Connected to server at %s:%d", client->server_ip, PORT);
	pthread_create(&client->recv_thread, NULL, receive_messages, client);
	
	return (0);
}

void	client_cleanup(t_client *client)
{
	client->running = 0;
	
	if (client->socket > 0)
		close_connection(client->socket);
	
	pthread_cancel(client->recv_thread);
}