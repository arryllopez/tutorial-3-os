/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U: Operating Systems
 *
 * Copyright (C) 2026, <GROUP NUMBER 2>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

#define BUFFER_LEN 256
#define NUM_PLAYERS 4

// Convert string to lowercase in-place
static void str_to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

// Returns true when every question in the game has been answered
static bool all_questions_answered(void) {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (!questions[i].answered) return false;
    }
    return true;
}

// Processes the answer from the user containing "what is" or "who is"
// and tokenizes it; tokens is populated with pointers into the input string.
void tokenize(char *input, char **tokens) {
    int i = 0;
    char *token = strtok(input, " \t\n\r");
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\n\r");
    }
    tokens[i] = NULL;
}

// Displays the game results for each player, ranked from first to last place
void show_results(player *players, int num_players) {
    // Sort a local copy by score descending (bubble sort)
    player sorted[NUM_PLAYERS];
    for (int i = 0; i < num_players; i++) sorted[i] = players[i];

    for (int i = 0; i < num_players - 1; i++) {
        for (int j = 0; j < num_players - 1 - i; j++) {
            if (sorted[j].score < sorted[j + 1].score) {
                player tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    printf("\n========================================\n");
    printf("           FINAL RESULTS\n");
    printf("========================================\n");
    for (int i = 0; i < num_players; i++) {
        printf("  %d. %-20s $%d\n", i + 1, sorted[i].name, sorted[i].score);
    }
    printf("========================================\n");
    printf("  Winner: %s with $%d!\n", sorted[0].name, sorted[0].score);
    printf("========================================\n\n");
}

int main(int argc, char *argv[])
{
    player players[NUM_PLAYERS];
    char buffer[BUFFER_LEN] = { 0 };
    char buffer_copy[BUFFER_LEN] = { 0 };

    // Display intro and initialize questions
    printf("========================================\n");
    printf("       Welcome to Jeopardy!\n");
    printf("========================================\n\n");
    initialize_game();

    // Prompt for player names
    printf("Enter the names of the %d players:\n", NUM_PLAYERS);
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("  Player %d name: ", i + 1);
        if (fgets(buffer, BUFFER_LEN, stdin) == NULL) return EXIT_FAILURE;
        buffer[strcspn(buffer, "\r\n")] = '\0';
        strncpy(players[i].name, buffer, MAX_LEN - 1);
        players[i].name[MAX_LEN - 1] = '\0';
        players[i].score = 0;
    }

    printf("\nPlayers registered! Let the game begin!\n");

    // Main game loop — continues until all questions are answered
    while (!all_questions_answered()) {
        display_categories();

        // --- Select which player is answering ---
        char current_player[MAX_LEN];
        do {
            printf("Enter the name of the player selecting a question: ");
            if (fgets(buffer, BUFFER_LEN, stdin) == NULL) goto game_over;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(current_player, buffer, MAX_LEN - 1);
            current_player[MAX_LEN - 1] = '\0';
            if (!player_exists(players, NUM_PLAYERS, current_player)) {
                printf("  Player \"%s\" not found. Please try again.\n", current_player);
            }
        } while (!player_exists(players, NUM_PLAYERS, current_player));

        // --- Select a category and dollar value ---
        char category[MAX_LEN];
        int value = 0;
        bool valid_selection = false;

        do {
            printf("Enter category (programming / algorithms / databases): ");
            if (fgets(buffer, BUFFER_LEN, stdin) == NULL) goto game_over;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(category, buffer, MAX_LEN - 1);
            category[MAX_LEN - 1] = '\0';
            str_to_lower(category);

            printf("Enter dollar amount (100 / 200 / 300 / 400): ");
            if (fgets(buffer, BUFFER_LEN, stdin) == NULL) goto game_over;
            value = atoi(buffer);

            // Validate category
            bool cat_ok = false;
            for (int i = 0; i < NUM_CATEGORIES; i++) {
                if (strcmp(categories[i], category) == 0) { cat_ok = true; break; }
            }

            // Validate dollar amount
            bool val_ok = (value == 100 || value == 200 || value == 300 || value == 400);

            if (!cat_ok) {
                printf("  Invalid category \"%s\". Please try again.\n", category);
            } else if (!val_ok) {
                printf("  Invalid dollar amount. Please enter 100, 200, 300, or 400.\n");
            } else if (already_answered(category, value)) {
                printf("  That question has already been answered. Please choose another.\n");
            } else {
                valid_selection = true;
            }
        } while (!valid_selection);

        // --- Display the question ---
        display_question(category, value);

        // --- Get and validate the player's answer ---
        char *tokens[16] = { NULL };
        bool valid_format = false;

        do {
            printf("Your answer (must start with \"what is\" or \"who is\"): ");
            if (fgets(buffer, BUFFER_LEN, stdin) == NULL) goto game_over;
            buffer[strcspn(buffer, "\r\n")] = '\0';

            // tokenize works on a copy since strtok modifies the string
            strncpy(buffer_copy, buffer, BUFFER_LEN - 1);
            buffer_copy[BUFFER_LEN - 1] = '\0';

            // Reset tokens
            for (int i = 0; i < 16; i++) tokens[i] = NULL;
            tokenize(buffer_copy, tokens);

            if (tokens[0] != NULL && tokens[1] != NULL && tokens[2] != NULL &&
                (strcmp(tokens[0], "what") == 0 || strcmp(tokens[0], "who") == 0) &&
                strcmp(tokens[1], "is") == 0) {
                valid_format = true;
            } else {
                printf("  Invalid format. Your answer must start with \"what is\" or \"who is\".\n");
            }
        } while (!valid_format);

        // --- Check the answer ---
        if (valid_answer(category, value, tokens[2])) {
            printf("  Correct! %s earns $%d!\n", current_player, value);
            update_score(players, NUM_PLAYERS, current_player, value);
        } else {
            // Find the correct answer to display
            for (int i = 0; i < NUM_QUESTIONS; i++) {
                if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
                    printf("  Incorrect! The correct answer was: %s\n", questions[i].answer);
                    break;
                }
            }
        }

        // Mark question as answered
        for (int i = 0; i < NUM_QUESTIONS; i++) {
            if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
                questions[i].answered = true;
                break;
            }
        }

        // Print current scores
        printf("\n  Current Scores:\n");
        for (int i = 0; i < NUM_PLAYERS; i++) {
            printf("    %-20s $%d\n", players[i].name, players[i].score);
        }
        printf("\n");
    }

game_over:
    show_results(players, NUM_PLAYERS);
    return EXIT_SUCCESS;
}
