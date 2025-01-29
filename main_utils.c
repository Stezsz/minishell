/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:18:07 by strodrig          #+#    #+#             */
/*   Updated: 2025/01/27 13:18:07 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_prompt(void)
{

}

/* Encontra o caminho do executável baseado no PATH */
char	*find_executable_path(char *command, char **env)
{
	char	*path_env = NULL;
	char	**paths;
	char	*full_path;
	int		i;

	int j = 0;
	while (env[j])
	{
		if (strncmp(env[j], "PATH=", 5) == 0)
		{
			path_env = env[j] + 5;
			break;
		}
		j++;
	}
	if (!path_env)
		return (NULL);

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = malloc(PATH_MAX);
		if (!full_path)
			return (NULL);
		snprintf(full_path, PATH_MAX, "%s/%s", paths[i], command);
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free(paths);
	return (NULL);
}

/* Trata comandos built-in, retorna 1 se executado, 0 caso contrário */
int	handle_builtin_commands(char **args, char **env)
{
	(void)env;

	if (!args[0])
		return (0);
	if (strcmp(args[0], "cd") == 0)
	{
		char *home = getenv("HOME");
		if (!args[1])
		{
			if (home)
				chdir(home);
			else
				printf("minishell: cd: HOME not set\n");
		}
		else if (chdir(args[1]) != 0)
			perror("minishell: cd");
		return (1);
	}
	return (0);
}


/* Executa um comando externo */
void	execute_command(char *input, char **env)
{
	pid_t	pid;
	char	*full_path;
	char	**args;
	int		i;

	args = ft_split(input, ' ');
	if (!args || !args[0])
	{
		free(args);
		return;
	}

	if (handle_builtin_commands(args, env))
	{
		free(args);
		return;
	}

	full_path = find_executable_path(args[0], env);
	pid = fork();
	if (pid == 0)
	{
		if (full_path)
		{
			execve(full_path, args, env);
			free(full_path);
		}
		else
			printf("minishell: command not found: %s\n", args[0]);
		exit(127);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);

	free(full_path);
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
