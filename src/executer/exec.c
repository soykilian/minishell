#include <minishell.h>

char	*ft_strjoin_path(char *path, char *cmd)
{
	char	*sol;
	char	*tmp;

	tmp = ft_strjoin("/", cmd);
	sol = ft_strjoin(path, tmp);
	free(tmp);
	return (sol);
}

char	*ft_getpath(char **envp, char *cmd)
{
	char	**path;
	char	**tmp;
	char	*command;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	while (*envp && ft_strncmp("PATH", *envp, 4))
		envp++;
	if (!*envp)
		return ("");
	tmp = ft_split(*envp, '=');
	path = ft_split(tmp[1], ':');
	ft_free_split(tmp);
	tmp = path;
	while (*path)
	{
		command = ft_strjoin_path(*path, cmd);
		if (access(command, F_OK) == 0)
			return (command);
		free(command);
		path++;
	}
	ft_free_split(tmp);
	return ("");
}

/*char	*ft_getpath(char **envp, char *cmd)
{
	int		i;
	char	**dirs;
	char	*slash;

	i = 0;
	if (!access(cmd, F_OK))
		return (cmd);
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH", 4) == 0)
			break ;
		envp++;
	}
	if (!*envp)
		return (cmd);
	*envp += 5;
	dirs = ft_split(*envp, ':');
	slash = ft_strjoin("/", cmd);
	i = 0;
	while (dirs[i] && access(ft_strjoin(dirs[i], slash), F_OK))
		i++;
	if (!dirs[i])
		return ("");
	return (ft_strjoin(dirs[i], slash));
}*/

char	**ft_return_cmd(t_ast *node, char *cmd)
{
	char	**sol;
	int		i;

	sol = ft_calloc(sizeof(char *), ft_astsize_r(node) + 2);
	sol[0] = cmd;
	i = 1;
	while (node)
	{
		sol[i] = node->data;
		i++;
		node = node->right;
	}
	sol[i] = NULL;
	return (sol);
}

void	ft_exec_command(t_ast *node)
{
	char	**cmd;
	char	**environ;

	if (!node)
		return ;
	environ = ft_envmatrix();
	cmd = ft_return_cmd(node->left, node->data);
	execve(ft_getpath(environ, cmd[0]), cmd, environ);
	perror(cmd[0]);
	//ft_free_split(environ);
	ft_free_split(cmd);
	exit(1);
}

int	ft_getredir(t_ast *tree)
{
	t_ast	**cpy;
	int		fd;

	fd = 1;
	if (!tree)
		return (0);
	cpy = ft_calloc(1, sizeof(void *));
	*cpy = tree;
	while (*cpy)
	{
		fd = open((*cpy)->data, O_CREAT | O_RDWR | O_TRUNC, 0644);
		(*cpy) = (*cpy)->right;
	}
	return (fd);
}

void	ft_exec_tree(t_ast *tree, int pip)
{
	int	fdesc;
	int	pid;
	int	fd[2];

	if (tree->type == T_PIPE_NODE)
	{
		printf("EJECUTANDO PIPE\n");
		ft_exec_tree(tree->left, 1);
		printf("first: %s\n", tree->left->data);
		ft_exec_tree(tree->right, 0);
		printf("second: %s\n", tree->right->data);
	}
	if (tree->type == T_COMMAND_NODE)
	{
		pid = fork();
		pipe(fd);
		if (pid == 0)
		{
			close(fd[READ_END]);
			if (pip)
				dup2(fd[WRITE_END], 1);
			close(fd[WRITE_END]);
			fdesc = ft_getredir(tree->right->right);
			if (fdesc)
				dup2(fdesc, 1);
			if (ft_strnstr("envpwdechoexitunsetexport", tree->data,
						25) != NULL && valid_builtins(tree))
			{
				ft_use_builtins(tree);
				exit(0);
			}
			else
				ft_exec_command(tree);
		}
		else
		{
			close(fd[WRITE_END]);
			if (pip)
				dup2(fd[READ_END], 0);
			close(fd[READ_END]);
			waitpid(pid, NULL, 0);
		}
	}
}
