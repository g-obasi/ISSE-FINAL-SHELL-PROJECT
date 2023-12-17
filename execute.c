/*
 * execute.c
 *
 * implementation of th execute_pipeline and its helper functions
 *
 * Author: Greenfield Obasi <gstjean@andrew.cmu.edu>
 */


#include "execute.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pipeline.h"
#include <sys/wait.h>


// Exit the shell - Documented in execute.h
void builtin_exit(int argc, char **argv) {
    exit(0);
}

// Print author information Documented in execute.h
void builtin_author(int out_fd, int argc, char **argv) {
    dprintf(out_fd, "Author: Greenfield Obasi St Jean\n");
}

// Change the current directory Documented in execute.h
void builtin_cd(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: cd <directory>\n");
        return;
    }

    if (chdir(argv[1]) != 0) {
        perror("cd");
    }
}

// Print the current directory - Documented in execute.h
void builtin_pwd(int out_fd, int argc, char **argv) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd");
    } else {
        dprintf(out_fd, "%s\n", cwd);
    }
}


// Execute an external command -Documented in execute.h
int forkexec_external_cmd(int in_fd, int out_fd, int next_in_fd, char **argv) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Child process
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        if (next_in_fd != -1) {
            close(next_in_fd);
        }

        execvp(argv[0], argv);
        perror("exec");
        exit(1);
    }

    // Parent process
    if (in_fd != STDIN_FILENO) {
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO) {
        close(out_fd);
    }

    return pid;
}


/*
 * Definition: This function frees up the memory allocated for the arguments
 *             array created by the split_into_commands function.
 *
 * Parameters:
 *   args    a pointer to an array of strings that contains the arguments.
 *
 * Returns:
 *   None
 */
void free_args(char** args) {
    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }
    free(args);
}


/*
 * Definition: This function splits the tokens of a command into separate commands.
 *
 * Parameters:
 *   cmd    a pointer to a Command structure that contains the command's arguments.
 *
 * Returns:
 *   A pointer to a three-dimensional array of characters that represents the
 *   separate commands.
 *
 * Details:
 *   This function first gets the number of arguments in the command. It then
 *   allocates memory for an array of commands. Each command is represented as
 *   an array of strings (i.e., an array of character arrays), so the commands
 *   array is a three-dimensional array of characters.
 *
 *   The function then iterates over each token in the command's arguments. If
 *   the token is a word or a quoted word, it duplicates the token's value and
 *   adds it to the current command. If the token is a pipe, it ends the current
 *   command and starts a new one.
 *
 *   The last element of each command array and the last element of the commands
 *   array are set to NULL to indicate the end of the command and the end of the
 *   commands, respectively.
 */

char*** split_into_commands(Command* cmd) {
    // Get the number of arguments in the command
    int num_args = CL_length(cmd->arguments);

    // Allocate memory for the array of commands
    char*** cmds = (char***) malloc((num_args + 1) * sizeof(char**));

    int cmd_index = 0;
    int arg_index = 0;

    // Allocate memory for the first command
    cmds[cmd_index] = (char**) malloc((num_args + 1) * sizeof(char*));

    // Copy each argument to the array
    for (int i = 0; i < num_args; i++) {
        Token token = CL_nth(cmd->arguments, i);
        if (token.type == TOK_WORD || token.type == TOK_QUOTED_WORD) {
            cmds[cmd_index][arg_index] = strdup(token.value);
            arg_index++;
        } else if (token.type == TOK_PIPE) {
            cmds[cmd_index][arg_index] = NULL;  // End of arguments for this command
            cmd_index++;
            arg_index = 0;
            // Allocate memory for the next command
            cmds[cmd_index] = (char**) malloc((num_args + 1) * sizeof(char*));
        }
    }

    // The last element of the array should be NULL
    cmds[cmd_index][arg_index] = NULL;  // End of arguments for the last command
    cmds[cmd_index + 1] = NULL;  // End of commands

    return cmds;
}


