#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef struct s_cmd
{
	int		argc;
	char	**argp;
	int		pipe[2];
	int		fdi;
	int		fdo;
}	t_cmd;

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_error(char *s)
{
	write(2, s, ft_strlen(s));
}

void	ft_fatal(void)
{
	ft_error("error: fatal\n");
	exit(1);
}

void	ft_copy(t_cmd *cmd, int j, char *s)
{
	if (cmd)
		cmd->argp[j] = s;
}

int	ft_parse(int argc, char **argv, t_cmd *cmd, int i)
{
	int	j;

	j = 0;
	while (i < argc)
	{
		if (!strcmp(argv[i], ";") || !strcmp(argv[i], "|"))
		{
			if (!j)
				ft_copy(cmd, j++, argv[i++]);
			break ;
		}
		ft_copy(cmd, j++, argv[i++]);
	}
	ft_copy(cmd, j, NULL);
	return (j);
}

int	ft_split(int argc, char **argv, t_cmd *cmdv)
{
	int	i;
	int	j;
	int	l;

	i = 1;
	l = 0;
	while (i < argc)
	{
		j = ft_parse(argc, argv, NULL, i);
		if (cmdv)
		{
			cmdv[l].pipe[0] = 0;
			cmdv[l].pipe[1] = 0;
			cmdv[l].fdi = 0;
			cmdv[l].fdo = 0;
			cmdv[l].argc = j;
			cmdv[l].argp = malloc((j + 1) * sizeof(char *));
			if (!cmdv[l].argp)
				ft_fatal();
			if (ft_parse(argc, argv, &cmdv[l], i) != j)
				ft_fatal();
		}
		l++;
		i += j;
	}
	return (l);
}

void ft_exec(t_cmd *cmd, char **envp)
{
	int	sts;
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		if (cmd->fdi)
			dup2(cmd->fdi, 0);
		if (cmd->fdo)
			dup2(cmd->fdo, 1);
		execve(cmd->argp[0], cmd->argp, envp);
	}
	if (cmd->fdi)
		close(cmd->fdi);
	if (cmd->fdo)
		close(cmd->fdo);
	if (pid == -1)
		ft_fatal();
	if (pid == 0)
	{
		ft_error("error: cannot execute ");
		ft_error(cmd->argp[0]);
		ft_error("\n");
		exit(1);
	}
	waitpid(pid, &sts, 0);
}

void	ft_exec_all(int cmdc, t_cmd *cmdv, char **envp)
{
	int	i;

	i = 0;
	while (i < cmdc)
	{
		if (!strcmp(cmdv[i].argp[0], ";") || !strcmp(cmdv[i].argp[0], "|"))
			;
		else if (!strcmp(cmdv[i].argp[0], "cd"))
		{
			if (cmdv[i].argc != 2)
				ft_error("error: cd: bad arguments\n");
			else if (chdir(cmdv[i].argp[1]) == -1)
			{
				ft_error("error: cd: cannot change directory to ");
				ft_error(cmdv[i].argp[1]);
				ft_error("\n");
			}
		}
		else
		{
			if (i < cmdc - 1 && !strcmp(cmdv[i + 1].argp[0], "|"))
			{
				if (pipe(cmdv[i].pipe) == -1)
					ft_fatal();
				cmdv[i + 2].fdi = cmdv[i].pipe[0];
				cmdv[i].fdo = cmdv[i].pipe[1];
			}
			ft_exec(&cmdv[i], envp);
		}
		i++;
	}
}

void	ft_print(int cmdc, t_cmd *cmdv)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmdc)
	{
		j = 0;
		while (j < cmdv[i].argc)
			printf("%s ", cmdv[i].argp[j++]);
		printf("\n");
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		cmdc;
	t_cmd	*cmdv;

	cmdc = ft_split(argc, argv, NULL);
	cmdv = malloc(argc * sizeof(t_cmd));
	if (!cmdv)
		ft_fatal();
	if (ft_split(argc, argv, cmdv) != cmdc)
		ft_fatal();
	ft_exec_all(cmdc, cmdv, envp);
	return (0);
}

