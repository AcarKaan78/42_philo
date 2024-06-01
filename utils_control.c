/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysenazsenel <aysenazsenel@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:56:07 by ilsahin           #+#    #+#             */
/*   Updated: 2024/05/15 23:59:27 by aysenazsene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_flags(t_table *table)
{
	pthread_mutex_lock(&table->flag_lock);
	if (table->dead_flag || table->full_flag)
	{
		pthread_mutex_unlock(&table->flag_lock);
		return (1);
	}
	pthread_mutex_unlock(&table->flag_lock);
	return (0);
}

int	update_flags(t_table *table, int flag)
{
	pthread_mutex_lock(&table->flag_lock);
	if (flag == 0)
		table->full_flag = 1;
	else
		table->dead_flag = flag;
	pthread_mutex_unlock(&table->flag_lock);
	return (1);
}

int	check_if_alive(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->table->die_lock);
	if (philo->last_meal + philo->table->rules->die < get_time())
	{
		pthread_mutex_unlock(&philo->table->die_lock);
		return (!update_flags(philo->table, philo->philo_id));
	}
	pthread_mutex_unlock(&philo->table->die_lock);
	return (1);
}

int	monitor_philosophers(t_table *table)
{
	int	i;
	int	full;

	full = 0;
	i = -1;
	while (++i < table->rules->n_p)
	{
		if (!check_if_alive(table->philos[i]))
		{
			print_status(table->philos[i], "died");
			if (table->rules->n_p == 1)
				pthread_mutex_unlock(&table->mutexes[0]);
			return (1);
		}
		pthread_mutex_lock(&table->philos[i]->eaten_lock);
		if (table->philos[i]->full_flag == 1)
			full++;
		pthread_mutex_unlock(&table->philos[i]->eaten_lock);
		if (full == table->rules->n_p)
		{
			update_flags(table, 0);
			return (1);
		}
	}
	return (0);
}
