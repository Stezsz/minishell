/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: strodrig <strodrig@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:48:54 by tborges-          #+#    #+#             */
/*   Updated: 2025/01/30 18:22:33 by strodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int val = execve("/bin/ls", argv, NULL);
	if (val == -1)
	{
		perror("Error");
	}
	return (0);

}

//Os comandos basicos do linux, como cd, pwd e ls são importados direto do char **env da main