/*
 * Definition: This function prints the commands that are split by the
 *             split_into_commands function.
 *
 * Parameters:
 *   cmds    a pointer to a three-dimensional array of characters that
 *           represents the separate commands.
 *
 * Returns:
 *   None
 *
 * Details:
 *   This function iterates over each command in the cmds array. For each
 *   command, it prints the command number (starting from 1) and then iterates
 *   over each argument in the command. For each argument, it prints the
 *   argument number (starting from 1) and the argument itself.
 *
 *   The function assumes that the cmds array and each command array are
 *   NULL-terminated, i.e., the last element of each array is NULL. This is
 *   used to determine when to stop iterating over the arrays.
 *
 *   The function uses the standard printf function to print the command and
 *   argument numbers and the arguments themselves. The output is sent to the
 *   standard output.
 */

void print_commands(char*** cmds) {
    // Iterate over each command
    for (int i = 0; cmds[i] != NULL; i++) {
        printf("Command %d:\n", i + 1);

        // Iterate over each argument in the command
        for (int j = 0; cmds[i][j] != NULL; j++) {
            printf("  Argument %d: %s\n", j + 1, cmds[i][j]);
        }
    }
}


//execute the pipeline - Documented in execute.h
void execute_pipeline(Pipeline* pl) {
    int num_children = 0;
    int pipefd[2];
    int in_fd = STDIN_FILENO;   // Standard input
    int out_fd = STDOUT_FILENO;  // Standard output
    int next_in_fd = -1;

    // If the pipeline has an input file, open it and set it as the input
    if (pl == NULL) {
        return;
    }else if (pl->input_file != NULL) {
        in_fd = open(pl->input_file, O_RDONLY);
        if (in_fd < 0) {
            perror("open input file");
            return;
        }
    }

    char*** cmds = split_into_commands(pl->head);
    print_commands(cmds);

    if (cmds == NULL || cmds[0][0] == NULL){
        return;
    }
    for (int i = 0; cmds[i] != NULL; i++) {
        char** argv = cmds[i];

        // Calculate argc as the length of argv
        int argc = 0;
        while (argv[argc] != NULL) {
            argc++;
        }

        // If this is not the last command, set up a pipe
        if (cmds[i + 1] != NULL) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            out_fd = pipefd[1];
            next_in_fd = pipefd[0];
        } else if (pl->output_file != NULL) {
            // If this is the last command and the pipeline has an output file,
            // open it and set it as the output
            out_fd = open(pl->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (out_fd < 0) {
                perror("open output file");
                return;
            }
        }

// Handle built-in commands
        if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0) {
            PLD_free(pl);
            builtin_exit(argc, argv);
        } else if (strcasecmp(argv[0], "author") == 0 ) {
            builtin_author(out_fd, argc, argv);
            if (out_fd != STDOUT_FILENO) {
                close(out_fd);
            }
        } else if (strcmp(argv[0], "cd") == 0) {
            builtin_cd(argc, argv);
        } else if (strcmp(argv[0], "pwd") == 0) {
            builtin_pwd(out_fd, argc, argv);
        } else {
            pid_t pid = forkexec_external_cmd(in_fd, out_fd, next_in_fd, argv);
            if (pid > 0) {
                num_children++;
            }
        }

        // Close the input file descriptor if it was opened
        if (in_fd != STDIN_FILENO) {
            close(in_fd);
        }
        in_fd = next_in_fd;
        next_in_fd = -1;
    }

    // Wait for all child processes to finish
    while(num_children > 0){
        int status;
        pid_t pid = wait(&status);
        if (pid == -1) {
            perror("wait");
        }
//        useful for debugging
        else {
            if (WIFEXITED(status)) {
                printf("Child %d terminated normally with exit status %d\n", pid, WEXITSTATUS(status));
            } else {
                printf("Child %d did not terminate normally\n", pid);
            }
        }
        num_children--;
    }
}