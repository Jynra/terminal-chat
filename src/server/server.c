#include "server.h"
#include "colors.h"

int	init_server(t_server *server, const char *pseudo)
{
	int					i;
	struct sockaddr_in	addr;
	
	memset(server, 0, sizeof(t_server));
	strncpy(server->pseudo, pseudo, MAX_PSEUDO_LENGTH - 1);
	generate_random_key(server->key, KEY_LENGTH);
	server->client_count = 0;
	server->running = 1;
	
	for (i = 0; i < MAX_CLIENTS; i++)
		server->clients[i].active = 0;
	
	if (pthread_mutex_init(&server->mutex, NULL) != 0)
		return (-1);
	
	server->socket = create_socket();
	configure_socket(server->socket);
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	
	if (bind_socket(server->socket, &addr) < 0)
		return (-1);
	
	if (listen(server->socket, MAX_CLIENTS) < 0)
		return (-1);
	
	return (0);
}

int	start_server(t_server *server)
{
	/* Configurer l'interface de chat pour le serveur */
	setup_chat_interface();
	
	char welcome_msg[BUFFER_SIZE];
	sprintf(welcome_msg, "Server started with key: %s", server->key);
	print_system_message(welcome_msg);
	
	sprintf(welcome_msg, "Waiting for connections on port %d...", PORT);
	print_system_message(welcome_msg);
	
	sprintf(welcome_msg, "Share this key with clients: %s", server->key);
	print_system_message(welcome_msg);
	
	while (server->running)
	{
		int					i;
		t_client_info		*client;
		struct sockaddr_in	client_addr;
		int					client_socket;
		char				client_key[KEY_LENGTH + 1];
		char				client_pseudo[MAX_PSEUDO_LENGTH];
		int					bytes_read;
		
		client_socket = accept_connection(server->socket, &client_addr);
		if (client_socket < 0)
		{
			log_message("Accept failed");
			continue;
		}
		
		/* Recevoir exactement KEY_LENGTH caractères pour la clé */
		bytes_read = recv(client_socket, client_key, KEY_LENGTH, MSG_WAITALL);
		if (bytes_read != KEY_LENGTH)
		{
			log_message("Failed to receive complete key");
			close_connection(client_socket);
			continue;
		}
		
		client_key[KEY_LENGTH] = '\0';
		
		if (strcmp(client_key, server->key) != 0)
		{
			log_message("Client with invalid key tried to connect");
			close_connection(client_socket);
			continue;
		}
		
		/* Recevoir le pseudo */
		bytes_read = recv(client_socket, client_pseudo, MAX_PSEUDO_LENGTH - 1, 0);
		if (bytes_read <= 0)
		{
			log_message("Failed to receive pseudo");
			close_connection(client_socket);
			continue;
		}
		
		client_pseudo[bytes_read] = '\0';
		/* Supprimer les retours à la ligne s'ils existent */
		client_pseudo[strcspn(client_pseudo, "\r\n")] = '\0';
		
		/* Trouver un slot libre pour le client */
		pthread_mutex_lock(&server->mutex);
		for (i = 0; i < MAX_CLIENTS; i++)
		{
			if (!server->clients[i].active)
			{
				server->clients[i].socket = client_socket;
				strncpy(server->clients[i].pseudo, client_pseudo, 
					MAX_PSEUDO_LENGTH - 1);
				server->clients[i].active = 1;
				client = &server->clients[i];
				server->client_count++;
				
				/* Créer une structure pour passer au thread */
				typedef struct {
					t_server *server;
					int client_index;
				} thread_data_t;
				
				thread_data_t *data = malloc(sizeof(thread_data_t));
				data->server = server;
				data->client_index = i;
				
				pthread_create(&client->thread, NULL, handle_client, data);
				break;
			}
		}
		pthread_mutex_unlock(&server->mutex);
		
		if (i == MAX_CLIENTS)
		{
			log_message("Maximum clients reached. Rejecting connection.");
			close_connection(client_socket);
			continue;
		}
		
		log_message("%s has joined the chat", client_pseudo);
		
		char	join_msg[BUFFER_SIZE];
		sprintf(join_msg, "%s has joined the chat", client_pseudo);
		print_system_message(join_msg);
		broadcast_message(server, join_msg, -1);
	}
	
	return (0);
}

void	server_cleanup(t_server *server)
{
	int	i;
	
	server->running = 0;
	
	pthread_mutex_lock(&server->mutex);
	for (i = 0; i < MAX_CLIENTS; i++)
	{
		if (server->clients[i].active)
		{
			close_connection(server->clients[i].socket);
			server->clients[i].active = 0;
		}
	}
	pthread_mutex_unlock(&server->mutex);
	
	close_connection(server->socket);
	pthread_mutex_destroy(&server->mutex);
}