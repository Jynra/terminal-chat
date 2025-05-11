#include "network.h"

int	create_socket(void)
{
	int	socket_fd;
	
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		error_exit("Socket creation failed");
	
	return (socket_fd);
}

void	configure_socket(int socket_fd)
{
	int	opt;
	
	opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		error_exit("Setsockopt failed");
}

int	bind_socket(int socket_fd, struct sockaddr_in *addr)
{
	if (bind(socket_fd, (struct sockaddr *)addr, sizeof(*addr)) < 0)
		error_exit("Bind failed");
	
	return (0);
}

void	close_connection(int socket_fd)
{
	if (socket_fd > 0)
		close(socket_fd);
}