#ifndef CLIENT_H
# define CLIENT_H

# include <pthread.h>
# include "network.h"
# include "utils.h"

typedef struct s_client
{
	int			socket;
	char		pseudo[MAX_PSEUDO_LENGTH];
	char		server_key[KEY_LENGTH + 1];
	char		server_ip[16];
	pthread_t	recv_thread;
	int			running;
	int			chat_started;	/* Nouveau champ pour tracking du début du chat */
}	t_client;

int		init_client(t_client *client, const char *pseudo, const char *key);
int		connect_client(t_client *client);
void	*receive_messages(void *arg);
void	send_message(t_client *client);
void	client_cleanup(t_client *client);

#endif