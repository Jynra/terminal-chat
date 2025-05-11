#include "network.h"

int	connect_to_server(t_connection *conn, const char *server_ip)
{
	conn->addr.sin_family = AF_INET;
	conn->addr.sin_port = htons(PORT);
	
	if (inet_pton(AF_INET, server_ip, &conn->addr.sin_addr) <= 0)
		return (-1);
	
	if (connect(conn->socket, (struct sockaddr *)&conn->addr, 
		sizeof(conn->addr)) < 0)
		return (-1);
	
	return (0);
}

int	accept_connection(int server_socket, struct sockaddr_in *client_addr)
{
	int			client_fd;
	socklen_t	addr_len;
	
	addr_len = sizeof(*client_addr);
	client_fd = accept(server_socket, (struct sockaddr *)client_addr, &addr_len);
	
	return (client_fd);
}