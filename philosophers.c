/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysenazsenel <aysenazsenel@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 14:55:42 by ilsahin           #+#    #+#             */
/*   Updated: 2024/05/15 23:50:48 by aysenazsene      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	validate_arguments(int arg_count, char **args)
{
	int	i;
	int	status;

	status = 1;
	if (arg_count != 5 && arg_count != 6)
		status *= 2;
	i = -1;
	while (args[++i])
	{
		if (!is_string_digit(args[i]))
			status *= 3;
	}
	if (status != 1)
	{
		if (status % 2 == 0)
			printf("Error: Wrong argument count!\n");
		if (status % 3 == 0)
			printf("Error: All arguments must be valid integer!\n");
		return (0);
	}
	return (1);
}

int	*validate_values(int arg_count, char **args)
{
	int	i;
	int	*values;

	values = malloc(sizeof(int) * (arg_count));
	if (!values)
		return (NULL);
	i = -1;
	while (++i < arg_count - 1)
	{
		values[i] = ft_atoi(args[i]);
		if (values[i] <= 0)
		{
			printf("Error: Arguments must be positive integer!\n");
			free(values);
			return (NULL);
		}
	}
	values[i] = 0;
	return (values);
}

void	initialize_rules(int arg_count, t_rules **rules, int *values)
{
	(*rules)->argument_count = arg_count - 1;
	(*rules)->n_p = values[0];
	(*rules)->die = values[1];
	(*rules)->eat = values[2];
	(*rules)->sleep = values[3];
	if (arg_count == 6)
		(*rules)->must_eat = values[4];
	else
		(*rules)->must_eat = -1;
}

int	main(int arg_count, char **args)
{
	char	**arguments;
	int		*values;
	t_rules	*rules;
	t_table	*table;

	arguments = &args[1];
	if (!validate_arguments(arg_count, arguments))
		return (1);
	values = validate_values(arg_count, arguments);
	if (!values)
		return (1);
	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (2);
	initialize_rules(arg_count, &rules, values);
	free(values);
	table = malloc(sizeof(t_table));
	if (!table)
		return (2);
	table->rules = rules;
	return (kick_starter(table));
	return (0);
}
