#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_cmd
{
	char	**args;
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
	int		sts;
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
	return (1);
}

void	ft_fatal(void)
{
	exit(ft_error("error: fatal\n"));
}

void	ft_copy(t_cmd *cmd, int r, char *s)
{
	if (cmd)
		cmd->args[r] = s;
}

int	ft_parse(t_cmd *cmd, t_state *g, int i)
{
	int	r;

	r = 0;
	while (i < g->argc)
	{
		if (!strcmp(g->argv[i], ";") || !strcmp(g->argv[i], "|"))
		{
			if (!r)
				ft_copy(cmd, r++, g->argv[i++]);
			break ;
		}
		ft_copy(cmd, r++, g->argv[i++]);
	}
	if (cmd)
		cmd->args[r] = NULL;
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
		r = ft_parse(NULL, g, i);
		if (r && !g->cmdv)
			o++;
		if (r && g->cmdv)
		{
			g->cmdv[o].args = malloc((r + 1) * sizeof(char *));
			if (!g->cmdv[o].args)
				return (o);
			g->cmdv[o].pipe[0] = 0;
			g->cmdv[o].pipe[1] = 0;
			g->cmdv[o].fdi = 0;
			g->cmdv[o].fdo = 0;
			if (ft_parse(&g->cmdv[o], g, i) != r)
				return (o);
			o++;
		}
		i += r;
	}
	return (o);
}

int	ft_exec(t_state *g, t_cmd *cmd)
{
	int		pid;
	int		sts;

	sts = 0;
	pid = fork();
	if (pid == 0)
	{
		if (cmd->fdi)
			dup2(cmd->fdi, 0);
		if (cmd->fdo)
			dup2(cmd->fdo, 1);
		execve(cmd->args[0], cmd->args, g->envp);
	}
	if (cmd->fdi)
		close(cmd->fdi);
	if (cmd->fdo)
		close(cmd->fdo);
	if (pid > 0)
		waitpid(pid, &sts, 0);
	if (WIFEXITED(sts))
		g->sts = WEXITSTATUS(sts);
	return (pid);
}

bool	ft_run(t_state *g)
{
	int	i;
	int	r;

	i = 0;
	while (i < g->cmdc)
	{
		if (!strcmp(g->cmdv[i].args[0], ";"))
			;
		else if (!strcmp(g->cmdv[i].args[0], "|"))
			;
		else if (!strcmp(g->cmdv[i].args[0], "cd"))
		{
			if (!g->cmdv[i].args[1])
				g->sts = ft_error("error: cd: bad arguments\n");
			if (chdir(g->cmdv[i].args[1]) == -1)
			{
				ft_error("error: cd: cannot change directory to ");
				ft_error(g->cmdv[i].args[1]);
				ft_error("\n");
				exit(1);
			}
		}
		else
		{
			if (i < g->cmdc - 1 && !strcmp(g->cmdv[i + 1].args[0], "|"))
			{
				if (pipe(g->cmdv[i + 2].pipe) == -1)
					ft_fatal();
				g->cmdv[i].fdo = g->cmdv[i + 2].pipe[1];
				g->cmdv[i + 2].fdi = g->cmdv[i + 2].pipe[0];
			}
			r = ft_exec(g, &g->cmdv[i]);
			if (r == -1)
				ft_fatal();
			if (r == 0)
			{
				ft_error("error: cannot execute ");
				ft_error(g->cmdv[i].args[0]);
				ft_error("\n");
				exit(1);
			}
		}
		free(g->cmdv[i++].args);
	}
	free(g->cmdv);
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
		ft_fatal();
	if (ft_split(&g) != g.cmdc)
		ft_fatal();
	ft_run(&g);
	return (g.sts);
}
