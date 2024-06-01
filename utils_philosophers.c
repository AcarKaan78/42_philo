/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philosophers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysenazsenel <aysenazsenel@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:56:24 by ilsahin           #+#    #+#             */
/*   Updated: 2024/05/16 00:02:57 by aysenazsene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	initialize_mutexes(t_table *table)
{
	int	i;

	table->mutexes = malloc(sizeof(pthread_mutex_t) * table->rules->n_p);
	if (!table->mutexes)
		return (2);
	i = -1;
	while (++i < table->rules->n_p)
		pthread_mutex_init(&(table->mutexes[i]), NULL);
	pthread_mutex_init(&table->eat_lock, NULL);
	pthread_mutex_init(&table->sleep_lock, NULL);
	pthread_mutex_init(&table->die_lock, NULL);
	pthread_mutex_init(&table->flag_lock, NULL);
	pthread_mutex_init(&table->time_mutex, NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
	return (0);
}

void	assign_philosophers(t_table *table, int i)
{
	table->philos[i]->table = table;
	table->philos[i]->last_meal = get_time();
	table->philos[i]->philo_id = i + 1;
	table->philos[i]->eaten = 0;
	table->philos[i]->philo = table->threads[i];
	pthread_mutex_init(&table->philos[i]->eaten_lock, NULL);
	if (i == 0)
	{
		table->philos[0]->right = &table->mutexes[0];
		table->philos[0]->left = &table->mutexes[table->rules->n_p - 1];
	}
	else
	{
		table->philos[i]->right = &table->mutexes[i];
		table->philos[i]->left = &table->mutexes[i - 1];
	}
}

void	initialize_philosophers(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->rules->n_p)
		assign_philosophers(table, i);
	table->time = get_current_time();
	i = -1;
	while (++i < table->rules->n_p)
	{
		pthread_create(&table->philos[i]->philo,
			NULL, routine, table->philos[i]);
		usleep(60);
	}
	while (1)
	{
		if (monitor_philosophers(table))
			break ;
	}
	i = -1;
	while (++i < table->rules->n_p)
		pthread_mutex_destroy(&(table->mutexes[i]));
	i = -1;
	while (++i < table->rules->n_p)
		pthread_join(table->philos[i]->philo, NULL);
}

int	start_simulation(t_table *table)
{
	int	i;

	if (initialize_mutexes(table))
		return (2);
	table->philos = malloc(sizeof(t_philosopher *) * table->rules->n_p);
	table->threads = malloc(sizeof(pthread_t) * table->rules->n_p);
	i = -1;
	while (++i < table->rules->n_p)
	{
		table->philos[i] = malloc(sizeof(t_philosopher));
		if (!table->philos[i])
			return (2);
	}
	if (!table->philos || !table->threads)
		return (2);
	table->dead_flag = 0;
	table->full_flag = 0;
	initialize_philosophers(table);
	return (0);
}
