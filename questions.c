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
#include "questions.h"

// Define the global questions array
question questions[NUM_QUESTIONS];

// Dollar values for each question tier
static int question_values[4] = {100, 200, 300, 400};

// Convert string to lowercase in-place
static void str_to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

// Find index of a question by category (case-insensitive) and value, returns -1 if not found
static int find_question(char *category, int value) {
    char cat_lower[MAX_LEN];
    strncpy(cat_lower, category, MAX_LEN - 1);
    cat_lower[MAX_LEN - 1] = '\0';
    str_to_lower(cat_lower);

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, cat_lower) == 0 && questions[i].value == value) {
            return i;
        }
    }
    return -1;
}

// Initializes the array of questions for the game
void initialize_game(void) {
    // Programming - $100
    strcpy(questions[0].category, "programming");
    strcpy(questions[0].question, "In C, this preprocessor directive is used to define symbolic constants.");
    strcpy(questions[0].answer, "define");
    questions[0].value = 100;
    questions[0].answered = false;

    // Programming - $200
    strcpy(questions[1].category, "programming");
    strcpy(questions[1].question, "This C data type is used to store a single character.");
    strcpy(questions[1].answer, "char");
    questions[1].value = 200;
    questions[1].answered = false;

    // Programming - $300
    strcpy(questions[2].category, "programming");
    strcpy(questions[2].question, "This C standard library function allocates a block of bytes on the heap.");
    strcpy(questions[2].answer, "malloc");
    questions[2].value = 300;
    questions[2].answered = false;

    // Programming - $400
    strcpy(questions[3].category, "programming");
    strcpy(questions[3].question, "This C function releases memory that was previously allocated on the heap.");
    strcpy(questions[3].answer, "free");
    questions[3].value = 400;
    questions[3].answered = false;

    // Algorithms - $100
    strcpy(questions[4].category, "algorithms");
    strcpy(questions[4].question, "In this simple sorting algorithm, adjacent elements are repeatedly swapped.");
    strcpy(questions[4].answer, "bubble");
    questions[4].value = 100;
    questions[4].answered = false;

    // Algorithms - $200
    strcpy(questions[5].category, "algorithms");
    strcpy(questions[5].question, "This efficient search algorithm requires a sorted array and runs in O(log n).");
    strcpy(questions[5].answer, "binary");
    questions[5].value = 200;
    questions[5].answered = false;

    // Algorithms - $300
    strcpy(questions[6].category, "algorithms");
    strcpy(questions[6].question, "Named after its Dutch inventor, this algorithm finds the shortest path in a graph.");
    strcpy(questions[6].answer, "dijkstra");
    questions[6].value = 300;
    questions[6].answered = false;

    // Algorithms - $400
    strcpy(questions[7].category, "algorithms");
    strcpy(questions[7].question, "This O(n log n) sorting algorithm recursively divides and merges subarrays.");
    strcpy(questions[7].answer, "mergesort");
    questions[7].value = 400;
    questions[7].answered = false;

    // Databases - $100
    strcpy(questions[8].category, "databases");
    strcpy(questions[8].question, "This SQL command is used to retrieve data from a database table.");
    strcpy(questions[8].answer, "select");
    questions[8].value = 100;
    questions[8].answered = false;

    // Databases - $200
    strcpy(questions[9].category, "databases");
    strcpy(questions[9].question, "This SQL command permanently removes rows from a table.");
    strcpy(questions[9].answer, "delete");
    questions[9].value = 200;
    questions[9].answered = false;

    // Databases - $300
    strcpy(questions[10].category, "databases");
    strcpy(questions[10].question, "This type of key uniquely identifies each record in a database table.");
    strcpy(questions[10].answer, "primary");
    questions[10].value = 300;
    questions[10].answered = false;

    // Databases - $400
    strcpy(questions[11].category, "databases");
    strcpy(questions[11].question, "This SQL command is used to modify existing records in a database table.");
    strcpy(questions[11].answer, "update");
    questions[11].value = 400;
    questions[11].answered = false;
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void) {
    printf("\n+--------------------+--------------------+--------------------+\n");
    printf("|   %-16s  |   %-16s  |    %-15s  |\n",
           "PROGRAMMING", "ALGORITHMS", "DATABASES");
    printf("+--------------------+--------------------+--------------------+\n");

    for (int v = 0; v < 4; v++) {
        printf("|");
        for (int c = 0; c < NUM_CATEGORIES; c++) {
            int idx = find_question(categories[c], question_values[v]);
            if (idx >= 0 && !questions[idx].answered) {
                printf("        $%-11d|", question_values[v]);
            } else {
                printf("        %-11s|", "----");
            }
        }
        printf("\n");
    }

    printf("+--------------------+--------------------+--------------------+\n\n");
}

// Displays the question for the category and dollar value
void display_question(char *category, int value) {
    int idx = find_question(category, value);
    if (idx >= 0) {
        printf("\n[%s - $%d]\n", questions[idx].category, questions[idx].value);
        printf("Question: %s\n\n", questions[idx].question);
    } else {
        printf("Question not found.\n");
    }
}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer) {
    int idx = find_question(category, value);
    if (idx < 0) return false;

    char ans_lower[MAX_LEN];
    strncpy(ans_lower, answer, MAX_LEN - 1);
    ans_lower[MAX_LEN - 1] = '\0';
    str_to_lower(ans_lower);

    return strcmp(questions[idx].answer, ans_lower) == 0;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value) {
    int idx = find_question(category, value);
    if (idx < 0) return true;   // treat non-existent question as unavailable
    return questions[idx].answered;
}
