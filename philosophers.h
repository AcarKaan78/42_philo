/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysenazsenel <aysenazsenel@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:55:54 by ilsahin           #+#    #+#             */
/*   Updated: 2024/05/16 00:03:51 by aysenazsene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_rules
{
	int	n_p;
	int	die;
	int	eat;
	int	sleep;
	int	must_eat;
	int	argument_count;
}	t_rules;

typedef struct s_philosopher
{
	int				philo_id;
	int				eaten;
	pthread_t		philo;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	pthread_mutex_t	eaten_lock;
	struct s_table	*table;
	long long		last_meal;
	int				philo_dead;
	int				full_flag;
}	t_philosopher;

typedef struct s_table
{
	t_rules			*rules;
	t_philosopher	**philos;
	pthread_t		*threads;
	pthread_mutex_t	*mutexes;
	long long		time;
	int				dead_flag;
	int				full_flag;
	pthread_mutex_t	eat_lock;
	pthread_mutex_t	sleep_lock;
	pthread_mutex_t	die_lock;
	pthread_mutex_t	flag_lock;
	pthread_mutex_t	time_mutex;
	pthread_mutex_t	print_mutex;
}	t_table;

int			is_string_numeric(char *str);
int			string_to_int(const char *str);
int			start_simulation(t_table *table);
long long	get_current_time(void);
void		wait_for_duration(int duration, t_philosopher *philo);
int			initialize_mutexes(t_table *table);
void		initialize_philosophers(t_table *table);
void		*philosopher_routine(void *philo_void);
int			perform_eating(t_philosopher *philo);
int			perform_sleeping(t_philosopher *philo);
int			print_status(t_philosopher *philo, char *status);
int			check_flags(t_table *table);
int			update_flags(t_table *table, int flag);
int			check_if_alive(t_philosopher *philo);
int			monitor_philosophers(t_table *table);
void		lock_forks(t_philosopher *philo);
void		unlock_forks(t_philosopher *philo);

#endif
