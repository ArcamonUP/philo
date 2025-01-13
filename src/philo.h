/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:40 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/13 15:05:46 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo
{
	int				num;
	struct timeval	tv;
	int				left_fork;
	int				right_fork;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_eat;
}	t_philo;

pthread_t	*init(int ac, char **av, t_philo *data);

int			print_move(t_philo *data, char *str);
int			ft_atoi(const char *str);
void		free_thread(pthread_t *tid, int num);
t_philo		*copy_data(t_philo data);

#endif