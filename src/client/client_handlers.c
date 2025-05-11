#include "client.h"

void	*receive_messages(void *arg)
{
	t_client	*client;
	char		buffer[BUFFER_SIZE];
	int			read_size;
	
	client = (t_client *)arg;
	
	while (client->running && 
		(read_size = recv(client->socket, buffer, BUFFER_SIZE - 1, 0)) > 0)
	{
		buffer[read_size] = '\0';
		printf("%s\n", buffer);
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
	
	printf("You can start chatting. Type your message:\n");
	
	while (client->running)
	{
		if (fgets(message, BUFFER_SIZE, stdin) == NULL)
			break;
		
		message[strcspn(message, "\n")] = '\0';
		
		if (strlen(message) > 0)
		{
			if (send(client->socket, message, strlen(message), 0) < 0)
			{
				log_message("Failed to send message");
				break;
			}
		}
	}
	
	client_cleanup(client);
}