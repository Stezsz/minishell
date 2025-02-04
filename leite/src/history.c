/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tborges- <tborges-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 18:02:40 by tborges-          #+#    #+#             */
/*   Updated: 2025/02/03 18:03:53 by tborges-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

#define HISTORY_FILE ".history"

// Função para guardar o comando no histórico
void add_to_history(const char *command) {
    FILE *history_file = fopen(HISTORY_FILE, "a");
    if (history_file == NULL) {
        perror("Erro ao abrir arquivo de histórico");
        return;
    }
    fprintf(history_file, "%s\n", command);
    fclose(history_file);
}

////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100

// Função para carregar o histórico
void load_history(char history[MAX_HISTORY][256], int *history_count) {
    FILE *history_file = fopen(HISTORY_FILE, "r");
    if (history_file == NULL) {
        perror("Erro ao abrir arquivo de histórico");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), history_file) && *history_count < MAX_HISTORY) {
        line[strcspn(line, "\n")] = '\0'; // Remover o '\n' do final
        strcpy(history[*history_count], line);
        (*history_count)++;
    }
    fclose(history_file);
}

////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

void show_history(char history[MAX_HISTORY][256], int history_count) {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}
