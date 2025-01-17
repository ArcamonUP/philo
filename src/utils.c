/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:50:41 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/17 11:07:44 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

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

t_thread	*cp_data(t_philo *data, int i)
{
	t_thread	*result;

	result = (t_thread *)malloc(sizeof(t_thread));
	if (!result)
		return (NULL);
	result->data = data;
	result->num = i + 1;
	return (result);
}

int	is_out_of_time(t_thread thread, struct timeval tv)
{
	struct timeval	tv2;

	gettimeofday(&tv2, NULL);
	if (thread.data->time_die < (tv2.tv_usec - tv.tv_usec))
		return (1);
	return (0);
}


void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n > 0)
	{
		*ptr = '\0';
		ptr++;
		n--;
	}
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
