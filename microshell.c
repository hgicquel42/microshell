/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgicquel <hgicquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 13:09:34 by hgicquel          #+#    #+#             */
/*   Updated: 2022/01/26 17:51:03 by hgicquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_cmd
{
	int		argc;
	char	**argv;
}	t_cmd;

typedef struct s_state
{
	int		argc;
	char	**argv;
	char	**envp;
	int		cmdc;
	t_cmd	*cmdv;
}	t_state;

void	ft_copy(t_state *g, int i, int o, int r)
{
	if (g->cmdv && g->cmdv[o].argv)
		g->cmdv[o].argv[r] = g->argv[i];
}

int	ft_parse(t_state *g, int i, int o)
{
	int	r;

	r = 0;
	while (i < g->argc)
	{
		if (!strcmp(g->argv[i], ";") || !strcmp(g->argv[i], "|"))
		{
			if (!r)
				ft_copy(g, i++, o, r++);
			break ;
		}
		ft_copy(g, i++, o, r++);
	}
	return (r);
}

int	ft_split(t_state *g)
{
	int		i;
	int		o;
	int		r;

	i = 0;
	o = 0;
	while (i < g->argc)
	{
		r = ft_parse(g, i, o);
		if (g->cmdv)
		{
			g->cmdv[o].argc = r;
			g->cmdv[o].argv = malloc(r * sizeof(char *));
			if (!g->cmdv[o].argv)
				return (o);
			if (ft_parse(g, i, o) != r)
				return (o);
		}
		i += r;
		o++;
	}
	return (o);
}

void	ft_show(t_state *g)
{
	int	i;
	int	j;

	i = 0;
	while (i < g->cmdc)
	{
		j = 0;
		while (j < g->cmdv[i].argc)
		{
			printf("'%s' ", g->cmdv[i].argv[j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void	ft_run(t_state *g)
{
	
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_putstr(char *s)
{
	
}

int	ft_error()
{
	
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_state	g;

	g.argc = argc - 1;
	g.argv = argv + 1;
	g.envp = envp;
	g.cmdv = NULL;
	g.cmdc = ft_split(&g);
	g.cmdv = malloc(g.cmdc * sizeof(t_cmd));
	if (!g.cmdv)
		return (1);
	if (ft_split(&g) != g.cmdc)
		return (1);
	ft_show(&g);
	return (0);
}
