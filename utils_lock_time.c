/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lock_time.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysenazsenel <aysenazsenel@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:56:13 by ilsahin           #+#    #+#             */
/*   Updated: 2024/05/16 00:00:14 by aysenazsene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	lock_forks(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right);
	print_status(philo, "has taken a fork");
}

void	unlock_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}

int	print_status(t_philosopher *philo, char *status)
{
	int static	has_printed = 0;

	pthread_mutex_lock(&philo->table->print_mutex);
	if (!has_printed)
	{
		if (status[0] != 'd')
		{
			pthread_mutex_lock(&philo->table->time_mutex);
			printf("%lld philosopher%d %s\n", get_time() - philo->table->time, philo->philo_id, status);
			pthread_mutex_unlock(&philo->table->time_mutex);
		}
		else
		{
			has_printed = 1;
			pthread_mutex_lock(&philo->table->time_mutex);
			printf("%lld philosopher%d died\n", get_time() - philo->table->time, philo->philo_id);
			pthread_mutex_unlock(&philo->table->time_mutex);
		}
		pthread_mutex_unlock(&philo->table->print_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
	return (1);
}

long long	get_current_time(void)
{
	struct timeval	current_time;
	long long		time;

	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}

void	wait_for_duration(int duration, t_philosopher *philo)
{
	long long	start_time;

	start_time = get_current_time();
	while (!check_flags(philo->table) && check_if_alive(philo))
	{
		if (get_current_time() >= start_time + duration)
			break ;
		usleep(60);
	}
}
