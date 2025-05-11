#ifndef NETWORK_H
# define NETWORK_H

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include "utils.h"

# define PORT 8085

typedef struct s_connection
{
	int					socket;
	struct sockaddr_in	addr;
}	t_connection;

int		create_socket(void);
void	configure_socket(int socket_fd);
int		bind_socket(int socket_fd, struct sockaddr_in *addr);
int		connect_to_server(t_connection *conn, const char *server_ip);
int		accept_connection(int server_socket, struct sockaddr_in *client_addr);
void	close_connection(int socket_fd);

#endif