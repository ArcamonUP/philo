/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:40 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/17 11:42:13 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo
{
	int				num_total;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	struct timeval	tv;
	pthread_mutex_t	**mutex;
	int				alive;
}	t_philo;

typedef struct s_thread
{
	int		num;
	t_philo	*data;
}	t_thread;

//init.c
pthread_t	*init(int ac, char **av, t_philo *data);

//moves.c
int			philo_took_fork(t_thread thread, int fork, struct timeval tv);
int			philo_eat(t_thread thread, struct timeval tv);
int			philo_sleep(t_thread thread, struct timeval tv);
int			philo_think(t_thread thread, struct timeval tv);

//utils.c
int			ft_atoi(const char *str);
t_thread	*cp_data(t_philo *data, int i);
int			is_out_of_time(t_thread thread, struct timeval tv);
void		*ft_calloc(size_t nmemb, size_t size);

//free.c
void		free_thread(pthread_t *tid, int num);
void		free_mutex(t_philo data);

void		print_move(t_thread thread, char *str);
//a enlever
#endif