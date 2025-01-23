/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:40 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/23 11:59:28 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>

typedef struct s_shared_value
{
	pthread_mutex_t	*mutex;
	int				value;
}	t_shared;

typedef struct s_philo
{
	int				num_total;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_eat;
	struct timeval	tv;
	pthread_mutex_t	**mutex;
	pthread_mutex_t	*writing;
	t_shared		alive;
}	t_philo;

typedef struct s_thread
{
	int		num;
	t_philo	*data;
}	t_thread;

//init.c
void		synchronize(t_thread *thread);
pthread_t	*init(int ac, char **av, t_philo *data);

//moves.c
void		print_move(t_thread thread, char *str);
int			philo_took_fork(t_thread th, struct timeval tv);
int			philo_eat(t_thread thread, struct timeval tv);
int			philo_sleep(t_thread thread, struct timeval tv);

//utils.c
int			ft_atoi(const char *str);
int			is_out_of_time(t_thread thread, struct timeval tv, int move);
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_putnbr(long long int n);
int			ft_strlen(char *str);

//free.c
void		free_thread(pthread_t *tid, int num);
void		free_mutex(t_philo data);

//threads.c
t_thread	*cp_data(t_philo *data, int i);

#endif