#ifndef SERVER_H
# define SERVER_H

# include <pthread.h>
# include "network.h"
# include "utils.h"

# define MAX_CLIENTS 10

typedef struct s_client_info
{
	int			socket;
	char		pseudo[MAX_PSEUDO_LENGTH];
	pthread_t	thread;
	int			active;
}	t_client_info;

typedef struct s_server
{
	int				socket;
	char			key[KEY_LENGTH + 1];
	char			pseudo[MAX_PSEUDO_LENGTH];
	t_client_info	clients[MAX_CLIENTS];
	int				client_count;
	pthread_mutex_t	mutex;
	int				running;
}	t_server;

int		init_server(t_server *server, const char *pseudo);
int		start_server(t_server *server);
void	*handle_client(void *arg);
void	broadcast_message(t_server *server, const char *message, int sender_id);
void	server_cleanup(t_server *server);

#endif