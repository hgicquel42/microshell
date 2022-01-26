/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgicquel <hgicquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 13:09:34 by hgicquel          #+#    #+#             */
/*   Updated: 2022/01/26 19:28:13 by hgicquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_cmd
{
	int		argc;
	char	**argv;
	int		pipe[2];
	int		fdi;
	int		fdo;
}	t_cmd;

typedef struct s_state
{
	int		argc;
	char	**argv;
	char	**envp;
	int		cmdc;
	t_cmd	*cmdv;
}	t_state;

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_error(char *s)
{
	write(2, s, ft_strlen(s));
	return (0);
}

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
		if (r && !g->cmdv)
			o++;
		if (r && g->cmdv)
		{
			g->cmdv[o].argc = r;
			g->cmdv[o].argv = malloc(r * sizeof(char *));
			g->cmdv[o].pipe[0] = 0;
			g->cmdv[o].pipe[1] = 0;
			g->cmdv[o].fdi = 0;
			g->cmdv[o].fdo = 0;
			if (!g->cmdv[o].argv)
				return (o);
			if (ft_parse(g, i, o) != r)
				return (o);
			o++;
		}
		i += r;
	}
	return (o);
}

bool	ft_exec(t_state *g, int i)
{
	int	pid;
	int	j;

	if (g->cmdv[i].fdi)
		printf("-> ");
	j = 0;
	while (j < g->cmdv[i].argc)
	{
		printf("'%s' ", g->cmdv[i].argv[j]);
		j++;
	}
	if (g->cmdv[i].fdo)
		printf("-> ");
	printf("\n");
	pid = fork();
	if (pid == -1)
		return (-1);
	
}

bool	ft_run(t_state *g)
{
	int	i;

	i = 0;
	while (i < g->cmdc)
	{
		if (!strcmp(g->cmdv[i].argv[0], ";"))
			i++;
		else if (!strcmp(g->cmdv[i].argv[0], "|"))
			i++;
		else
		{
			if (i < g->cmdc - 1 && !strcmp(g->cmdv[i + 1].argv[0], "|"))
			{
				if (pipe(g->cmdv[i + 2].pipe) == -1)
					return (false);
				g->cmdv[i].fdo = g->cmdv[i + 2].pipe[0];
				g->cmdv[i + 2].fdi = g->cmdv[i + 2].pipe[0];
			}
			if (!ft_exec(g, i))
				return (false);
			i++;
		}
	}
	return (true);
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
		return (1 + ft_error("error: fatal\n"));
	if (ft_split(&g) != g.cmdc)
		return (1 + ft_error("error: fatal\n"));
	if (!ft_run(&g))
		return (1 + ft_error("error: fatal\n"));
	return (0);
}
