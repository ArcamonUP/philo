/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:40 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/26 16:04:26 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>

# define WAIT 2
# define ALIVE 1
# define DEAD 0
# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

typedef struct s_shared_value
{
	sem_t	*semaphore;
	int		value;
}	t_shared;

typedef struct s_philo
{
	int				num_total;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_eat;
	struct timeval	tv;
	sem_t			*writing;
	t_shared		alive;
	t_shared		forks;
}	t_philo;

typedef struct s_thread
{
	int		num;
	t_philo	*data;
}	t_thread;

//init.c
pthread_t	*init(int ac, char **av, t_philo *data, t_thread ***t_data);

//moves.c
void		print_move(t_thread thread, char *str, int i);
int			philo_took_fork(t_thread th, struct timeval tv);
int			philo_eat(t_thread thread, struct timeval tv);
int			philo_sleep_think(t_thread thread, struct timeval tv);

//utils.c
int			ft_atoi(const char *str);
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_putnbr(long long int n);
int			ft_strlen(char *str);

//free.c
int			end_process(t_philo *data, t_thread **thd, pthread_t *tid, int ret);
void		end_semaphore(t_philo data);
void		free_t_data(t_thread **t_data);

//threads.c
t_thread	*cp_data(t_philo *data, int i);
int			synchronize(t_thread *thread);
int			is_out_of_time(t_thread thread, struct timeval tv, int move);

#endif