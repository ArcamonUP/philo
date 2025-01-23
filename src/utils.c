/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:50:41 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/23 12:04:54 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int	ft_atoi(const char *str)
{
	int	nb;
	int	i;
	int	result;

	i = 0;
	nb = 1;
	result = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			nb = -nb;
		i++;
	}
	while (str[i] && '0' <= str[i] && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * nb);
}

int	is_out_of_time(t_thread thread, struct timeval tv, int move)
{
	struct timeval	tv2;
	long long int	current_time;
	long long int	max_time;

	gettimeofday(&tv2, NULL);
	max_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) + thread.data->time_die;
	current_time = (tv2.tv_sec * 1000) + (tv2.tv_usec / 1000);
	pthread_mutex_lock(thread.data->alive.mutex);
	if (!thread.data->alive.value || current_time > max_time)
	{
		if (thread.data->alive.value)
		{
			thread.data->alive.value = 0;
			print_move(thread, "died");
		}
		pthread_mutex_unlock(thread.data->alive.mutex);
		return (-1);
	}
	pthread_mutex_unlock(thread.data->alive.mutex);
	if (current_time + move < max_time)
		return (move * 1000);
	return ((max_time - current_time + 1) * 1000);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void			*result;
	unsigned char	*ptr;
	int				n;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (nmemb >= (SIZE_MAX / size))
		return (NULL);
	result = malloc(nmemb * size);
	if (result == NULL)
		return (NULL);
	ptr = (unsigned char *)result;
	n = nmemb * size;
	while (n > 0)
	{
		*ptr = '\0';
		ptr++;
		n--;
	}
	return (result);
}

void	ft_putnbr(long long int n)
{
	int				size;
	long long int	temp;
	char			c;

	size = 1;
	temp = n;
	while (temp / 10 != 0)
	{
		temp /= 10;
		size *= 10;
	}
	while (size != 0)
	{
		c = (n / size) + '0';
		write(STDOUT_FILENO, &c, 1);
		n = n % size;
		size = size / 10;
	}
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
