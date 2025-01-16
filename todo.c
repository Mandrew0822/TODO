#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_TODO_LENGTH 256
#define MAX_TODOS 100
#define MAX_PATH_LENGTH 512

char TODO_FILE[MAX_PATH_LENGTH];

// Initialize the todo file path
void init_todo_path() {
    const char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Error: Could not get HOME directory\n");
        exit(1);
    }

    // Create .todo directory if it doesn't exist
    char todo_dir[MAX_PATH_LENGTH];
    snprintf(todo_dir, sizeof(todo_dir), "%s/.todo", home);
    
    struct stat st = {0};
    if (stat(todo_dir, &st) == -1) {
        if (mkdir(todo_dir, 0700) == -1) {
            fprintf(stderr, "Error: Could not create .todo directory\n");
            exit(1);
        }
    }

    // Set the todo file path
    snprintf(TODO_FILE, sizeof(TODO_FILE), "%s/.todo/todo.txt", home);
}

// Function to add a todo item
void add_todo(char *todo_text) {
    FILE *fp = fopen(TODO_FILE, "a");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open todo file\n");
        exit(1);
    }
    fprintf(fp, "%s\n", todo_text);
    fclose(fp);
}

// Function to remove a todo item by number
void remove_todo(int number) {
    FILE *fp = fopen(TODO_FILE, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open todo file\n");
        exit(1);
    }

    char todos[MAX_TODOS][MAX_TODO_LENGTH];
    int count = 0;

    while (fgets(todos[count], MAX_TODO_LENGTH, fp) && count < MAX_TODOS) {
        todos[count][strcspn(todos[count], "\n")] = 0;
        count++;
    }
    fclose(fp);

    if (number < 1 || number > count) {
        fprintf(stderr, "Error: Invalid todo number\n");
        exit(1);
    }

    fp = fopen(TODO_FILE, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open todo file\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        if (i != number - 1) {
            fprintf(fp, "%s\n", todos[i]);
        }
    }
    fclose(fp);
}

// Function to display todos
void display_todos() {
    FILE *fp = fopen(TODO_FILE, "r");
    if (fp == NULL) {
        if (errno == ENOENT) {
            // File doesn't exist yet, create it
            fp = fopen(TODO_FILE, "w");
            if (fp == NULL) {
                fprintf(stderr, "Error: Could not create todo file\n");
                exit(1);
            }
            fclose(fp);
            fp = fopen(TODO_FILE, "r");
        }
        if (fp == NULL) {
            fprintf(stderr, "Error: Could not open todo file\n");
            exit(1);
        }
    }

    printf("TODO LIST:\n");
    printf("---------------\n");

    char line[MAX_TODO_LENGTH];
    int count = 1;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        printf("%d. %s\n", count++, line);
    }
    printf("---------------\n");
    fclose(fp);
}

int main(int argc, char *argv[]) {
    init_todo_path();

    if (argc == 1) {
        display_todos();
        return 0;
    }

    if (argc == 2) {
        char *endptr;
        long num = strtol(argv[1], &endptr, 10);
        if (*endptr == '\0') {
            remove_todo(num);
            return 0;
        }
    }

    char todo_text[MAX_TODO_LENGTH] = "";
    for (int i = 1; i < argc; i++) {
        strcat(todo_text, argv[i]);
        if (i < argc - 1) strcat(todo_text, " ");
    }
    add_todo(todo_text);

    return 0;
}