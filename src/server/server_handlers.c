#include "server.h"

void	*handle_client(void *arg)
{
	typedef struct {
		t_server *server;
		int client_index;
	} thread_data_t;
	
	thread_data_t	*data = (thread_data_t *)arg;
	t_server		*server = data->server;
	int				client_index = data->client_index;
	char			buffer[BUFFER_SIZE];
	int				read_size;
	
	free(data);
	
	while ((read_size = recv(server->clients[client_index].socket, buffer, 
		BUFFER_SIZE - 1, 0)) > 0)
	{
		buffer[read_size] = '\0';
		// Supprimer les retours à la ligne
		buffer[strcspn(buffer, "\r\n")] = '\0';
		
		if (strlen(buffer) > 0)
			broadcast_message(server, buffer, client_index);
	}
	
	if (read_size == 0)
	{
		char	disconnect_msg[BUFFER_SIZE];
		
		sprintf(disconnect_msg, "%s has left the chat", 
			server->clients[client_index].pseudo);
		broadcast_message(server, disconnect_msg, -1);
		log_message("%s", disconnect_msg);
	}
	else if (read_size == -1)
		log_message("Recv failed for client %s", 
			server->clients[client_index].pseudo);
	
	pthread_mutex_lock(&server->mutex);
	close_connection(server->clients[client_index].socket);
	server->clients[client_index].active = 0;
	server->client_count--;
	pthread_mutex_unlock(&server->mutex);
	
	return (NULL);
}

void	broadcast_message(t_server *server, const char *message, int sender_id)
{
	int		i;
	char	formatted_msg[BUFFER_SIZE];
	
	if (sender_id >= 0)
		sprintf(formatted_msg, "%s: %s", 
			server->clients[sender_id].pseudo, message);
	else
		strncpy(formatted_msg, message, BUFFER_SIZE - 1);
	
	pthread_mutex_lock(&server->mutex);
	for (i = 0; i < MAX_CLIENTS; i++)
	{
		if (server->clients[i].active && i != sender_id)
			send(server->clients[i].socket, formatted_msg, 
				strlen(formatted_msg), 0);
	}
	pthread_mutex_unlock(&server->mutex);
	
	printf("%s\n", formatted_msg);
}