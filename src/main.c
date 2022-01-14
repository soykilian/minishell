/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsanchez <dsanchez@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 16:14:18 by dsanchez          #+#    #+#             */
/*   Updated: 2022/01/14 19:42:31 by dsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <readline/readline.h>

extern char	**environ;

char	*ft_get_path(char *arg)
{
	char	*env;
	char	**path;
	char	*command;

	if (access(arg, F_OK) == 0)
		return (arg);
	env = getenv("PATH");
	if (!env)
		return ("");
	path = ft_split(env, ':');
	while (*path)
	{
		command = ft_strjoin(*path, ft_strjoin("/", arg));
		if (access(command, F_OK) == 0)
			return (command);
		path++;
	}
	return ("");
}

int	ft_execute(char **tokens)
{
	int			pid;

	pid = fork();
	if (pid == 0)
	{
		execve(ft_get_path(tokens[0]), tokens, environ);
		perror(tokens[0]);
		exit(0);
	}
	else if (pid < 0)
	{
		perror("minish");
		return (0);
	}
	else
	{
		waitpid(pid, NULL, 0);
		return (1);
	}
}

/*char	**ft_get_tokens(char *line)
{
	char	**tokens;

	tokens = ft_split(line,  ' ');
	return (tokens);
}*/

void    ft_print_tokens(char **tokens)
{
	if (!tokens)
		return ;
    while (*tokens)
    {
        printf("%s\n", *tokens);
        tokens++;
    }
}

int	main()
{
	char		**tokens;
	t_pstatus	status;
	char		*pwd;
	int			state;

	state = 1;
	while (state)
	{
		pwd = getenv("PWD");
		printf("\033[0;32m[%s]\033[0;33m", pwd);
		status.data = readline("$ \033[0;37m");
		status.curr = 0;
		tokens = ft_get_tokens(&status);
        ft_print_tokens(tokens);
		//ft_execute(tokens);
	}
	return (0);
}
