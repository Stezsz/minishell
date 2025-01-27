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

char *find_executable_path(char *command, char **env) {
	char *path_env = NULL;
	char full_path[PATH_MAX];

	// Obtém a variável de ambiente PATH
	for (int i = 0; env[i] != NULL; i++) {
		if (strncmp(env[i], "PATH=", 5) == 0) {
			path_env = env[i] + 5;  // Ignora o "PATH="
			break;
		}
	}
	if (!path_env) {
		return NULL;  // PATH não encontrado
	}
	// Divide o PATH em diretórios
	char *dir = strtok(path_env, ":");
	while (dir != NULL) {
		// Constrói o caminho completo (diretório + / + comando)
		strncpy(full_path, dir, PATH_MAX);
		strcat(full_path, "/");
		strcat(full_path, command);
		// Verifica se o arquivo existe e é executável
		if (access(full_path, X_OK) == 0) {
			return strdup(full_path);  // Retorna o caminho completo
		}
		dir = strtok(NULL, ":");
	}

	return NULL;  // Comando não encontrado
}

void execute_command(char *input, char **env) {
	pid_t pid;
	pid = fork();

	if (pid == 0) {
		// Processo filho
		char *args[] = {input, NULL};
		char *full_path = find_executable_path(input, env);

		if (full_path) {
			execve(full_path, args, env);  // Usa o caminho completo
			free(full_path);
		} else {
			printf("minishell: comando não encontrado: %s\n", input);
		}

		exit(1);
	} else if (pid > 0) {
		// Processo pai
		waitpid(pid, NULL, 0);
	} else {
		perror("fork");  // Se fork falhar
	}
}
