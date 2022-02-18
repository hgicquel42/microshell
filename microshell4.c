#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

int	ft_split(int argc, char **argv, t_cmd *cmdp)
{
	int	i;
	int	j;
	int	l;

	i = 1;
	l = 0;
	while (i < argc)
	{
		j = ft_parse(argc, argv, NULL, i);
		if (cmdp)
		{
			cmdp[l].pipe[0] = 0;
			cmdp[l].pipe[1] = 0;
			cmdp[l].fdi = 0;
			cmdp[l].fdo = 0;
			cmdp[l].argc = j;
			cmdp[l].argp = malloc((j + 1) * sizeof(char *));
			if (!cmdp[l].argp)
				ft_fatal();
			if (ft_parse(argc, argv, &cmdp[l], i) != j)
				ft_fatal();
		}
		i += j;
		l++;
	}
	return (l);
}

void	ft_exec(t_cmd *cmd, char **envp)
{
	int	pid;
	int	sts;

	sts = 0;
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

void	ft_exec_all(int cmdc, t_cmd *cmdp, char **envp)
{
	int	i;

	i = 0;
	while (i < cmdc)
	{
		if (!strcmp(cmdp[i].argp[0], ";") || !strcmp(cmdp[i].argp[0], "|"))
			;
		else if (!strcmp(cmdp[i].argp[0], "cd"))
		{
			if (cmdp[i].argc != 2)
				ft_error("error: cd: bad arguments\n");
			else if (chdir(cmdp[i].argp[1]) == -1)
			{
				ft_error("error: cd: cannot change directory to ");
				ft_error(cmdp[i].argp[1]);
				ft_error("\n");
			}
		}
		else
		{
			if (i < cmdc - 1 && !strcmp(cmdp[i + 1].argp[0], "|"))
			{
				if (pipe(cmdp[i].pipe) == -1)
					ft_fatal();
				cmdp[i + 2].fdi = cmdp[i].pipe[0];
				cmdp[i].fdo = cmdp[i].pipe[1];
			}
			ft_exec(&cmdp[i], envp);
		}
		i++;
	}
}

void	ft_print(int cmdc, t_cmd *cmdp)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmdc)
	{
		j = 0;
		while (cmdp[i].argp[j])
			printf("%s ", cmdp[i].argp[j++]);
		printf("\n");
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		cmdc;
	t_cmd	*cmdp;

	cmdc = ft_split(argc, argv, NULL);
	cmdp = malloc(cmdc * sizeof(t_cmd));
	if (!cmdp || ft_split(argc, argv, cmdp) != cmdc)
		ft_fatal();
	ft_exec_all(cmdc, cmdp, envp);
	return (0);
}
