/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysenazsenel <aysenazsenel@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:56:00 by ilsahin           #+#    #+#             */
/*   Updated: 2024/05/15 23:55:44 by aysenazsene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_eating_status(t_philosopher *philo)
{
	if (philo->eaten == philo->table->rules->must_eat)
	{
		pthread_mutex_lock(&philo->eaten_lock);
		philo->full_flag = 1;
		pthread_mutex_unlock(&philo->eaten_lock);
		return (0);
	}
	lock_forks(philo);
	if (!check_if_alive(philo))
	{
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		return (0);
	}
	return (1);
}

int	perform_eating(t_philosopher *philo)
{
	int	eat_duration;

	if (!check_eating_status(philo))
		return (0);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->table->eat_lock);
	eat_duration = philo->table->rules->eat;
	pthread_mutex_unlock(&philo->table->eat_lock);
	pthread_mutex_lock(&philo->table->die_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->table->die_lock);
	wait_for_duration(eat_duration, philo);
	pthread_mutex_lock(&philo->eaten_lock);
	philo->eaten += 1;
	pthread_mutex_unlock(&philo->eaten_lock);
	unlock_forks(philo);
	return (1);
}

int	perform_sleeping(t_philosopher *philo)
{
	int	sleep_duration;

	if (philo->eaten == philo->table->rules->must_eat)
	{
		pthread_mutex_lock(&philo->eaten_lock);
		philo->full_flag = 1;
		pthread_mutex_unlock(&philo->eaten_lock);
		return (0);
	}
	if (!check_if_alive(philo))
		return (0);
	pthread_mutex_lock(&philo->table->sleep_lock);
	sleep_duration = philo->table->rules->sleep;
	pthread_mutex_unlock(&philo->table->sleep_lock);
	print_status(philo, "is sleeping");
	wait_for_duration(sleep_duration, philo);
	return (1);
}

void	*philosopher_routine(void *philo_void)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philo_void;
	while (!check_flag_status(philo->table))
	{
		if (!perform_eating(philo))
			return (NULL);
		if (!perform_sleeping(philo))
			return (NULL);
		print_status(philo, "is thinking");
	}
	return (NULL);
}
