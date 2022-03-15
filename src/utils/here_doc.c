#include <minishell.h>

char	*get_name(int i) {
   	char	*num;
   	char	*name;
	char	*tmp;

	num = ft_itoa(i);
	tmp = ft_strjoin(num, ".tmp");
	name = ft_strjoin(".", tmp);
	free(num);
	free(tmp);
	return (name);
}

char	*ft_open_tmp()
{
	int		i;
	int		fd;
	char	*name;

	i = 0;
	while (i++ < 1000)
	{
		name = get_name(i);
		if (access(name, F_OK) == 0)
		{
			free(name);
			continue ;
		}
		fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd != -1)
			break ;
		else
		{
			free(name);
			return (NULL);
		}
	}
	return (name);
}

void	ft_read_input(t_ast *node, char *name)
{
	char	*line;
	int		fd;

	line = NULL;
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (line != NULL)
	{
		line = readline("> ");
		if (ft_strlen(line) == ft_strlen(node->data) 
				&& ft_strncmp(line, node->data, ft_strlen(line)) == 0)
		{
			close(fd);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	free(line);
}

int	ft_write_here_doc(t_ast *node)
{
	int		pid;
	int		status;
	char	*name;

	name = ft_open_tmp();
	pid = fork();
	if (pid == 0)
	{
		sig_here_doc();
		ft_read_input(node, name);
		exit(EXIT_SUCCESS);
	}
	else
	{
		sig_ignore();
		waitpid(pid, &status, 0);
		node->data = name;
		if (WIFSIGNALED(status) && WTERMSIG(status) + 128 == 130)
		{
			write(1, "\n", 1);
			return (1);
		}
		my_signal();
	}
	return (0);
}

int	ft_process_here_doc(t_ast **tree)
{
	t_ast	*curr;

	if ((*tree)->type == T_PIPE_NODE)
	{
		ft_process_here_doc(&((*tree)->left));
		ft_process_here_doc(&((*tree)->right));
	}
	else
	{
		curr = (*tree)->right->left;
		while (curr)
		{
			if (curr->type == T_DOUBLE_IN_NODE)
			{
				if (ft_write_here_doc(curr))
					return (1);
			}
			curr = curr->left;
		}
	}
	return (0);
}
