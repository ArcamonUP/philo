/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:50:41 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/26 19:23:38 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

// A classic str to int converter
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

//A classic calloc function (malloc + bzero)
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
